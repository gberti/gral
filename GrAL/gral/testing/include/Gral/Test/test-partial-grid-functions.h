#ifndef NMWR_GB_GRAL_TEST_PARTIAL_GRID_FUNCTIONS_H
#define NMWR_GB_GRAL_TEST_PARTIAL_GRID_FUNCTIONS_H

// $LICENSE

/*! \file  

    Test of partial grid functions 
    (models of $GrAL PartialGridFunction concept).

 */

#include <iostream>

/*! \brief Test of partial grid functions
    
     Test models of the $GrAL PartialGridFunction concept.
     \templateparams
      - \c E is a model of $GrAL GridElement. A partial grid function
         defined on elements of type \c E is tested.
    \ingroup gridfunctiontests

    Example:
    \code
    MyGrid g;
    typedef grid_types<MyGrid> gt;
    // test partial grid functions on vertices
    test_partial_grid_function<gt::Vertex>(g, std::cout);
    \endcode
*/

template<class E>
bool test_partial_grid_functions(typename E::grid_type const& G, 
				 std::ostream& out);

#ifdef NMWR_INCLUDE_TEMPLATE_DEFS
#include "Gral/Test/test-partial-grid-functions.C"
#endif

#endif
