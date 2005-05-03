#ifndef GRAL_GB_GMV_FMT_OUTPUT_H
#define GRAL_GB_GMV_FMT_OUTPUT_H

// $LICENSE_NEC

#include <vector>
#include <string>
#include <algorithm>

#include "Gral/Base/common-grid-basics.h"

#include "Container/heterogeneous-list.h"
#include "Container/histogram-table.h"
#include "Container/bijective-mapping.h"

#include "Utility/as-string.h"
#include "Utility/ref-ptr.h"

#include <boost/type_traits.hpp>

namespace GrAL {

  struct true_type{};
  struct false_type {};
  template<bool B> struct decide        { typedef true_type  type;};
  template<>       struct decide<false> { typedef false_type type;};


/*! \brief Output adapter for the GMV format base class.
   
    Not intended for public use. 
    See OstreamGMVFmt2D and OstreamGMVFmt3D.
 */
class OstreamGMVFmt_base {
  typedef OstreamGMVFmt_base self;

protected:
  std::ostream * out;
  bool           owned;
  int            num_vars;
  bool           material_treat_special;

  void copy(self const& rhs);

public: 
  OstreamGMVFmt_base();
  OstreamGMVFmt_base(std::ostream& ot);
  OstreamGMVFmt_base(std::string const& nm);

  OstreamGMVFmt_base(self const& rhs) { copy(rhs);}
  self & operator=   (self const& rhs) { if(this != &rhs) copy(rhs); return *this; }

  virtual ~OstreamGMVFmt_base();

  void init(std::string const& nm);

  std::ostream& Out() { return *out;}


  template<int N>
  std::string element_tag(vertex_type_tag, grid_dim_tag<N>) const { return "1";}
  std::string element_tag(cell_type_tag,   grid_dim_tag<3>) const { return "0";}
  // surface variables don't take an element tag. 
  std::string element_tag(cell_type_tag,   grid_dim_tag<2>) const { return "";} 
  std::string element_tag(facet_type_tag,  grid_dim_tag<3>) const { return "2";}

  template<class ELEM_TAG>
  std::string material_description(ELEM_TAG,           grid_dim_tag<2>,          int nmats, std::string matnames) const { 
    return "surfmats";
  }

  template<class ELEM_TAG>
  std::string material_description(ELEM_TAG  elem_tag, grid_dim_tag<3> grid_tag, int nmats, std::string matnames) const { 
    std::string res = "material ";
    res += (as_string(nmats) + " ");
    res += as_string(element_tag(elem_tag, grid_tag));
    res += "\n";
    res += matnames;
    return res;
  }

  template<class GF>
  struct gf_name_pair {
    std::string   name;
    GF const*     gf;
    gf_name_pair(std::string const& nm, GF const& ggf)
      : name(nm), gf(&ggf) {}
  };

  template<class GF>
  static gf_name_pair<GF> pair(std::string const& nm, GF const& gf)
    { return gf_name_pair<GF>(nm,gf);}


private:
  // map material ids to names
  struct material_names_impl {
    virtual std::string name(int m) const = 0;
    virtual ~material_names_impl() {}
  };

  template<class MAP>
  struct material_names_impl_tt : public material_names_impl {
  private:
    ref_ptr<MAP> f;
  public:
    material_names_impl_tt(ref_ptr<MAP> ff) : f(ff) {}
    virtual std::string name(int m) const { return (*f)(m);}
  };


  struct material_names {
    ref_ptr<material_names_impl> impl;
  public:
    material_names()  {}

    template<class MAP>    material_names(ref_ptr<MAP> f) { init(f);}

    template<class MAP>    void init(ref_ptr<MAP> f) {
      impl = copy_to_ref_ptr(material_names_impl_tt<MAP>(f)); 
    }
    
    std::string name(int m) const { 
      return (impl != 0 && impl->name(m) != "" ? impl->name(m) : "mat" + as_string(m)); }
  };

  material_names the_material_names;

public:
  template<class MAP>
  void set_material_names(ref_ptr<MAP>  f) { the_material_names.init(f);}

  void output_materials() { material_treat_special = true;}

  template<class MAP>
  void output_materials(ref_ptr<MAP>  f) {
    set_material_names(f);
    material_treat_special = true;
  }


protected:
  virtual void begin_variable() = 0;
  virtual void end_variable()   = 0;
private:
  template<class GF>
  void copy_gf(GF const& gf, std::string const& varname) 
    {
      typedef element_traits<typename GF::element_type> et;
      typedef typename et::element_type_tag   element_type_tag;
      typedef typename et::grid_dimension_tag grid_dimension_tag;

      *out << varname << " ";
      *out << element_tag(element_type_tag(), grid_dimension_tag());
      *out << '\n';
      for(typename et::ElementIterator e(gf.TheGrid()); ! e.IsDone(); ++e) {
	// we assume  numbering in gf.TheGrid() 
	// is the same as numbering in GMV file.
	*out << gf(*e) << ' ';
      }
      *out << '\n' << std::flush;
    }
  template<class GF>
  void copy_material(GF const& gf)
  { 
    typedef typename GF::value_type vt;
    copy_material(gf, typename decide<boost::is_integral<vt>::value>::type()); 
  }

  template<class GF>
  void copy_material(GF const& gf, false_type) {}
 
  template<class GF>
  void copy_material(GF const& gf, true_type) 
  {
    typedef element_traits<typename GF::element_type> et;
    typedef typename et::element_type_tag   element_type_tag;
    typedef typename et::grid_dimension_tag grid_dimension_tag;

    // map material IDs to consecutive range [1, nmats]
    typedef histogram_table<typename GF::value_type> hist_table_type;
    hist_table_type hist(gf.begin(), gf.end());
    unsigned nmats = hist.size();
    if(nmats > 128) {
      std::cerr << "WARNING: GMV supports only 128 materials, but " << nmats << " were given!" << std::endl;
      nmats = 128;
    }

    std::vector<typename GF::value_type> origmat; // (hist.size());
    for(typename hist_table_type::const_iterator it = hist.begin(); it != hist.end(); ++it)
      origmat.push_back((*it).first);
    std::sort(origmat.begin(), origmat.end());
    bijective_mapping<typename GF::value_type, int> new_mat;
    for(unsigned m = 0; m < nmats; ++m)
      new_mat[origmat[m]] = m+1;
    // map possible extraneous materials
    for(unsigned m = nmats; m < origmat.size(); ++m)
      new_mat[origmat[m]] = nmats;
    std::string matnames;
    for(unsigned m = 0; m < nmats; ++m)
      matnames +=  the_material_names.name(origmat[m]) +  "  "; 



    *out << material_description(element_type_tag(), grid_dimension_tag(), nmats, matnames) 
	 << "\n";
    for(typename et::ElementIterator e(gf.TheGrid()); ! e.IsDone(); ++e) {
      *out << new_mat(gf(*e)) << " ";
    }
    *out << "\n" << std::flush;
  }

  typedef heterogeneous_list::END END;
  void copy_grid_functions_rec(heterogeneous_list::List<END,END>) {}

  void copy_materials_rec(heterogeneous_list::List<END,END>) {}


  template<class GF, class TAIL>
  void copy_grid_functions_rec(heterogeneous_list::List<GF,TAIL> gfs)
    { 
      if( ! (material_treat_special && gfs.head().name == "material")) {
	num_vars++;
	copy_gf(*(gfs.head().gf), gfs.head().name);
      }
      copy_grid_functions_rec(gfs.tail());
    }

  template<class GF, class TAIL>
  void copy_materials_rec(heterogeneous_list::List<GF,TAIL> gfs)
    { 
      if(material_treat_special && gfs.head().name == "material") {
        copy_material(*(gfs.head().gf));
      }
      copy_materials_rec(gfs.tail());
    }

 
public:
  void copy_grid_functions(heterogeneous_list::List<END,END>) {}

  template<class GF, class TAIL>
  void copy_grid_functions(heterogeneous_list::List<GF,TAIL> gfs)
    {
      begin_variable();
      copy_grid_functions_rec(gfs);
      end_variable(); 
      copy_materials_rec(gfs);
    }
  
};

} // namespace GrAL 


#endif







