#ifndef NMWR_GB_ADD_PARTIAL_SHARED_H
#define NMWR_GB_ADD_PARTIAL_SHARED_H



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
//  void AddPartialSharedRanges(...);
//
//  collect bilateral shared vertices and facets of partition p 
//  of partitioning Prtng of Grid g.
//  This operates locally, i.e. must be called for each p.
//
//  OUTPUT:
//  ------
//    for all partitions q < p , e \in {v,f}: 
//    Sh^P(q) = Sh^P(q) \cup   {e \in sh_e | e \in E(q)}
//    Sh^Q(p) = Sh^Q(p) \cup eC{e \in sh_e | e \in E(q)}
//
//    [ ovrlp(P).vertices(q).shd = ovrlp(Q).vertices(P).shd \cup vC({v \in sh_v | v \in V(q)})
//      ovrlp(P).facets(q).shd   = ovrlp(Q).facets(P).shd   \cup fC({f \in sh_f | f \in F(q)})
//    ]
//    where vC, fC are correspondance mappings on vertices and facets, resp.
//    These are normally the identity, but are useful for identification
//    of vertices and facets in a periodic geometry.
//
//----------------------------------------------------------------

template<class Partition, class Overlap,
         class Part2Cell,
         class VertexRange, class FacetRange,
         class VtxCorr,     class FacetCorr>
void AddPartialSharedRanges(Overlap          &  ovrlp,    // out: {coarse cell} -> ranges
			    Partition   const&  Prtng,    // in
			    int                 p,
			    Part2Cell   const&  P2C,
			    VertexRange const&  shared_v,
			    FacetRange  const&  shared_f,
			    VtxCorr     const&  vC,
			    FacetCorr   const&  fC);

} // namespace GrAL 

#ifdef NMWR_INCLUDE_TEMPLATE_DEFS
#include "Gral/Distributed/internal/add-partial-shared.C"
#endif

#endif
