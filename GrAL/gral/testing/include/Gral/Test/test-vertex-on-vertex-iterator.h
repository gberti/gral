#ifndef NMWR_GB_GRAL_TEST_VERTEX_ON_VERTEX_ITERATOR_H
#define NMWR_GB_GRAL_TEST_VERTEX_ON_VERTEX_ITERATOR_H

// $LICENSE

#include <iostream>


#ifdef NMWR_INCLUDE_TEMPLATE_DEFS
#include "Gral/Test/test-vertex-on-vertex-iterator.C"
#else

namespace GrAL {

template<class G>
bool test_vertex_on_vertex_iterator(G const& g, ::std::ostream & out);

} // namespace GrAL 

#endif

#endif
