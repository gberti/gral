#ifndef NMWR_GB_GRAL_TEST_VERTEX_ON_EDGE_ITERATOR_H
#define NMWR_GB_GRAL_TEST_VERTEX_ON_EDGE_ITERATOR_H

// $LICENSE

/*! \file  

    Test of vertex-on-edge iterators (models of $GrAL VertexOnEdgeIterator).
*/


#include <iostream>


#ifdef NMWR_INCLUDE_TEMPLATE_DEFS
#include "Gral/Test/test-vertex-on-edge-iterator.C"
#else

namespace GrAL {

/*! \brief Test a vertex-on-edge iterator
 
    Test a model of $GrAL VertexOnEdgeIterator

    \ingroup incidenceiteratortests
 */
template<class G>
bool test_vertex_on_edge_iterator(G const& g, ::std::ostream & out);

/*! \brief Test a vertex-on-edge iterator
 
    Test a model of $GrAL VertexOnEdgeIterator

    \ingroup incidenceiteratortests
 */
template<class G, class GT>
bool test_vertex_on_edge_iterator(G const& g, ::std::ostream & out, GT);

} // namespace GrAL 

#endif

#endif
