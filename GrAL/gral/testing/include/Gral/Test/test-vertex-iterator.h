#ifndef NMWR_GB_GRAL_TEST_VERTEX_ITERATOR_H
#define NMWR_GB_GRAL_TEST_VERTEX_ITERATOR_H

// $LICENSE

#include <iostream>
#include "Gral/Base/common-grid-basics.h"


#ifdef NMWR_INCLUDE_TEMPLATE_DEFS
#include "Gral/Test/test-vertex-iterator.C"
#else

//template<class G, class GT>
//bool test_vertex_iterator_gt(G const& g, std::ostream & out);

template<class G> 
bool test_vertex_iterator(G const& g, std::ostream & out);

template<class G, class GT = grid_types<G> >
bool test_vertex_iterator(G const& g, std::ostream & out, GT);

#endif

#endif
