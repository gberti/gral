#ifndef NMWR_GB_GRAL_TEST_ARCHETYPES_H
#define NMWR_GB_GRAL_TEST_ARCHETYPES_H

// $LICENSE_NEC

#include <iostream>
#include "Gral/Base/common-grid-basics.h"

#ifdef NMWR_INCLUDE_TEMPLATE_DEFS
#include "Gral/Test/test-archetypes.C"
#else

template<class G>
bool test_archetypes(G const& g, std::ostream & out);

template<class G, class GT>
bool test_archetypes(G const& g, std::ostream & out, GT);


#endif

#endif
