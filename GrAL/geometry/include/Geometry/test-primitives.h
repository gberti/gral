#ifndef GRAL_GB_TEST_PRIMITIVES_H
#define GRAL_GB_TEST_PRIMITIVES_H

// $LICENSE

#include <iostream>

namespace GrAL {

/*! \brief Test dimension-independent primitives.

    POINT must be capable of representing a point 
    with exactly 3 components.
 */
template<class POINT>
struct test_algebraic_primitives 
{
  void do_tests(::std::ostream & out);
};

} // namespace GrAL 


#ifdef NMWR_INCLUDE_TEMPLATE_DEFS
#include "Geometry/internal/test-primitives.C"
#endif


#endif
