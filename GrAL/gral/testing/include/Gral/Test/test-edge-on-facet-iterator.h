#ifndef NMWR_GB_GRAL_TEST_EDGE_ON_FACET_ITERATOR_H
#define NMWR_GB_GRAL_TEST_EDGE_ON_FACET_ITERATOR_H

// $LICENSE

#include <iostream>

//template<class G>
//bool test_edge_on_facet_iterator(G const& g, std::ostream & out);


#ifdef NMWR_INCLUDE_TEMPLATE_DEFS
#include "Gral/Test/test-edge-on-facet-iterator.C"

#else

template<class G>
extern bool test_edge_on_facet_iterator(G const& g, std::ostream & out);

#endif

#endif
