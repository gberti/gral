#ifndef NMWR_GB_ALGEBRAIC_PRIMITIVES_2D_H
#define NMWR_GB_ALGEBRAIC_PRIMITIVES_2D_H


// $LICENSE

//----------------------------------------------------------------
//
// specialize dimension_dependent_primitives<POINT>
// to dimension_dependent_primitives_2d<POINT> for 2D points.
//
//----------------------------------------------------------------

#include <algorithm> // swap
#include <vector>

#include "Geometry/basic-algebraic-primitives.h"

namespace GrAL {

/*! \brief geometric functions specific to 2D

    \ingroup algebraicprimitives   

   This class can be used to enforce 2D primitives 
   for point classes with runtime variable dimension:
   \code
    template<>
    struct dimension_dependent_primitives<MyPoint> 
      : public dimension_dependent_primitives_2d<MyPoint> {};
   \endcode

  Then \c algebraic_primitives<MyPoint> automatically contains 2D primitives.
*/
template<class POINT>
struct dimension_dependent_primitives_2d 
  : public virtual basic_algebraic_primitives<POINT> 
{
private:
  typedef basic_algebraic_primitives<POINT> base;
public:
  typedef typename base::scalar scalar;
  typedef typename base::real   real;
  typedef point_traits<POINT>   pt;


  struct matrix_type {
    typedef point_traits<POINT>         pt;
    typedef dimension_dependent_primitives_2d<POINT> ap;
    typedef typename ap::scalar         scalar;
    
    POINT a[2]; // columns
   
    matrix_type() { init();}
    matrix_type(POINT const& a0, POINT const& a1)
    { 
      a[0] = a0; a[1] = a1;
    }      
    explicit matrix_type(scalar s) 
    { 
      init();
      for(int i = 0; i < 2; ++i)
	for(int j = pt::LowerIndex(a[i]); j <= pt::UpperIndex(a[i]); ++j)
	  a[i][j] = s;
    }
    void init() 
    {
      for(int i = 0; i < 2; ++i)
	pt::ConstructWithDim(2, a[i]);
    }

    POINT const& operator[](int i) const { cv(i); return a[i-offset()];}
    POINT      & operator[](int i)       { cv(i); return a[i-offset()];}
    scalar   operator()(int i, int j) const { cv(i); cv(j); return a[i-offset()][j];}
    scalar & operator()(int i, int j)       { cv(i); cv(j); return a[i-offset()][j];}

    int LowerIndex()  const { return pt::LowerIndex(a[0]);}
    int UpperIndex()  const { return pt::UpperIndex(a[0]);}

    static matrix_type UnitMatrix() {
      matrix_type res(0);
      for(int i = res.LowerIndex(); i <= res.UpperIndex(); ++i)
	res(i,i) = 1;
      return res;
    }

    matrix_type operator *(matrix_type const& rhs) const {
     matrix_type res;
     for(int i = 0; i < 2; ++i)
       ap::matrix_vector_product(a[0],a[1], rhs.a[i], res.a[i]);
     return res;
    }
    matrix_type & operator *=(matrix_type const& rhs) {
      *this = *this * rhs;
      return *this;
    }
    POINT operator* (POINT const& b) { POINT res; ap::matrix_vector_product(a[0],a[1],b,res); return res;}

    int offset() const { return pt::LowerIndex(a[0]);}
    void cv(int i) const { REQUIRE( pt::LowerIndex(a[0]) <= i && i <= pt::UpperIndex(a[0]), "i= " << i, 1);}
  }; // struct matrix_type



  static double det2(const POINT& p1, const POINT& p2)
    {
      int l = point_traits<POINT>::LowerIndex(p1);
      // IOMgr::Info() << "lower index: " << l << endl;
      // IOMgr::Info() << p1 << " " << p2 << endl;
      return (p1[l]*p2[l+1]-p1[l+1]*p2[l]);
    }
  /*! \brief returns the determinant of the matrix A
   */
  static scalar det(matrix_type const& A) { return det2(A.a[0], A.a[1]);}

  static double signed_triangle_area(const POINT& p1, const POINT& p2, const POINT& p3)
    { 
      // IOMgr::Info() << "signed triangle area: " 
      //		    << p1 << " " << p2 << " " << p3 << endl;
      return (0.5 * det2(p1-p2,p1-p3)); }

  static double triangle_area(const POINT& p1, const POINT& p2, const POINT& p3)
    {
      double a(signed_triangle_area(p1,p2,p3));
      return(a >= 0 ? a : -a);
    }

  static POINT normal_with_same_length(const POINT& p)
    {
      int l = LowerIndex(p);
      return POINT(-p[l+1],p[l]);
    }

  static POINT normed_normal(const POINT& p)
    {
      POINT q(normal_with_same_length(p));
      return (q/norm_2(q));
    }


  static void transpose(POINT & p, POINT & q) {
    ::std::swap(y(p),x(q));
  }

  static void transpose(matrix_type & A) {
    transpose(A.a[0], A.a[1]);
  }

  static POINT left_perp(POINT const& p) {
    return POINT(-y(p), x(p));
  }
  static void left_rotate(POINT & p) {
    scalar xp = x(p);
    x(p) = y(p);
    y(p) = -xp;
  }
  static POINT right_perp(POINT const& p) {
    return POINT(y(p), -x(p));
  }
  static void right_rotate(POINT & p) {
    scalar xp = x(p);
    x(p) = -y(p);
    y(p) =  xp;
  }
  static POINT any_perp(POINT const& p) {
    return POINT(-y(p), x(p));
  }
 
    /*! \brief Return normal direction of (1-dimensional) hyperspace spanned by [begin,end[.

      Dimension-independent interface.
   */
  template<class IT>
  static POINT perp(IT begin, IT end) { return normal_with_same_length(*begin);}

  /*! \brief Calculate matrix-vector product
       \f$ c =  A*b  \f$
  */
  static void matrix_vector_product(POINT const& a1, POINT const& a2,
				    POINT const& b, 
				    POINT      & c)
  {
    c = pt::x(b)*a1 + pt::y(b) * a2;
  }
  /*! \brief Calculate matrix product
       \f$ C =  A*B  \f$
  */
  static void matrix_product(POINT const& a1, POINT const& a2, 
			     POINT const& b1, POINT const& b2, 
			     POINT      & c1, POINT      & c2)
  {
    matrix_vector_product(a1,a2, b1, c1);
    matrix_vector_product(a1,a2, b2, c2);
  }


  static ::std::vector<POINT> basis_completion(::std::vector<POINT> const& dirs) {
    ::std::vector<POINT> res;
    if(dirs.size() == 1) {
      res.resize(1);
      res[0] = perp(dirs.begin(), dirs.end());
    }
    else if(dirs.size() == 0) {
      res.resize(2);
      res[0] = pt::Origin(2); pt::x(res[0]) = 1;
      res[1] = pt::Origin(2); pt::y(res[1]) = 1;
    }
    return res;
  }


  /*! \brief Solve \f$ (A_1,A_2) x = b \f$ for \f$ x \f$
      The implementation uses Cramer's rule. 
  */
  static void solve2(POINT const& A1, POINT const& A2,
                     POINT      & X,  POINT const& b)
  {
    scalar d = det2(A1,A2);
    x(X) = ( y(A2) * x(b) - x(A2) * y(b))/d;
    y(X) = (-y(A1) * x(b) + x(A1) * y(b))/d;
  }

  /*! \brief Solve \f$ A x = b \f$ for \f$ x \f$
      The implementation uses Cramer's rule. 
  */
  static void solve(matrix_type const& A, 
		    POINT            & x,  POINT const& b)
  { solve2(A.a[0], A.a[1], x, b);}

  /*! \brief Solve \f$ A x = b \f$ for \f$ x \f$
      The implementation uses Cramer's rule. 
  */
  static POINT solution(matrix_type const& A, 
			POINT const& b)
  { 
    POINT x; 
    pt::ConstructWithDim(pt::Dimension(b), x);
    solve2(A.a[0], A.a[1], x, b);
    return x;
  }


  /*! \brief Solve \f$ (A_1,A_2) x = b \f$ for \f$ x \f$
      The implementation uses Gauss elimination with pivoting
  */
  static void solveA(POINT  A1, POINT  A2,
                     POINT& X , POINT  b)
  {
    if(fabs(x(A1)) < fabs(y(A1))) {
      ::std::swap(x(A1),y(A1));
      ::std::swap(x(A2),y(A2));
      ::std::swap(x(b) ,y(b));
    }
    // Lu = b, l_21 = a21/a11 = y(A1)/x(A1)
    scalar u1 = x(b);
    scalar u2 = y(b)-(y(A1)/x(A1))*u1;
    // Rx = u, r11 = x(A1), r12 = x(A2), r22 = y(A2) - y(A1)x(A1)^-1x(A2)
    y(X) = u2/(y(A2) - (y(A1)/x(A1))*x(A2));
    x(X) = (u1 - x(A2)*y(X))/x(A1);
  }

  /*! \brief Calulate inverse \f$ (A_1,A_2)^{-1}\f$
   */
  static void invert(POINT const& r1, POINT const& r2,
		      POINT      & i1, POINT      & i2)
  {
    scalar d = det2(r1,r2);
    REQUIRE_ALWAYS( (d != 0.0), "Matrix singular!\n",1);
    pt::x(i1) =  pt::y(r2) / d;
    pt::y(i2) =  pt::x(r1) / d;
    pt::y(i1) = -pt::y(r1) / d;
    pt::x(i2) = -pt::x(r2) / d;
  }

  /*! \brief Calculate inverse of \c A by Cramer's rule

  */
  static matrix_type inverse(matrix_type const& A)
  {
    matrix_type res;
    invert(A.a[0], A.a[1],  res.a[0], res.a[1]);
    return res;
  }


};

} // namespace GrAL 


#endif
