#ifndef NMWR_GB_ALGEBRAIC_PRIMITIVES_3D_H
#define NMWR_GB_ALGEBRAIC_PRIMITIVES_3D_H

//----------------------------------------------------------------
//   (c) Guntram Berti, 1997
//   Chair for Numerical Mathematics & Scientific Computing (NMWR)
//   TU Cottbus - Germany
//   http://math-s.math.tu-cottbus.de/NMWR
//   
//----------------------------------------------------------------

//----------------------------------------------------------------
//
// template<class POINT>
// struct dimension_dependent_primitives_3d;
// geometric functions specific to 3D:
//  * det3(p1,p2,p3)
//  * vector_product(p1,p2)
//  * triangle_area(p1,p2)
// 
// This can be used to specialize dimension_dependent_primitives<POINT>
// to dimension_dependent_primitives_3d<POINT> for 3D points.
//
//----------------------------------------------------------------


template<class POINT>
struct dimension_dependent_primitives_3d 
  : public basic_algebraic_primitives<POINT> {

  static double det3(const POINT& p1, const POINT& p2, const POINT& p3)
    {
      int l = LowerIndex(p1);
      return(
	     p1[l]*(p2[l+1]*p3[l+2] - p2[l+2]*p3[l+1])
	     -p1[l+1]*(p2[l]*p3[l+2] - p2[l+2]*p3[l])
	     +p1[l+2]*(p2[l]*p3[l+1] - p2[l+1]*p3[l])
	     );
    }

  static double triangle_area(const POINT& p1, const POINT& p2, const POINT& p3)
    { return 0.5*norm(vectorproduct(p1-p2,p1-p3));}

  static POINT vectorproduct(const POINT& p, const POINT& q)   // p x q 
    { 
      int lp = LowerIndex(p)-1;
      int lq = LowerIndex(q)-1; 
      return (POINT(p[lp+2]*q[lq+3]-p[lp+3]*q[lq+2], 
		    p[lp+3]*q[lq+1]-p[lp+1]*q[lq+3], 
		    p[lp+1]*q[lq+2]-p[lp+2]*q[lq+1]));
    }

};

#endif
