#ifndef NMWR_GB_GRAL_TEST_PARTIAL_GRID_FUNCTIONS_H
#define NMWR_GB_GRAL_TEST_PARTIAL_GRID_FUNCTIONS_H

// $LICENSE

#include <iostream>

template<class E>
bool test_partial_grid_functions(typename E::grid_type const& G, 
				 std::ostream& out);

#ifdef NMWR_INCLUDE_TEMPLATE_DEFS
#include "Gral/Test/test-partial-grid-functions.C"
#endif

#endif
