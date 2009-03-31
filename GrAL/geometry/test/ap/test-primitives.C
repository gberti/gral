/*! \file
   
   Test harness for \c algebraic_primitives.

*/

/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */

#include "Geometry/test-primitives.h"
#include "Geometry/test-primitives2d.h"
#include "Geometry/test-primitives3d.h"
#include "Geometry/point.h"
#include "Geometry/coords.h"

#include <iostream>



int main() {
  using namespace GrAL;

  typedef coordN<3> coord3d_type;
  typedef coordN<2> coord2d_type;
 std::cout << "Testing general primitives: \n\n";
	   
 test_algebraic_primitives<point> test;
 test.do_tests(std::cout);

 std::cout << std::endl
	   << "Testing 2D primitives: \n\n";

 test_algebraic_primitives2d<coord2d_type> test2d;
 test2d.do_tests(std::cout);

 std::cout << std::endl
	   << "Testing 3D primitives: \n\n";

 test_algebraic_primitives3d<coord3d_type> test3d;
 test3d.do_tests(std::cout);

}
