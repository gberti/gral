#ifndef NMWR_GB_COARSE_FROM_PARTITIONING_H
#define NMWR_GB_COARSE_FROM_PARTITIONING_H

//----------------------------------------------------------------
//   (c) Guntram Berti, 1998
//   Chair for Numerical Mathematics & Scientific Computing (NMWR)
//   TU Cottbus - Germany
//   http://math-s.math.tu-cottbus.de/NMWR
//   
//----------------------------------------------------------------


//----------------------------------------------------------------
//  
//  Construct a coarse grid from a partitioning of a fine grid.
//  The cells of the coarse grid correspond to the partitions of
//  the fine grid. This works only for 2D grids (see implementation).
//
//  Note that there is no geometry constructed for the coarse grid.
//  In principle, it may inherit a geometry from the partitioned
//  fine grid (in Partition P) via the correspondences.
//
//  Meaning of Parameters:
//  ---------------------
//  out:
//  G : newly created grid, cells corresponding to partitions of Partitioning P.
//  coarse2fine_v:   maps created coarse vertices to corr. fine vertices
//  coarsecell2part: maps created coarse cells to partition numbers.
//----------------------------------------------------------------

template<class CoarseGrid, class Partition, 
         class CoarseToFineVertex, class CoarseCellToPart>
extern void 
ConstructCoarsePartitionGrid(CoarseGrid& G,      // out
			     const Partition& P, // in
			     CoarseToFineVertex& coarse2fine_v,   // out
			     CoarseCellToPart  & coarsecell2part); // out

#ifdef NMWR_INCLUDE_TEMPLATE_DEFS
#include "Gral/Partitioning/internal/coarse-grid-from-partitioning.C"
#endif
  
#endif
