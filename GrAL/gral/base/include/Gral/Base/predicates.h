#ifndef GRAL_GB_BASE_PREDICATES_H
#define GRAL_GB_BASE_PREDICATES_H

// $LICENSE

#include "Gral/Base/common-grid-basics.h"


template<class tag1, class tag2, class E1, class E2>
struct incidence { };

template<class E1, class E2>
struct incidence<vertex_type_tag, cell_type_tag,E1,E2> 
{ 
  typedef element_traits<E2> et2;
  typedef grid_types<typename et2::grid_type> gt;
  typedef typename gt::VertexOnCellIterator iterator;
};

template<class E1, class E2>
struct incidence<edge_type_tag, cell_type_tag,E1,E2> 
{ 
  typedef element_traits<E2> et2;
  typedef grid_types<typename et2::grid_type> gt;
  typedef typename gt::EdgeOnCellIterator iterator;
};

template<class E1, class E2>
struct incidence<facet_type_tag, cell_type_tag,E1,E2> 
{ 
  typedef element_traits<E2> et2;
  typedef grid_types<typename et2::grid_type> gt;
  typedef typename gt::FacetOnCellIterator iterator;
};

template<class E1, class E2>
struct incidence<vertex_type_tag, facet_type_tag,E1,E2> 
{ 
  typedef element_traits<E2> et2;
  typedef grid_types<typename et2::grid_type> gt;
  typedef typename gt::VertexOnFacetIterator iterator;
};

template<class E1, class E2>
struct incidence<vertex_type_tag, edge_type_tag,E1,E2> 
{ 
  typedef element_traits<E2> et2;
  typedef grid_types<typename et2::grid_type> gt;
  typedef typename gt::VertexOnEdgeIterator iterator;
};

template<class E1, class E2>
struct incidence<edge_type_tag, facet_type_tag,E1,E2> 
{ 
  typedef element_traits<E2> et2;
  typedef grid_types<typename et2::grid_type> gt;
  typedef typename gt::EdgeOnFacetIterator iterator;
};



/*! predicate to decide wether two elements are incident.

  \todo Tests
  \todo branch on iterator capabilities:
        Use either E1OnE2Iter or E2OnE1Iter.
*/
template<class E1, class E2>
bool incident(E1 const& e1, E2 const& e2)
{ 
  typedef typename element_traits<E1>::element_type_tag tag1;
  typedef typename element_traits<E2>::element_type_tag tag2;
  typedef typename incidence<tag1,tag2,E1,E2>::iterator E1OnE2Iterator;
  for(E1OnE2Iterator e1e2(e2); ! e1e2.IsDone(); ++e1e2)
    if(*e1e2 == e1)
      return true;
  return false;
}



#endif
