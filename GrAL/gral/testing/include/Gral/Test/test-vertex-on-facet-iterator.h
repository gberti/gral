#ifndef NMWR_GB_GRAL_TEST_VERTEX_ON_FACET_ITERATOR_H
#define NMWR_GB_GRAL_TEST_VERTEX_ON_FACET_ITERATOR_H

// $LICENSE

/*! \file  

    Test of vertex-on-facet iterators (models of $GrAL VertexOnFacetIterator).
*/

#include <iostream>


#ifndef NMWR_INCLUDE_TEMPLATE_DEFS

/*! \brief Test a vertex-on-facet iterator
 
    Test a model of $GrAL VertexOnFacetIterator

    \ingroup incidenceiteratortests
*/
template<class G>
bool test_vertex_on_facet_iterator(G const& g, std::ostream & out);

/*! \brief Test a vertex-on-facet iterator
 
    Test a model of $GrAL VertexOnFacetIterator

    \ingroup incidenceiteratortests
*/
template<class G, class GT>
bool test_vertex_on_facet_iterator(G const& g, std::ostream & out, GT);

#else
#include "Gral/Test/test-vertex-on-facet-iterator.C"
#endif

#endif
