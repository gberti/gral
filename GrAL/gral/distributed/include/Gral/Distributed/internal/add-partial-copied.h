#ifndef NMWR_GB_ADD_PARTIAL_COPIED_H
#define NMWR_GB_ADD_PARTIAL_COPIED_H



/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti, 
    Contact: guntram AT users DOT berlios DOT de

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


namespace GrAL {

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


} // namespace GrAL 

#ifdef NMWR_INCLUDE_TEMPLATE_DEFS
#include "Gral/Distributed/internal/add-partial-copied.C"
#endif

#endif
