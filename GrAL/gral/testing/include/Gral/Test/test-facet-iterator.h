#ifndef NMWR_GB_GRAL_TEST_FACET_ITERATOR_H
#define NMWR_GB_GRAL_TEST_FACET_ITERATOR_H

// $LICENSE

#include <iostream>



#ifdef NMWR_INCLUDE_TEMPLATE_DEFS
#include "Gral/Test/test-facet-iterator.C"

#else

template<class G>
bool test_facet_iterator(G const& g, std::ostream & out);

template<class G, class GT>
bool test_facet_iterator(G const& g, std::ostream & out, GT);


#endif

#endif
