#ifndef NMWR_GB_GRAL_TEST_FACE_ITERATOR_H
#define NMWR_GB_GRAL_TEST_FACE_ITERATOR_H

// $LICENSE

/*! \file  

    Test of face iterators (models of $GrAL FaceIterator).

*/


#include <iostream>


#ifdef NMWR_INCLUDE_TEMPLATE_DEFS
#include "Gral/Test/test-face-iterator.C"

#else

/*! \brief Test of face iterators

    Models of $GrAL FaceIterator are tested.
    
    \ingroup sequenceiteratortest
*/
template<class G>
bool test_face_iterator(G const& g, std::ostream & out);

/*! \brief Test of face iterators

    Models of $GrAL FaceIterator are tested.
    
    \ingroup sequenceiteratortest
*/
template<class G, class GT>
bool test_face_iterator(G const& g, std::ostream & out, GT);

#endif

#endif
