#ifndef NMWR_GB_GRAL_TEST_EDGE_ITERATOR_H
#define NMWR_GB_GRAL_TEST_EDGE_ITERATOR_H

// $LICENSE

/*! \file  

    Test of edge iterators (models of $GrAL EdgeIterator).

*/

#include <iostream>



#ifdef NMWR_INCLUDE_TEMPLATE_DEFS
#include "Gral/Test/test-edge-iterator.C"

#else

/*! \brief Test of edge iterators

    Models of $GrAL EdgeIterator are tested.
    
    \ingroup sequenceiteratortest
*/
template<class G>
bool test_edge_iterator(G const& g, std::ostream & out);

/*! \brief Test of edge iterators

    Models of $GrAL EdgeIterator are tested.
    
    \ingroup sequenceiteratortest
*/
template<class G, class GT>
bool test_edge_iterator(G const& g, std::ostream & out, GT);

#endif

#endif
