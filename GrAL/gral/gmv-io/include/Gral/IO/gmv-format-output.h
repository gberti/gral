#ifndef GRAL_GB_GMV_FMT_OUTPUT_H
#define GRAL_GB_GMV_FMT_OUTPUT_H

// $LICENSE_NEC

#include <vector>
#include <string>

#include "Gral/Base/common-grid-basics.h"

#include "Container/heterogeneous-list.h"
#include "Utility/as-string.h"



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

  template<unsigned N>
  int element_tag(vertex_type_tag, grid_dim_tag<N>) const { return 1;}
  int element_tag(cell_type_tag,   grid_dim_tag<3>) const { return 0;} 
  int element_tag(cell_type_tag,   grid_dim_tag<2>) const { return 2;} 
  int element_tag(facet_type_tag,  grid_dim_tag<3>) const { return 2;}


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

      *out << varname << " " 
	   << element_tag(element_type_tag(), grid_dimension_tag()) 
	   << '\n';;
      for(typename et::ElementIterator e(gf.TheGrid()); ! e.IsDone(); ++e) {
	// we assume  numbering in gf.TheGrid() 
	// is the same as numbering in GMV file.
	*out << gf(*e) << ' ';
      }
      *out << '\n';
    }

  typedef heterogeneous_list::END END;
  void copy_grid_functions_rec(heterogeneous_list::List<END,END>) {}


  template<class GF, class TAIL>
  void copy_grid_functions_rec(heterogeneous_list::List<GF,TAIL> gfs)
    { 
      num_vars++;
      copy_gf(*(gfs.head().gf), gfs.head().name);
      copy_grid_functions_rec(gfs.tail());
    }

 
public:
  void copy_grid_functions(heterogeneous_list::List<END,END>) {}

  template<class GF, class TAIL>
  void copy_grid_functions(heterogeneous_list::List<GF,TAIL> gfs)
    {
      begin_variable();
      copy_grid_functions_rec(gfs);
      end_variable(); 
    }
  
};




#endif







