#ifndef GRAL_GB_GEOMETRY_TEST_PRIMITIVES3D_C
#define GRAL_GB_GEOMETRY_TEST_PRIMITIVES3D_C

#include "Geometry/test-primitives3d.h"
#include "Geometry/algebraic-primitives.h"
#include "Geometry/primitives3d.h"

#include "Utility/pre-post-conditions.h"


template<class POINT>
void test_algebraic_primitives3d<POINT>::do_tests(std::ostream& out)
{
  typedef point_traits<POINT>         pt;
  typedef dimension_dependent_primitives_3d<POINT> ap;

  POINT b = pt::Origin(3);
  int li = pt::LowerIndex(b); // should be the same for all points

  POINT e[3];
  for(int i = 0; i < 3; ++i) {
    e[i] = pt::Origin(3);
    e[i][li + i] = 1;
    b[li+i] = 1;
  }

  for(int i = 0; i < 3; ++i) {
    int j = (i < 2 ? i+1 : 0);
    int k = (i > 0 ? i-1 : 2);
    out << "ap::det3(e["<<i<<"],e["<<k<<"],e["<<j<<"]) = "
	<< ap::det3(e[i],e[k],e[j]) << '\n'
	<< "ap::det3(e["<<i<<"],e["<<j<<"],e["<<k<<"]) = "
	<< ap::det3(e[i],e[j],e[k]) << '\n'
	<< "ap::vectorproduct(e["<<i<<"],e["<<j<<"]) = "
	<< ap::vectorproduct(e[i],e[j]) << '\n'
	<< "ap::vectorproduct(e["<<i<<"],e["<<k<<"]) = "
	<< ap::vectorproduct(e[i],e[k]) << '\n';

  }

  POINT x = pt::Origin(3);
  ap::solve(e[0],e[1],e[2],x,b);
  out << x << '\n';
  ap::solve(e[0],e[2],e[1],x,b);
  out << x << '\n';

  POINT inv[3];
  for(int i = 0; i < 3; ++i) {
    inv[i] = pt::Origin(3);
  }
  out << "ap::inverse(e[0],e[1],e[2]) = ";
  ap::inverse(e[0],e[1],e[2],inv[0],inv[1],inv[2]);
  out << inv[0] << "  " << inv[1] << "  " << inv[2] << '\n';

  out << "ap::inverse(e[0],e[2],e[1]) = ";
  ap::inverse(e[0],e[2],e[1],inv[0],inv[1],inv[2]);
  out << inv[0] << "  " << inv[1] << "  " << inv[2] << '\n';

  out<< "ap::condition(e[0],e[1],e[2],ap::Norm_frobenius()) = "
     <<  ap::condition(e[0],e[1],e[2],ap::Norm_frobenius()) << "\n";
  out<< "ap::condition(e[0],e[1],e[2],ap::Norm_1()) = "
     <<  ap::condition(e[0],e[1],e[2],ap::Norm_1()) << "\n";
  out<< "ap::condition(e[0],e[1],e[2],ap::Norm_infinity()) = "
     <<  ap::condition(e[0],e[1],e[2],ap::Norm_infinity()) << "\n";

  POINT a[3];
  for(int i = 0; i < 3; ++i) {
    a[i] = pt::Origin(3);
    a[i][li+i] = i+1;
  }


  out << "ap::inverse(a[0],a[1],a[2]) = ";
  ap::inverse(a[0],a[1],a[2],inv[0],inv[1],inv[2]);
  out << inv[0] << "  " << inv[1] << "  " << inv[2] << '\n';
  out << "ap::inverse(a[0],a[1],a[2]) = ";
  ap::inverse(a[0],a[2],a[1],inv[0],inv[1],inv[2]);
  out << inv[0] << "  " << inv[1] << "  " << inv[2] << '\n';

  out << "ap::matrixnorm_frobenius(a[0],a[1],a[2]) = "
      << ap::matrixnorm_frobenius(a[0],a[1],a[2]) << '\n'
      << "ap::matrixnorm_1(a[0],a[1],a[2]) = "
      << ap::matrixnorm_1(a[0],a[1],a[2]) << '\n'
      << "ap::matrixnorm_infinity(a[0],a[1],a[2]) = "
      << ap::matrixnorm_infinity(a[0],a[1],a[2]) << '\n';

  out<< "ap::condition(a[0],a[1],a[2],ap::Norm_frobenius()) = "
     <<  ap::condition(a[0],a[1],a[2],ap::Norm_frobenius()) << "\n";
  out<< "ap::condition(a[0],a[1],a[2],ap::Norm_1()) = "
     <<  ap::condition(a[0],a[1],a[2],ap::Norm_1()) << "\n";
  out<< "ap::condition(a[0],a[1],a[2],ap::Norm_infinity()) = "
     <<  ap::condition(a[0],a[1],a[2],ap::Norm_infinity()) << "\n";


}


#endif
