#ifndef GRAL_GB_GEOMETRY_TEST_PRIMITIVES2D_C
#define GRAL_GB_GEOMETRY_TEST_PRIMITIVES2D_C

#include "Geometry/test-primitives2d.h"
#include "Geometry/algebraic-primitives.h"
#include "Geometry/primitives2d.h"

#include "Utility/pre-post-conditions.h"


template<class POINT>
void test_algebraic_primitives2d<POINT>::do_tests(std::ostream& out)
{
  typedef point_traits<POINT>         pt;
  typedef dimension_dependent_primitives_2d<POINT> ap;

  POINT b = pt::Origin(2);
  int li = pt::LowerIndex(b); // should be the same for all points

  POINT e[2];
  for(int i = 0; i < 2; ++i) {
    e[i] = pt::Origin(2);
    e[i][li + i] = 1;
    b[li+i] = 1;
  }

  for(int i = 0; i < 2; ++i) {
    int j = (i < 1 ? i+1 : 0);
    out << "ap::det2(e["<<i<<"],e["<<j<<") = "
	<< ap::det2(e[i],e[j]) << '\n';
  }

  POINT x = pt::Origin(2);
  ap::solve2(e[0],e[1],x,b);
  out << x << '\n';
  ap::solve2(e[1],e[0],x,b);
  out << x << '\n';

  POINT inv[2];
  for(int i = 0; i < 2; ++i) {
    inv[i] = pt::Origin(2);
  }
  /*
  ap::inverse(e[0],e[1],inv[0],inv[1]);
  out << inv[0] << "  " << inv[1] << '\n';
  ap::inverse(e[1],e[0],inv[0],inv[1]);
  out << inv[0] << "  " << inv[1] << '\n';
  */

}


#endif
