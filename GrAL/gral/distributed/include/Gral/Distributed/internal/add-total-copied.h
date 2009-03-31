#ifndef NMWR_GB_ADD_TOTAL_COPIED_H
#define NMWR_GB_ADD_TOTAL_COPIED_H



/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


namespace GrAL {

//----------------------------------------------------------------
//
// Construct the local total overlap ranges from shared_{v,f} and pattern
// as subranges of a global grid.
// This operates locally, i.e. must be called for each p.
//
// OUTPUT:
// -------
// Changed are ovrlp.{cells,vertices}.{copied,exposed}
// ovrlp is the overlap structure corresponding to the partition
// p of Prtng for which inside(p) == true.
//  Cp^P = Cp^P \cup Hull_pat(sh_v, !inside)   (outer hull)
//  Ex^P = Ex^P \cup Hull_pat(sh_v,  inside)   (inner hull)
//
//----------------------------------------------------------------

template<class Overlap, class Partition, class InsidePred, class Pat,
         class VertexRange, class FacetRange>
void AddTotalRanges(Overlap          &  ovrlp,   // out
		    Partition   const&  Prtng,   // in
		    InsidePred  const&  inside,  //
		    Pat         const&  pattern,
		    VertexRange const&  shared_v,
		    FacetRange  const&  shared_f);

} // namespace GrAL 

#ifdef NMWR_INCLUDE_TEMPLATE_DEFS
#include "Gral/Distributed/internal/add-total-copied.C"
#endif

#endif
