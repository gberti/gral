#ifndef NMWR_GB_ADD_TOTAL_SHARED_C
#define NMWR_GB_ADD_TOTAL_SHARED_C



/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti, 
    Contact: guntram AT users DOT berlios DOT de

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include "Gral/Distributed/internal/add-total-shared.h"
#include "Gral/Base/element-sequence-references.h"

namespace GrAL {

//----------------------------------------------------------------
//
//  void AddTotalSharedRanges(...);
//
//  collect shared vertices and facets of partition p 
//  of partitioning Prtng of Grid g.
//  output: 
//    shared_v = shared_v \cup bv(p)
//    shared_f = shared_f \cup bf(p)
//    where
//      bv = {v \in V(p)| \exists q \neq p : v \in V(q)} 
//      bf = {f \in F(p)| \exists q \neq p : f \in F(q)}
//    are the vertices and facets, resp., that are shared
//    with some other partition q of Prtng.
// 
//----------------------------------------------------------------

template<class Grid, class Partition, class VertexRange, class FacetRange>
void AddTotalSharedRanges(VertexRange       & shared_v, // out
			  FacetRange        & shared_f, // out
			  Partition  const& Prtng,      // in
			  int               p,          // in
			  Grid       const& G)          // in
{
  // shared facets are boundary facets of part p that do no ly on grid boundary
  typedef typename Partition::PartBdFacetIterator   PartBdFacetIterator;
  for(PartBdFacetIterator pbf = Prtng.FirstBdFacet(p); ! pbf.IsDone(); ++pbf) 
    if(! G.IsOnBoundary(*pbf)) 
      shared_f.push_back(*pbf);

  // shared vertices are vertices of boundary facets
 
  //  typedef typename VertexRange::Element Vertex;
  //copy_it_range_filter(VerticesOfFacets(shared_f).FirstVertex(), 
  //   		       back_inserter(shared_v.end()), 
  //		       element2handle<Vertex>(shared_f.TheGrid()));
  typedef typename vertex_set_of_facets<FacetRange>::VertexIterator  RgeVertexIterator;
  for(RgeVertexIterator v = VerticesOfFacets(shared_f).FirstVertex(); ! v.IsDone(); ++v)
    shared_v.push_back(*v);
}

} // namespace GrAL 

#endif
