#ifndef NMWR_GB_CONSTRUCT_OVERLAP_H
#define NMWR_GB_CONSTRUCT_OVERLAP_H


// $LICENSE

namespace GrAL {

/*! Construct an overlap from a partitioning and a stencil (overlap pattern)
    \ingroup overlap
    
   \templateparams
    - \c Overlap: e.g dyn_overlap<CoarseGrid,Partition::grid_type> 
    - \c CoarseGrid: $GrAL CellGridRange with $GrAL CellOnCellIterator
    - \c Partition: e.g. partitioning<fine_grid_type>
    - \c OVPattern: e.g. \c overlap_pattern or \queue<type_tag>
    - \c Part2Cell: mapping [0, ... n-1] to cells of \c CrsG 
    - \c Cell2Part: inverse mapping
    - \c GridMap: e.g. \c GridSelfMap<fine_grid_type>. In general the identity,
      except for periodic grids

 */
template<class Overlap, class CoarseGrid, 
         class Partition, class OVPattern,
         class Part2Cell, class Cell2Part,
         class GridMap>
void ConstructOverlap(Overlap          & Ovlp,         // out
		      CoarseGrid  const& CrsG,         
		      Partition   const& Prtng,
		      OVPattern   const& ovlp_pattern,
		      Part2Cell   const& part2cell,
		      Cell2Part   const& cell2part,
		      GridMap     const& grid_map);

} // namespace GrAL 


#ifdef NMWR_INCLUDE_TEMPLATE_DEFS
#include "Gral/Distributed/internal/construct-overlap.C"
#endif

#endif
