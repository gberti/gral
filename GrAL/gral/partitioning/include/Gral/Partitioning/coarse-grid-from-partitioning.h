#ifndef NMWR_GB_COARSE_FROM_PARTITIONING_H
#define NMWR_GB_COARSE_FROM_PARTITIONING_H


// $LICENSE

/*! \file
 */

/*! \defgroup quotientgrid Quotient grid Components
    \ingroup partitioningmodule
*/

namespace GrAL {

/*! \brief   Construct a coarse grid from a partitioning of a fine grid.
  
  \ingroup quotientgrid
  
  The cells of the coarse grid correspond to the partitions of
  the fine grid. This works only for 2D grids (see implementation).

  Note that there is no geometry constructed for the coarse grid.
  In principle, it may inherit a geometry from the partitioned
  fine grid (in Partition P) via the correspondences.

 
  \param  G  (Output) Coarse grid with  cells corresponding to partitions of \c P
  \param  P  (Input) Partitioning 
  \param  coarse2fine_v  (Output)  maps created coarse vertices to corresponding fine vertices
  \param coarsecell2part (Output)  maps created coarse cells to partition numbers.

*/

template<class CoarseGrid, class Partition, 
         class CoarseToFineVertex, class CoarseCellToPart>
extern void 
ConstructCoarsePartitionGrid(CoarseGrid& G,      // out
			     const Partition& P, // in
			     CoarseToFineVertex& coarse2fine_v,   // out
			     CoarseCellToPart  & coarsecell2part); // out

} // namespace GrAL

#ifdef NMWR_INCLUDE_TEMPLATE_DEFS
#include "Gral/Partitioning/internal/coarse-grid-from-partitioning.C"
#endif
  
#endif
