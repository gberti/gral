
#include "Geometry/test-primitives.h"
#include "Geometry/test-primitives2d.h"
#include "Geometry/test-primitives3d.h"
#include "Geometry/point.h"

#include <iostream>



int main() {

 std::cout << "Testing general primitives: \n\n";
	   
 test_algebraic_primitives<point> test;
 test.do_tests(std::cout);

 std::cout << std::endl
	   << "Testing 2D primitives: \n\n";

 test_algebraic_primitives2d<point> test2d;
 test2d.do_tests(std::cout);

 std::cout << std::endl
	   << "Testing 3D primitives: \n\n";

 test_algebraic_primitives3d<point> test3d;
 test3d.do_tests(std::cout);

}
