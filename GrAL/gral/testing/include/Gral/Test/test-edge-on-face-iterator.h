#ifndef NMWR_GB_GRAL_TEST_EDGE_ON_FACE_ITERATOR_H
#define NMWR_GB_GRAL_TEST_EDGE_ON_FACE_ITERATOR_H

// $LICENSE

/*! \file  

    Test of edge-on-face iterators (models of $GrAL EdgeOnFaceIterator).

*/

#include <iostream>


#ifdef NMWR_INCLUDE_TEMPLATE_DEFS
#include "Gral/Test/test-edge-on-face-iterator.C"
#else

/*! \brief Test a edge-on-face iterator
 
    Test a model of $GrAL EdgeOnFaceIterator

    \ingroup incidenceiteratortests
 */
template<class G>
bool test_edge_on_face_iterator(G const& g, std::ostream & out);

/*! \brief Test a edge-on-face iterator
 
    Test a model of $GrAL EdgeOnFaceIterator

    \ingroup incidenceiteratortests
 */
template<class G, class GT>
bool test_edge_on_face_iterator(G const& g, std::ostream & out, GT);

#endif

#endif
