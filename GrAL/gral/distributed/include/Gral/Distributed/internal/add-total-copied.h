#ifndef NMWR_GB_ADD_TOTAL_COPIED_H
#define NMWR_GB_ADD_TOTAL_COPIED_H

//----------------------------------------------------------------
//   (c) Guntram Berti, 1998
//   Chair for Numerical Mathematics & Scientific Computing (NMWR)
//   TU Cottbus - Germany
//   http://math-s.math.tu-cottbus.de/NMWR
//   
//----------------------------------------------------------------

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


#ifdef NMWR_INCLUDE_TEMPLATE_DEFS
#include "Grids/Distributed/generic/add-total-copied.C"
#endif

#endif
