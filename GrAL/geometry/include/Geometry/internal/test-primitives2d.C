#ifndef GRAL_GB_GEOMETRY_TEST_PRIMITIVES2D_C
#define GRAL_GB_GEOMETRY_TEST_PRIMITIVES2D_C


/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti, 
    Contact: guntram AT users DOT berlios DOT de

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


/*! \file

*/

#include "Geometry/test-primitives2d.h"
#include "Geometry/algebraic-primitives.h"
#include "Geometry/primitives2d.h"

#include "Utility/pre-post-conditions.h"

namespace GrAL {

/*! \brief Test 2D primitives in dimension_dependent_primitives_2d
  
    \ingroup algebraicprimitives 
*/
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
 
 
  ap::invert(e[0],e[1],inv[0],inv[1]);
  out << inv[0] << "  " << inv[1] << '\n';
  ap::invert(e[1],e[0],inv[0],inv[1]);
  out << inv[0] << "  " << inv[1] << '\n';

  typename ap::matrix_type A(0);
  typename ap::matrix_type B(ap::matrix_type::UnitMatrix());
  typename ap::matrix_type C = A*B;

  out << "A= " << A[li+0] << "  " << A[li+1] << "\n"
      << "B= " << B[li+0] << "  " << B[li+1] << "\n"
      << "C= " << C[li+0] << "  " << C[li+1] << "\n";
 
  A(li+0,li+0) = 1;
  A(li+1,li+1) = 2;
  out << "A=   "   << A[li+0] << "  " << A[li+1] << "\n";
  C = A*B;
  out << "A*B= " << C[li+0] << "  " << C[li+1] << "\n";
  typename ap::matrix_type Ainv = ap::inverse(A);
  out << "A^-1 = " << Ainv[li+0] << "  " << Ainv[li+1] << "\n";

  typename ap::matrix_type D(0);
  D(li+0, li+1) = 1;
  D(li+1, li+0) = 1;
  out << "D=     " << D[li+0] << "  " << D[li+1] << "\n";
  typename ap::matrix_type Dinv = ap::inverse(D);
  out << "D^-1 = " << Dinv[li+0] << "  " << Dinv[li+1] << "\n";

  D *= D;
  out << "D^2 =  " << D[li+0] << "  " << D[li+1] << "\n";

  
}

} // namespace GrAL 

#endif
