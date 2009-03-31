#ifndef NMWR_GB_GRAL_TEST_FACE_ITERATOR_H
#define NMWR_GB_GRAL_TEST_FACE_ITERATOR_H


/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


/*! \file  

    Test of face iterators (models of $GrAL FaceIterator).

*/


#include <iostream>


#ifdef NMWR_INCLUDE_TEMPLATE_DEFS
#include "Gral/Test/test-face-iterator.C"

#else

namespace GrAL {

/*! \brief Test of face iterators

    Models of $GrAL FaceIterator are tested.
    
    \ingroup sequenceiteratortest
*/
template<class G>
bool test_face_iterator(G const& g, ::std::ostream & out);

/*! \brief Test of face iterators

    Models of $GrAL FaceIterator are tested.
    
    \ingroup sequenceiteratortest
*/
template<class G, class GT>
bool test_face_iterator(G const& g, ::std::ostream & out, GT);

} // namespace GrAL 

#endif

#endif
