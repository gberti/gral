#ifndef NMWR_GB_GRAL_TEST_FACE_ITERATOR_H
#define NMWR_GB_GRAL_TEST_FACE_ITERATOR_H

// $LICENSE

#include <iostream>


#ifdef NMWR_INCLUDE_TEMPLATE_DEFS
#include "Gral/Test/test-face-iterator.C"

#else

template<class G>
bool test_face_iterator(G const& g, std::ostream & out);

template<class G, class GT>
bool test_face_iterator(G const& g, std::ostream & out, GT);

#endif

#endif
