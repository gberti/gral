#ifndef NMWR_GB_GRIDS_TRIANG2D_CONSTRUCT_H
#define NMWR_GB_GRIDS_TRIANG2D_CONSTRUCT_H

//----------------------------------------------------------------
//   (c) Guntram Berti, 1999
//   Chair for Numerical Mathematics & Scientific Computing (NMWR)
//   TU Cottbus - Germany
//   http://math-s.math.tu-cottbus.de/NMWR
//   
//----------------------------------------------------------------


#include "Grids/Algorithms/construct-grid.h"
#include "Grids/Triang2D/triang2d.h"

//----------------------------------------------------------------
// copy-construct a Triang2D <dest> from another 2D grid <source_grid>
// supporting 
//  - grid_types<>, 
//  - CellIterator, VertexIterator, VertexOnCellIterator,
//  - vertex_handle
// This template is defined in generic/construct.C
//----------------------------------------------------------------

//----------------------------------------------------------------
//  Partial specializations of the templates in 
//  Grids/Algorithms/construct-grid.h
//----------------------------------------------------------------

template<class Geom, class G2, class Geom2>
extern  void 
ConstructGrid(Triang2D   & destG,
	      Geom       & destGeom,
	      G2    const& srcG, 
	      Geom2 const& srcGeom);

template<class Geom, class G2, class Geom2, class VertexMap>
extern void 
ConstructGridV(Triang2D      & destG,
	       Geom          & destGeom, 
	       G2       const& srcG,
	       Geom2    const& srcGeom,
	       VertexMap     & VCorrG2_G1); 
// VertexCorr  maps G2::vertex_handles to  G1::vertex_handles
// for transfer of grid-related information, e.g. grid-functions.

template<class Geom, class G2, class Geom2, class VertexMap, class CellMap>
extern void 
ConstructGridVC(Triang2D      & destG, 
		Geom          & destGeom, 
		G2       const& srcG,
		Geom2    const& srcGeom,
		VertexMap     & VCorrG2_G1,
		CellMap       & CCorrG2_G1); 
// CellCorr  maps G2::cell_handles to  G1::vertex_handles
// for transfer of grid-related information, e.g. grid-functions.

template<class G2, class VertexMap, class CellMap>
extern void 
ConstructGrid0(Triang2D      & destG, 
	       G2       const& srcG,
	       VertexMap     & VCorrG2_G1,
	       CellMap       & CCorrG2_G1); 

template<class G2, class VertexMap, class CellMap>
extern void 
ConstructGrid0(Triang2D      & destG, 
	       G2       const& srcG);

// for inclusion model

#ifdef NMWR_INCLUDE_TEMPLATE_DEFS
#include "Grids/Triang2D/generic/construct.C"
#endif

#endif
