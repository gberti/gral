#ifndef NMWR_GB_ALGEBRAIC_PRIMITIVES
#define NMWR_GB_ALGEBRAIC_PRIMITIVES


//----------------------------------------------------------------
//   (c) Guntram Berti, 1997
//   Chair for Numerical Mathematics & Scientific Computing (NMWR)
//   TU Cottbus - Germany
//   http://math-s.math.tu-cottbus.de/NMWR
//   
//----------------------------------------------------------------

#include <math.h>
#include "Utility/pre-post-conditions.h"
#include "Geometry/point-traits.h"

//----------------------------------------------------------------
//
//  this file contains some generic implementations for 
//  elementary algebraic & geometric operations,
//  such as determinants or area-calculations.
//
//  class structure:
//  
//                + - - - ddp 2d - - - +
//                |                    |
//                + - - - ddp 3d - - - +
//                |                    |
// ap ---- ddp -- + - - - - - - - - - -+--- bap
//
// where:
// ap  - algebraic_primitives:            user-interface class
// bap - basic_algebraic_primitives:      non-dimension-dependent functions
// ddp - dimension_dependent_primitives : bap + possible extensions for 2D/3D. 
//       This class may be specialized for concrete POINT types.
// ddp{2d,3d} - dimension_dependent_primitives_{2d,3d} : special functions
//       for 2D or 3D. 
//       This can be used to specialize ddp, e.g.:
//       dimension_dependent_primitives<mypoint> : 
//         public dimension_dependent_primitives_2d<mypoint> {};
//
//----------------------------------------------------------------




template<class POINT>
struct basic_algebraic_primitives : public point_traits<POINT> {

  typedef typename point_traits<POINT>::component_type scalar;

  static scalar sqr(scalar x) { return (x*x);}

  static scalar dot(const POINT& p, const POINT& q)
    {
      int d = Dim(p);
      int lp = LowerIndex(p);
      int lq = LowerIndex(q); 
      scalar sum = 0.0;
      for(int i = 0; i< d; i++)
	sum += p[lp+i]*q[lq+i];
      
      return sum;
    }

  static scalar distance2(const POINT& p1, const POINT& p2)
    {
      scalar sum = 0;
      for(int i = LowerIndex(p1); i<= UpperIndex(p1); i++)
	sum += sqr(p1[i]-p2[i]);
      return sum;
    }

  static scalar  distance(const POINT& p1, const POINT& p2)
    { return sqrt(distance2(p1,p2));}

  static scalar norm2(const POINT& p)
    {
      scalar sum = 0;
      typedef point_traits<POINT> traits;
      for(int i = traits::LowerIndex(p); i<= traits::UpperIndex(p); i++)
	sum += sqr(p[i]);
      return sum;
    }

  static scalar norm(const POINT& p) { return sqrt(norm2(p));}
  static POINT normalization(const POINT& p) { return(p/norm(p));}
  // better: ?
  // static void normalize(POINT& p) { p *= 1.0/norm(p);}

  // calculate the angle a in radians between p and q; so that 
  // a rotation about a with axis p x q will map p to q.
  static  scalar cos_of_angle(const POINT& p, const POINT& q)
    {return (dot(p,q)/(norm(p)*norm(q)));}

  static  scalar angle(const POINT& p, const POINT& q)
    {return acos(cos_of_angle(p,q));}

};



  ////////////////////////////////////////////////////////////////
  //
  // dimension dependent functions 
  //
  ////////////////////////////////////////////////////////////////



/*
template<class POINT, class dim_tag>
struct algebraic_primitives_for_dim   
 : public basic_algebraic_primitives<POINT>  
{};

template<class POINT>
struct dimension_dependent_primitives 
  : public algebraic_primitives_for_dim<POINT,point_traits<POINT>::dimension_tag> 
{};

// partial specializations for tag2D and tag3D:

template<class POINT, tag2D>
struct algebraic_primitives_for_dim 
  : public dimension_dependent_primitives_2d {};

template<class POINT, tag3D>
struct algebraic_primitives_for_dim 
  : public dimension_dependent_primitives_3d {};
  */


template<class POINT>
struct dimension_dependent_primitives 
  : public basic_algebraic_primitives<POINT>  {};



template<class POINT>
struct algebraic_primitives : public dimension_dependent_primitives<POINT> {};

#include "Geometry/primitives2d.h"
#include "Geometry/primitives3d.h"


//-------------------------------------------------------------------------
//
// calculate centroid and area of a simple polygon, given by the vertices
// [begin, end).
// The centroid (or center of inertia) c of a domain D is defined by
// \int_{D} x_i dx dy / area(D)
//
// This is adapted from GraphicsGems IV, centroid.c
//
//-------------------------------------------------------------------------

template<class PIt, class Q>
void get_polygon2d_center_and_area(PIt begin, PIt end, // in:  iterator over polygon vertices
				   Q& center,          // out: centroid
				   double& area);      // out: polygon area

#ifdef NMWR_INCLUDE_TEMPLATE_DEFS
#include "Geometry/algebraic-primitives.C"
#endif

#endif
