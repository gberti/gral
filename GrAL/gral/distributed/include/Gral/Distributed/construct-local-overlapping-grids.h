#ifndef NMWR_GB_CONSTRUCT_OVERLAPPING_GRIDS_H
#define NMWR_GB_CONSTRUCT_OVERLAPPING_GRIDS_H

//----------------------------------------------------------------
//   (c) Guntram Berti, 1998
//   Chair for Numerical Mathematics & Scientific Computing (NMWR)
//   TU Cottbus - Germany
//   http://math-s.math.tu-cottbus.de/NMWR
//   
//----------------------------------------------------------------

//----------------------------------------------------------------
//
// semantics:
// ovlp_grid.TheGrid()    = Grid(Cells(owned \cup ovlp.copied()))
// ovlp_grid.TheOverlap() = ovlp
//
//----------------------------------------------------------------


template<class OvlpGrid, class CellRange, class Geometry,
         class Overlap, class VertexCorr, class CellCorr>
void ConstructLocalOverlappingGrid(OvlpGrid       & ovlp_grid,
				   CellRange const& owned_c,
				   Geometry  const& geom, 
				   Overlap        & ovlp,
				   VertexCorr     & v_corr,
				   CellCorr       & c_corr);


#ifdef NMWR_INCLUDE_TEMPLATE_DEFS
#include "Gral/Distributed/internal/construct-local-overlapping-grids.C"
#endif

#endif
