#ifndef GRAL_GB_TEST_PRIMITIVES2D_H
#define GRAL_GB_TEST_PRIMITIVES2D_H

// $LICENSE

namespace GrAL {

template<class POINT>
struct test_algebraic_primitives2d
{
  void do_tests(::std::ostream & out);
};

} // namespace GrAL {

#ifdef NMWR_INCLUDE_TEMPLATE_DEFS
#include "Geometry/internal/test-primitives2d.C"
#endif


#endif
