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

  void output_materials() { material_treat_special = true;}

  template<int N>
  std::string element_tag(vertex_type_tag, grid_dim_tag<N>) const { return "1";}
  std::string element_tag(cell_type_tag,   grid_dim_tag<3>) const { return "0";}
  // surface variables don't take an element tag. 
  std::string element_tag(cell_type_tag,   grid_dim_tag<2>) const { return "";} 
  std::string element_tag(facet_type_tag,  grid_dim_tag<3>) const { return "2";}


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
      *out << '\n';
    }
  template<class GF>
  void copy_material(GF const& gf)
  { 
    typedef typename GF::value_type vt;
    copy_material(gf, decide<boost::is_integral<vt>::value>::type()); 
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

    std::vector<typename GF::value_type> mat; // (hist.size());
    for(typename hist_table_type::const_iterator it = hist.begin(); it != hist.end(); ++it)
      mat.push_back((*it).first);
    std::sort(mat.begin(), mat.end());
    bijective_mapping<typename GF::value_type, int> new_mat;
    for(unsigned m = 0; m < nmats; ++m)
      new_mat[mat[m]] = m+1;
    // map possible extraneous materials
    for(unsigned m = nmats; m < mat.size(); ++m)
      new_mat[mat[m]] = nmats;


    *out << "material "
	 << nmats << " "
	 << element_tag(element_type_tag(), grid_dimension_tag())
	 << "\n";
    for(unsigned m = 0; m < nmats; ++m)
      *out << "mat" << mat[m] << " ";
    *out << "\n";
    for(typename et::ElementIterator e(gf.TheGrid()); ! e.IsDone(); ++e) {
      *out << new_mat(gf(*e)) << " ";
    }
    *out << "\n";
  }

  typedef heterogeneous_list::END END;
  void copy_grid_functions_rec(heterogeneous_list::List<END,END>) {}
 typedef heterogeneous_list::END END;
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







