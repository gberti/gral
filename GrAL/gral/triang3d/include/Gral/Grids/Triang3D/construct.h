#ifndef GRAL_GB_GRIDS_TRIANG3D_CONSTRUCT_H
#define GRAL_GB_GRIDS_TRIANG3D_CONSTRUCT_H

// $LICENSE 


#include "Gral/Base/construct-grid.h"
#include "Gral/Grids/Triang3D/triang3d.h"


/*! copy-construct a Triang3D <dest> from another 3D grid <source_grid>
   supporting 
    - grid_types<>, 
    - CellIterator, VertexIterator, VertexOnCellIterator,
    - vertex_handle
  This template is defined in generic/construct.C
*/


template<class Geom, class G2, class Geom2>
extern  void 
ConstructGrid(Triang3D   & destG,
	      Geom       & destGeom,
	      G2    const& srcG, 
	      Geom2 const& srcGeom);

template<class Geom, class G2, class Geom2, class VertexMap>
extern void 
ConstructGridV(Triang3D      & destG,
	       Geom          & destGeom, 
	       G2       const& srcG,
	       Geom2    const& srcGeom,
	       VertexMap     & VCorrG2_G1); 
// VertexCorr  maps G2::vertex_handles to  G1::vertex_handles
// for transfer of grid-related information, e.g. grid-functions.

template<class Geom, class G2, class Geom2, class VertexMap, class CellMap>
extern void 
ConstructGridVC(Triang3D      & destG, 
		Geom          & destGeom, 
		G2       const& srcG,
		Geom2    const& srcGeom,
		VertexMap     & VCorrG2_G1,
		CellMap       & CCorrG2_G1); 
// CellCorr  maps G2::cell_handles to  G1::vertex_handles
// for transfer of grid-related information, e.g. grid-functions.

template<class G2, class VertexMap, class CellMap>
extern void 
ConstructGrid0(Triang3D      & destG, 
	       G2       const& srcG,
	       VertexMap     & VCorrG2_G1,
	       CellMap       & CCorrG2_G1); 

template<class G2, class VertexMap, class CellMap>
extern void 
ConstructGrid0(Triang3D      & destG, 
	       G2       const& srcG);

// for inclusion model

#ifdef NMWR_INCLUDE_TEMPLATE_DEFS
#include "Gral/Grids/Triang3D/construct.C"
#endif

#endif
