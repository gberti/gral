#ifndef NMWR_GB_ADD_PARTIAL_COPIED_H
#define NMWR_GB_ADD_PARTIAL_COPIED_H

//----------------------------------------------------------------
//   (c) Guntram Berti, 1998
//   Chair for Numerical Mathematics & Scientific Computing (NMWR)
//   TU Cottbus - Germany
//   http://math-s.math.tu-cottbus.de/NMWR
//   
//----------------------------------------------------------------

//----------------------------------------------------------------
//
// construct partial (= pairwise) synchronized overlap ranges
// as subranges of a global grid using a partition Prtng of the 
// cells and the total overlap ranges.
// This operates locally, i.e. must be called for each p.
// 
//----------------------------------------------------------------

template<class Overlap,     class Partition, 
         class Part2Cell,
         class VertexRange, class CellRange,
         class VtxCorr,     class CellCorr>
void AddPartialRanges(Overlap          &    ovrlp,     // out
		      Partition   const&    Prtng,
		      int                   p,
		      Part2Cell   const&    P2C,
		      VertexRange const&    copied_v,
		      CellRange   const&    copied_c,
		      VtxCorr     const&    vtx_corr,
		      CellCorr    const&    cell_corr);


#ifdef NMWR_INCLUDE_TEMPLATE_DEFS
#include "Grids/Distributed/generic/add-partial-copied.C"
#endif

#endif
