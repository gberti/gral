
#include "Geometry/test-primitives.h"
#include "Geometry/test-primitives2d.h"
#include "Geometry/test-primitives3d.h"
#include "Geometry/point.h"

#include <iostream>

/*
typedef double double3[3];

template<>
struct point_traits<double3>
{
  struct Ptype {
    double3 X;
    void operator=(const double3 Y) { X[0]=Y[0];X[1]=Y[1];X[2]=Y[2];}
    operator double* () { return X;}
  };

  typedef double component_type;
  static double3 orig;
  static double3& Origin()         { return orig;}
  static double3& Origin(unsigned) { return orig;}

  static int LowerIndex(const double3 ) { return 0;}
  static int UpperIndex(const double3) { return 2;}
  static int Dim(const double3 ) { return 3;}
  static int Dim()               { return 3;}
  static double  x(double3 const& p) { return p[0];}
  static double& x(double3      & p) { return p[0];}
  static double  y(double3 const& p) { return p[1];}
  static double& y(double3      & p) { return p[1];}
  static double  z(double3 const& p) { return p[2];}
  static double& z(double3      & p) { return p[2];}
};

double3 point_traits<double3>::orig = {0,0,0};

template<>
class point_traits<float[3]>
{};
*/


int main() {

  /*
 test_algebraic_primitives<double3> test_d3;
 test_d3.do_tests(std::cout);
  */

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
