#ifndef NMWR_GB_ALGEBRAIC_PRIMITIVES
#define NMWR_GB_ALGEBRAIC_PRIMITIVES



// $LICENSE

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

  typedef point_traits<POINT> pt;
  typedef typename pt::component_type scalar;
  typedef scalar   real; // should use scalar_traits<scalar>::real
  // anayway, some of the below are not correct for scalar = complex.

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

  static scalar squared_norm_2(const POINT& p)
    {
      scalar sum = 0;
      typedef point_traits<POINT> traits;
      for(int i = traits::LowerIndex(p); i<= traits::UpperIndex(p); i++)
	sum += sqr(p[i]);
      return sum;
    }

  static real norm_2(const POINT& p) { return sqrt(squared_norm_2(p));}
  static POINT normalization(const POINT& p) { return(p/norm_2(p));}

  static real norm_1(POINT const& p) {
    real sum = 0;
    for(int i = pt::LowerIndex(p); i <= pt::UpperIndex(p); ++i)
      sum += fabs(p[i]);
    return sum;
  }

  static real norm_infinity(POINT const& p) {
   real max_comp = 0;
   for(int i = pt::LowerIndex(p); i < pt::UpperIndex(p); ++i)
      max_comp = ( max_comp < fabs(p[i]) ? fabs(p[i]): max_comp);
   return max_comp;
  }
  // better: ?
  // static void normalize(POINT& p) { p *= 1.0/norm(p);}

  static real rel_diff(POINT const& p1, POINT const& p2)
    {
     POINT p(p1-p2);
     real result = norm_2(p);
     if(result != 0.0) {
       result /= std::max(norm_2(p1), norm_2(p2));
     }
     return result;
    }

  /*! \brief calculate the cosine of the angle \f$\alpha\f$ in radians between p and q

     The result is oriented such that a rotation about \f$\arccos \alpha\f$ 
     with axis p x q will map p to q.
  */
  static  scalar cos_of_angle(const POINT& p, const POINT& q)
    {return (dot(p,q)/(norm_2(p)*norm_2(q)));}

  /*! \brief calculate the angle \f$\alpha\f$ in radians between p and q

     The result is oriented such that a rotation about \f$ \alpha\f$ 
     with axis p x q will map p to q.
  */
  static  scalar angle(const POINT& p, const POINT& q)
    {return acos(cos_of_angle(p,q));}

};



////////////////////////////////////////////////////////////////
//
// dimension dependent functions 
//
////////////////////////////////////////////////////////////////



#include "Geometry/primitives2d.h"
#include "Geometry/primitives3d.h"


template<class POINT, class DIM_TAG = tag_unknown_dim>
struct dimension_dependent_primitives 
  : public basic_algebraic_primitives<POINT>  {};

template<class POINT>
struct dimension_dependent_primitives<POINT, tag2D>
  : public dimension_dependent_primitives_2d<POINT>  {};

template<class POINT>
struct dimension_dependent_primitives<POINT, tag3D>
  : public  dimension_dependent_primitives_3d<POINT>  {};

template<class POINT>
struct dimension_dependent_primitives<POINT, variable_dimension_tag>
  : public dimension_dependent_primitives_2d<POINT>,
    public dimension_dependent_primitives_3d<POINT>
  {};

template<class POINT, class PT = point_traits<POINT> >
struct algebraic_primitives 
  : public dimension_dependent_primitives
    <
     POINT,
     typename  PT::dimension_tag
    > 
{};



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
