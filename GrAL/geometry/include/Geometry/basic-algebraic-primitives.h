#ifndef GRAL_GB_GEOMETRY_BASIC_ALGEBRAIC_PRIMITIVES
#define GRAL_GB_GEOMETRY_BASIC_ALGEBRAIC_PRIMITIVES

// $LICENSE

#include "Utility/pre-post-conditions.h"
#include "Geometry/point-traits.h"

#include <math.h>
#include <vector>


namespace GrAL {



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
  static void  normalize(POINT & p) { p = normalization(p);}

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
       result /= ::std::max(norm_2(p1), norm_2(p2));
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

  /*! \brief Complete the vector system \c dirs with linearly independent vectors

      If \c dirs contains \em k linearly independent vectors, the union of \c dirs and the result
      \c basis_completion(dirs) will form a basis of \f$ \R^n \f$. 


  static ::std::vector<POINT> basis_completion(::std::vector<POINT> const& dirs) {
    unsigned n = pt::Dim(dirs[0]);
    ::std::vector<POINT> res(n-dirs.size(), pt::Origin(n));
    unsigned ld = pt::LowerIndex(dirs[0]);
    unsigned k = 0;
    unsigned ej = 0;
    while(k < n-dirs.size()) {
      // find the next unit vector which makes an angle of more than (1+\eps) Pi/4
      // with all dirs[i]. There must be at least n-dirs.size() of these.
      scalar max_cos_angle = 0;
      do {
	// check unit vector ej
	res[k][ld+ej] = 1;
	max_cos_angle = 0;
	for(unsigned i = 0; i < dirs.size(); ++i)
	  max_cos_angle = ::std::max(max_cos_angle, fabs(dot(res[k], dirs[i])/norm_2(dirs[i]) ));
	res[k][ld+ej] = 0;
	++ej;
      } while(max_cos_angle > 1.01/sqrt(2.0) && ej < n);
      ENSURE(max_cos_angle <=  1.01/sqrt(2.0), 
	     "did not find independent vector! max_cos_angle=" << max_cos_angle,1);  
      res[k][ld+ej-1] = 1;
      ++k;
    }
    return res;
  }
  */
};

// to be specialized for dim = 2/3
template<class POINT, class DIM_TAG = tag_unknown_dim>
struct dimension_dependent_primitives 
  : public basic_algebraic_primitives<POINT>  {};


} // namespace GrAL


#endif