#ifndef GRAL_GB_GEOMETRY_TEST_PRIMITIVES3D_C
#define GRAL_GB_GEOMETRY_TEST_PRIMITIVES3D_C


/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


/*! \file

*/

#include "Geometry/test-primitives3d.h"
#include "Geometry/algebraic-primitives.h"
#include "Geometry/geometric-primitives.h"
#include "Geometry/primitives3d.h"

#include "Utility/pre-post-conditions.h"

#include <vector>

namespace GrAL {

template<class P>
void check_intersections(P const& s0, P const& s1,
			 P const& t0, P const& t1, P const& t2,
			 std::ostream& out)
{

  typedef segment<P>   segment3;
  typedef ray<P>       ray3;
  typedef triangle<P>  triangle3;
  typedef intersection_segment_triangle<segment3, triangle3> intersection_st;
  typedef intersection_ray_triangle<ray3, triangle3>         intersection_rt;

  segment3  S(s0, s1);
  triangle3 T(t0, t1, t2);
  ray3      R(s0, s1 -s0);
  intersection_st  IS(S,T);
  intersection_rt  IR(R,T);
  
  out << "\n" 
      << "Segment:  [" << S.p0() << "," << S.p1() << "]\n"
      << "Triangle: [" << T.p0() << "," << T.p1() << "," << T.p2() << "]\n"
      << "Ray:      [" << R.p0() << "," << R.dir() << "]\n";

  // segment intersection
  out << "segment does" 
      << (IS.segment_intersects_plane() ? " " : " not ")
      << "intersect plane\n"
      << "segment does" 
      << (IS.segment_intersects_triangle() ? " " : " not ")
      << "intersect triangle\n";
  if(IS.segment_intersects_plane())
    out << "plane intersection: " << IS.intersection() << '\n'; 

  // ray intersection
  out << "line does" 
      << (IR.ray_intersects_plane() ? " " : " not ")
      << "intersect plane\n"
      << "ray does" 
      << (IR.ray_intersects_triangle() ? " " : " not ")
      << "intersect triangle\n";
  if(IR.ray_intersects_plane())
    out << "plane intersection: " << IR.intersection() << '\n'; 
}


/*! \brief Test 3D primitives in dimension_dependent_primitives_3d
  
    \ingroup algebraicprimitives 
*/
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
  ap::solve3(e[0],e[1],e[2],x,b);
  out << x << '\n';
  ap::solve3(e[0],e[2],e[1],x,b);
  out << x << '\n';

  POINT inv[3];
  for(int i = 0; i < 3; ++i) {
    inv[i] = pt::Origin(3);
  }
  out << "ap::inverse(e[0],e[1],e[2]) = ";
  ap::invert(e[0],e[1],e[2],inv[0],inv[1],inv[2]);
  out << inv[0] << "  " << inv[1] << "  " << inv[2] << '\n';

  out << "ap::inverse(e[0],e[2],e[1]) = ";
  ap::invert(e[0],e[2],e[1],inv[0],inv[1],inv[2]);
  out << inv[0] << "  " << inv[1] << "  " << inv[2] << '\n';

  out<< "ap::condition(e[0],e[1],e[2],ap::Norm_frobenius()) = "
     <<  ap::condition(e[0],e[1],e[2],typename ap::Norm_frobenius()) << "\n";
  out<< "ap::condition(e[0],e[1],e[2],ap::Norm_1()) = "
     <<  ap::condition(e[0],e[1],e[2],typename ap::Norm_1()) << "\n";
  out<< "ap::condition(e[0],e[1],e[2],ap::Norm_infinity()) = "
     <<  ap::condition(e[0],e[1],e[2],typename ap::Norm_infinity()) << "\n";

  POINT a[3];
  for(int i = 0; i < 3; ++i) {
    a[i] = pt::Origin(3);
    a[i][li+i] = i+1;
  }


  out << "ap::inverse(a[0],a[1],a[2]) = ";
  ap::invert(a[0],a[1],a[2],inv[0],inv[1],inv[2]);
  out << inv[0] << "  " << inv[1] << "  " << inv[2] << '\n';
  out << "ap::inverse(a[0],a[1],a[2]) = ";
  ap::invert(a[0],a[2],a[1],inv[0],inv[1],inv[2]);
  out << inv[0] << "  " << inv[1] << "  " << inv[2] << '\n';

  out << "ap::matrixnorm_frobenius(a[0],a[1],a[2]) = "
      << ap::matrixnorm_frobenius(a[0],a[1],a[2]) << '\n'
      << "ap::matrixnorm_1(a[0],a[1],a[2]) = "
      << ap::matrixnorm_1(a[0],a[1],a[2]) << '\n'
      << "ap::matrixnorm_infinity(a[0],a[1],a[2]) = "
      << ap::matrixnorm_infinity(a[0],a[1],a[2]) << '\n';

  out<< "ap::condition(a[0],a[1],a[2],ap::Norm_frobenius()) = "
     <<  ap::condition(a[0],a[1],a[2],typename ap::Norm_frobenius()) << "\n";
  out<< "ap::condition(a[0],a[1],a[2],ap::Norm_1()) = "
     <<  ap::condition(a[0],a[1],a[2],typename ap::Norm_1()) << "\n";
  out<< "ap::condition(a[0],a[1],a[2],ap::Norm_infinity()) = "
     <<  ap::condition(a[0],a[1],a[2],typename ap::Norm_infinity()) << "\n";



  // matrix type
  typename ap::matrix_type A(0);
  typename ap::matrix_type B(ap::matrix_type::UnitMatrix());
  typename ap::matrix_type C = A*B;
  out << "A= " << A[li+0] << "  " << A[li+1] << "  " << A[li+2] << "\n"
      << "B= " << B[li+0] << "  " << B[li+1] << "  " << B[li+2] << "\n"
      << "C= " << C[li+0] << "  " << C[li+1] << "  " << C[li+2] << "\n";
  A(li+0,li+0) = 1;
  A(li+1,li+1) = 2;
  A(li+2,li+2) = 3;
  out << "A= " << A[li+0] << "  " << A[li+1] << "  " << A[li+2] << "\n";
  C = A*B;
  out << "C= " << C[li+0] << "  " << C[li+1] << "  " << C[li+2] << "\n";
  typename ap::matrix_type D(0);
  D(li+0, li+1) = 1;
  D(li+1, li+2) = 1;
  out << "D= " << D[li+0] << "  " << D[li+1] << "  " << D[li+2] << "\n";
  D *= D;
  out << "D= " << D[li+0] << "  " << D[li+1] << "  " << D[li+2] << "\n";

  out << "det(A)= " << ap::det(A) << "\n"
      << "|A|_1  = " << ap::matrixnorm_1(A) << "\n"
      << "|A|_oo = " << ap::matrixnorm_infinity(A) << "\n"
      << "|A|_F  = " << ap::matrixnorm_frobenius(A) << "\n";


  typedef segment<POINT>   segment3;
  typedef ray<POINT>       ray3;
  typedef triangle<POINT>  triangle3;
  typedef intersection_segment_triangle<segment3, triangle3> intersection_t;
  typedef intersection_ray_triangle<ray3, triangle3>         intersection_rt;

  //------- intersection test #1 -----------

  // S = [ (0.25,0.25,-1), (0.25,0.25, 1)] 
  // T = [ (0,0,0), (1,0,0), (0,1,1)]
  // intersection (0.25,0.25,0)

  POINT p[2], q[3];
  for(int i = 0; i < 2; ++i) {
    p[i] = pt::Origin(3);
  }  
  for(int i = 0; i < 3; ++i) {
    q[i] = pt::Origin(3);
  }  

  pt::z(p[1]) = 1;  
  pt::z(p[0]) = -1;
  pt::x(p[0]) = pt::x(p[1]) = pt::y(p[0]) = pt::y(p[1]) = 0.25;

  pt::x(q[1]) = 1;
  pt::y(q[2]) = 1;

  check_intersections(p[0],p[1],
		      q[0],q[1],q[2], out);


  //------- intersection test #2 -----------

  // S = [ (0.25,0.25, 0.5), (0.25,0.25, 1)] 
  // T = [ (0,0,0), (1,0,0), (0,1,1)]
  // intersection does not exist

  // segment: uses const& of POINT
  pt::z(p[0]) = 0.5; 
  check_intersections(p[0],p[1],
		      q[0],q[1],q[2], out);



  //------- intersection test #3 -----------
  // S = [(0.5,0.5,0), (0.5,0.5,1)]
  // T = [(-0.5,-0.5,0), (-0.5, 0.5,1), (-0.5,-0.5,1)
  // intersection does not exist


  pt::x(p[0]) = pt::x(p[1]) = pt::y(p[0]) = pt::y(p[1]) =  0.5;
  pt::z(p[0]) = 0; pt::z(p[1]) = 1;
  pt::x(q[0]) = pt::x(q[1]) = pt::x(q[2]) = -0.5;
  pt::y(q[1]) = 0.5;
  pt::y(q[0]) = pt::y(q[2]) = -0.5;
  pt::z(q[0]) = 0;
  pt::z(q[1]) = pt::z(q[2]) = 1;

  check_intersections(p[0],p[1],
		      q[0],q[1],q[2], out);

  //------- intersection test #4 -----------
  // segmen/ray parallel to triangle

  {
    POINT p0(1,1,2), p1(1,0,2);
    POINT q0(0,0,0), q1(1,0,0), q2(0,1,0);
    check_intersections(p0,p1,
			q0,q1,q2,out);
  }
  //------- intersection test #5 -----------
  // intersection on edge of triangle
  {
    POINT p0(.5,.5,-1), p1(.5,.5,1);
    POINT q0(0,0,0), q1(1,0,0), q2(0,1,0);
    check_intersections(p0,p1,
			q0,q1,q2,out);
  }



  std::vector<POINT> d1(e, e+1);
  std::vector<POINT> d2(e, e+2);
  std::vector<POINT> d3(1, e[0]+e[1]);  
  std::vector<POINT> d4(2); d4[0]= e[0]+e[1]; d4[1] = e[1]+e[2];

  std::vector<POINT> c1 = ap::basis_completion(d1);
  std::vector<POINT> c2 = ap::basis_completion(d2);
  std::vector<POINT> c3 = ap::basis_completion(d3);
  std::vector<POINT> c4 = ap::basis_completion(d4);

  out << "ap::basis_completion(" << d1[0] << ") = " << c1[0] << "; " << c1[1] << std::endl;
  out << "ap::basis_completion(" << d2[0] << "; " << d2[1] << ") = " << c2[0] << std::endl;
  out << "ap::basis_completion(" << d3[0] << ") = " << c3[0] << "; " << c3[1] << std::endl;
  out << "ap::basis_completion(" << d4[0] << "; " << d4[1] << ") = " << c4[0] << std::endl;

  
}

} // namespace GrAL 

#endif
