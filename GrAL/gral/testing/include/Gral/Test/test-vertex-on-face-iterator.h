#ifndef NMWR_GB_GRAL_TEST_VERTEX_ON_FACE_ITERATOR_H
#define NMWR_GB_GRAL_TEST_VERTEX_ON_FACE_ITERATOR_H

// $LICENSE

#include <iostream>



#ifdef NMWR_INCLUDE_TEMPLATE_DEFS

#include "Gral/Test/test-vertex-on-face-iterator.C"

#else

template<class G>
bool test_vertex_on_face_iterator(G const& g, std::ostream & out);

#endif

#endif
