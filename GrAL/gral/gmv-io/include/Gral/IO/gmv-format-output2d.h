#ifndef GRAL_GB_GMV_FMT_OUTPUT2D_H
#define GRAL_GB_GMV_FMT_OUTPUT2D_H

// $LICENSE_NEC

#include <vector>
#include <string>

#include "Gral/Grids/Complex2D/all.h"
#include "Container/bijective-mapping.h"

#include "Gral/Base/element-numbering.h"
#include "Gral/Base/partial-grid-morphism.h"
#include "Gral/Algorithms/construct-isomorphism.h"

#include "Container/inhomogeneous-list.h"
#include "Utility/as-string.h"


/*! \brief Output adapter for the GMV format (2D grids).

    The <A HREF= "http://www-xdiv.lanl.gov/XCM/gmv/"> 
    General Mesh Viewer </A> (GMV) is a versatile tool
    for viewing 3D meshes and fields defined on them.

    Usage: (\c MyGrid is 2D grid object, model of $GrAL CellVertexInputGrid)
    \code
    OstreamGMV2DFmt Out("grid.gmv");
    ConstructGrid(Out,MyGrid, MyGeom); 
    \endcode

   \todo support output of named grid functions.
 */
class OstreamGMV2DFmt {
  typedef OstreamGMV2DFmt self;
public:
  typedef Complex2D archetype_type;
protected:
  std::ostream * out;
  bool           owned;
  int num_vars;
private:
  void copy(self const& rhs);

public: 
  OstreamGMV2DFmt();
  OstreamGMV2DFmt(std::ostream& ot);
  OstreamGMV2DFmt(std::string const& nm);

  OstreamGMV2DFmt(self const& rhs) { copy(rhs);}
  self & operator=   (self const& rhs) { if(this != &rhs) copy(rhs); return *this; }

  ~OstreamGMV2DFmt();

  void init(std::string const& nm);

  std::ostream& Out() { return *out;}

  template<unsigned N>
  int element_tag(vertex_type_tag, grid_dim_tag<N>) const { return 1;}
  int element_tag(cell_type_tag,   grid_dim_tag<3>) const { return 0;} 
  int element_tag(cell_type_tag,   grid_dim_tag<2>) const { return 2;} 
  int element_tag(facet_type_tag,  grid_dim_tag<3>) const { return 2;}

  template<class GF > //, class GRID = typename GF::grid_type>
  void copy_gf(GF const& gf) // , GRID const& G = gf.TheGrid()) 
    {
      typedef element_traits<typename GF::element_type> et;
      std::string varname = "testvar" + as_string(num_vars); // FIXME!

      typedef typename et::element_type_tag   element_type_tag;
      typedef typename et::grid_dimension_tag grid_dimension_tag;
      *out << varname << " " 
	   << element_tag(element_type_tag()) 
	   << '\n';;
      for(typename et::ElementIterator e(gf.TheGrid()); ! e.IsDone(); ++e) {
	// we assume  numbering in gf.TheGrid() 
	// is the same as numbering in GMV file.
	*out << gf(*e) << ' ';
      }
    }

  void copy_grid_functions_rec(List<END,END>) {}

  template<class GF, class TAIL>
  void copy_grid_functions_rec(List<GF,TAIL> gfs)
    { 
      num_vars++;
      copy_gf(gfs.head());
      copy_grid_functions(gfs.tail());
    }

 
  void copy_grid_functions(List<END,END>) {}

  template<class GF, class TAIL>
  void copy_grid_functions(List<GF,TAIL> gfs)
    {
      *out << "surfvars" << '\n';
      copy_grid_functions_rec(gfs);
      *out << "\n" << "endsvars" << "\n";
    }
};



/*! \brief ConstructGrid overload for OstreamGMV2DFmt

  \relates OstreamGMV2DFmt
  
  \see Module \ref GMV2Dformat
  \see Module \ref mutatingoperations
  \see \ref ConstructGrid
 */
template<class GRID,class GEOM>
void ConstructGrid(OstreamGMV2DFmt& Out, 
		   GRID const& G,
		   GEOM const& GEO);

template<class GRID,class GEOM, class GF, class MOREGFS>
void ConstructGrid(OstreamGMV2DFmt& Out, 
		   GRID const& G,
		   GEOM const& GEO,
		   List<GF,MOREGFS> GFS);

#ifdef NMWR_INCLUDE_TEMPLATE_DEFS
#include "Gral/IO/gmv-format-output2d.tt.C"
#endif


#endif







