/*! \file
   
   Test harness for \c algebraic_primitives.

*/
#include "Geometry/test-primitives.h"
#include "Geometry/test-primitives2d.h"
#include "Geometry/test-primitives3d.h"
#include "Geometry/point.h"
#include "Geometry/coords.h"

#include <iostream>



int main() {

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
