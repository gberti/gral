#ifndef NMWR_GB_CONSTRUCT_OVERLAP_H
#define NMWR_GB_CONSTRUCT_OVERLAP_H

//----------------------------------------------------------------
//   (c) Guntram Berti, 1998
//   Chair for Numerical Mathematics & Scientific Computing (NMWR)
//   TU Cottbus - Germany
//   http://math-s.math.tu-cottbus.de/NMWR
//   
//----------------------------------------------------------------

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


#ifdef NMWR_INCLUDE_TEMPLATE_DEFS
#include "Gral/Distributed/internal/construct-overlap.C"
#endif

#endif
