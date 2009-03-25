#ifndef NMWR_GB_ALGEBRAIC_PRIMITIVES_3D_H
#define NMWR_GB_ALGEBRAIC_PRIMITIVES_3D_H



/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti, 
    Contact: guntram AT users DOT berlios DOT de

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


namespace GrAL {

/*! \file 

*/

/*! \brief geometric functions specific to 3D

    \ingroup algebraicprimitives   

   This class can be used to enforce 3D primitives 
   for point classes with runtime variable dimension:
   \code
    template<>
    struct dimension_dependent_primitives<MyPoint> 
      : public dimension_dependent_primitives_3d<MyPoint> {};
   \endcode

  Then \c algebraic_primitives<MyPoint> automatically contains 3D primitives.
*/



template<class POINT>
struct dimension_dependent_primitives_3d 
  : public virtual basic_algebraic_primitives<POINT> {
private:
  typedef basic_algebraic_primitives<POINT> base;
  typedef typename base::pt                 pt;
public:
  typedef typename base::scalar scalar;
  typedef typename base::real   real;

  struct matrix_type {
    typedef point_traits<POINT>         pt;
    typedef dimension_dependent_primitives_3d<POINT> ap;
    typedef typename ap::scalar         scalar;
    
    POINT a[3];
   
    matrix_type() { init();}
    matrix_type(POINT const& a0, POINT const& a1, POINT const& a2)
    { 
      a[0] = a0; a[1] = a1; a[2] = a2;
    }      
    explicit matrix_type(scalar s) 
    { 
      init();
      for(int i = 0; i < 3; ++i)
	for(int j = pt::LowerIndex(a[i]); j <= pt::UpperIndex(a[i]); ++j)
	  a[i][j] = s;
    }
    void init() 
    {
      for(int i = 0; i < 3; ++i)
	pt::ConstructWithDim(3, a[i]);
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
     for(int i = 0; i < 3; ++i)
       ap::matrix_vector_product(a[0],a[1],a[2], rhs.a[i], res.a[i]);
     return res;
    }
    matrix_type & operator *=(matrix_type const& rhs) {
      *this = *this * rhs;
      return *this;
    }
    POINT operator* (POINT const& b) { POINT res; ap::matrix_vector_product(a[0],a[1],a[2],b,res); return res;}

    int offset() const { return pt::LowerIndex(a[0]);}
    void cv(int i) const { REQUIRE( pt::LowerIndex(a[0]) <= i && i <= pt::UpperIndex(a[0]), "i= " << i, 1);}
  }; // struct matrix_type


  /*! \brief returns the determinant of the matrix (p1,p2,p3)
   */
  static scalar det3(const POINT& p1, const POINT& p2, const POINT& p3)
    {
      int l = pt::LowerIndex(p1);
      return(
	     p1[l]*(p2[l+1]*p3[l+2] - p2[l+2]*p3[l+1])
	     -p1[l+1]*(p2[l]*p3[l+2] - p2[l+2]*p3[l])
	     +p1[l+2]*(p2[l]*p3[l+1] - p2[l+1]*p3[l])
	     );
    }
  /*! \brief returns the determinant of the matrix A
   */
  static scalar det(matrix_type const& A) { return det3(A.a[0], A.a[1], A.a[2]);}

  /*! \brief returns the area of the triangle with corners pi in \f$ \R^3 \f$
   */
  static scalar triangle_area(const POINT& p1, const POINT& p2, const POINT& p3)
    { return 0.5*norm_2(vectorproduct(p1-p2,p1-p3));}

  /*! \brief returns \f$ p \times q \f$
   */
  static POINT vectorproduct(const POINT& p, const POINT& q)   // p x q 
    { 
      int lp = LowerIndex(p)-1;
      int lq = LowerIndex(q)-1; 
      return (construct(p[lp+2]*q[lq+3]-p[lp+3]*q[lq+2], 
			p[lp+3]*q[lq+1]-p[lp+1]*q[lq+3], 
			p[lp+1]*q[lq+2]-p[lp+2]*q[lq+1]));
    }

  /*! \brief Construct a 3D point from 3 scalars

      \note this could go into point_traits<POINT>
  */
  static POINT construct(scalar xx, scalar yy, scalar zz) 
  {
    POINT res;
    pt::ConstructWithDim(3, res);
    x(res) = xx;
    y(res) = yy;
    z(res) = zz;
    return res;
  }

  /*! \brief Return a vector perpendicular to \c p 
   */
  static POINT any_perp(POINT const& p) {
    POINT e1(pt::Origin(3)); pt::x(e1) = 1;
    POINT e2(pt::Origin(3)); pt::y(e2) = 1;
    // at least one of e1, e2 is independent of p, the one with smaller scalar product 
    // is more orthogonal.
    return( ( fabs(dot(e1,p)) < fabs(dot(e2,p))) 
	    ? vectorproduct(e1,p) 
	    : vectorproduct(e2,p));
    /*
    int max_i = LowerIndex(p);
    if(abs(pt::y(p)) > abs(p[max_i])) max_i = 1+LowerIndex(p);
    if(abs(pt::z(p)) > abs(p[max_i])) max_i = 2+LowerIndex(p);
    int next_i = (max_i < UpperIndex(p) ? max_i+1  : LowerIndex(p));
    int zero_i = (next_i< UpperIndex(p) ? next_i+1 : LowerIndex(p));
    res[max_i ] = -p[next_i];
    res[next_i] =  p[max_i];
    res[zero_i] =  0;
    return res;
    */

  }


  static ::std::vector<POINT> basis_completion(::std::vector<POINT> const& dirs) {
    ::std::vector<POINT> res;
    if(dirs.size() == 1) {
      res.resize(2);
      res[0] = any_perp(dirs[0]);
      res[1] = vectorproduct(res[0],dirs[0]);
    }
    else if(dirs.size() == 2) {
      res.resize(1);
      res[0] = perp(dirs.begin(), dirs.end());
    }
   else if(dirs.size() == 0) {
     res.resize(3);
     res[0] = pt::Origin(3); pt::x(res[0]) = 1;
     res[1] = pt::Origin(3); pt::y(res[1]) = 1;
     res[2] = pt::Origin(3); pt::z(res[2]) = 1;
   }
   return res;
  }

  /*! \brief Return normal direction of (2-dimensional) hyperspace spanned by [begin,end[.

      Dimension-independent interface.
   */
  template<class IT>

  static POINT perp(IT begin, IT end) { IT p2 = begin; ++p2; return vectorproduct(*begin, *p2);}

  /*! \brief Return normal direction of (2-dimensional) hyperspace spanned by \c dirs

      Dimension-independent interface.
   */
  template<class SEQ>
  static POINT perp(SEQ const& dirs) { 
    typename SEQ::const_iterator p1 = dirs.begin(); 
    typename SEQ::const_iterator p2 = p1; ++p2; 
    return vectorproduct(*p1, *p2);
  }


  /*! \brief Calculate matrix-vector product
       \f$ c =  A*b  \f$
  */
  static void matrix_vector_product(POINT const& a1, POINT const& a2, POINT const& a3,
				    POINT const& b, 
				    POINT      & c)
  {
    c = pt::x(b)*a1 + pt::y(b) * a2 + pt::z(b) * a3;
  }

  /*! \brief Calculate matrix product
       \f$ C =  A*B  \f$
  */
  static void matrix_product(POINT const& a1, POINT const& a2, POINT const& a3,
			     POINT const& b1, POINT const& b2, POINT const& b3,
			     POINT      & c1, POINT      & c2, POINT      & c3)
  {
    matrix_vector_product(a1,a2,a3, b1, c1);
    matrix_vector_product(a1,a2,a3, b2, c2);
    matrix_vector_product(a1,a2,a3, b3, c3);
  }


  /*! \brief Calculate inverse by Cramer's rule
    
     The inverse I = (i1,i2,i3) of the matrix (r1,r2,r3) 
     with the rows ri is calculated using Cramer's rule.
   */
  static void invert(POINT const& r1, POINT const& r2, POINT const& r3,
		     POINT      & i1, POINT      & i2, POINT      & i3)
    {
      // inversion by Cramer's rule 
      scalar d = det3(r1,r2,r3);
      REQUIRE_ALWAYS( (d != 0.0), "Matrix singular!\n",1);
      scalar d_inv = 1.0/d;
      int li = pt::LowerIndex(r1);
      POINT e[3]; // unit vectors
      for(int i = 0; i < 3; ++i) {
	e[i] = pt::Origin(3);
	e[i][i+ li] = 1;
      }
      i1[li+0] = d_inv * det3(e[0],r2,r3); 
      i2[li+0] = d_inv * det3(e[1],r2,r3); 
      i3[li+0] = d_inv * det3(e[2],r2,r3); 

      i1[li+1] = d_inv * det3(r1,e[0],r3); 
      i2[li+1] = d_inv * det3(r1,e[1],r3); 
      i3[li+1] = d_inv * det3(r1,e[2],r3); 

      i1[li+2] = d_inv * det3(r1,r2,e[0]); 
      i2[li+2] = d_inv * det3(r1,r2,e[1]); 
      i3[li+2] = d_inv * det3(r1,r2,e[2]); 
    }
  /*! \brief Calculate inverse of \c A by Cramer's rule

  */
  static matrix_type inverse(matrix_type const& A)
  {
    matrix_type res;
    invert(A.a[0], A.a[1], A.a[2],  res.a[0], res.a[1], res.a[2]);
    return res;
  }

  /*! \brief Solve \f$ (A1,A2,A3)x = b \f$ for \f$ x \f$

      The implementation uses Cramer's rule. 
   */
  static void solve3(POINT const& A1, POINT const& A2, POINT const& A3,
		     POINT      & x,  POINT const& b)
    {
      // solve by Cramer's rule
      scalar d = det3(A1,A2,A3);
      REQUIRE_ALWAYS( (d != scalar(0)), "Matrix singular!\n",1);
      scalar d_inv = scalar(1)/d;
      int li = pt::LowerIndex(A1);
      x[li+0] = d_inv * det3(b, A2,A3);
      x[li+1] = d_inv * det3(A1,b ,A3);
      x[li+2] = d_inv * det3(A1,A2,b );
    }
  /*! \brief Solve \f$ A x = b \f$ for \f$ x \f$
      The implementation uses Cramer's rule. 
  */
  static void solve(matrix_type const& A, 
		    POINT            & x,  POINT const& b)
  { solve3(A.a[0], A.a[1], A.a[2], x, b);}

  /*! \brief Solve \f$ A x = b \f$ for \f$ x \f$
      The implementation uses Cramer's rule. 
  */
  static POINT solution(matrix_type const& A, 
			POINT const& b)
  { 
    POINT x; 
    pt::ConstructWithDim(pt::Dimension(b), x);
    solve(A.a[0], A.a[1], A.a[2], x, b);
    return x;
  }

  /*!  \brief Calulate the infinity norm of \f$ A = (A_1,A_2,A_3) \f$

  */
  static real matrixnorm_infinity(POINT const& A1, POINT const& A2, POINT const& A3)
    {
      real max_sum = 0;
      for(int i = pt::LowerIndex(A1); i <= pt::UpperIndex(A1); ++i) {
	real sum = fabs(A1[i]) + fabs(A2[i]) + fabs(A3[i]);
	max_sum = (max_sum < sum ? sum : max_sum);
      }
      return max_sum;
    }
  /*!  \brief Calulate the infinity norm of \c A

  */
  static real matrixnorm_infinity(matrix_type const& A)
  { return matrixnorm_infinity(A.a[0], A.a[1], A.a[2]);}

  /*! \brief Calculate the 1-norm \f$ \|A\|_1 = \max_j \sum_i |a_{ij}| \f$

   */
  static real matrixnorm_1(POINT const& A1, POINT const& A2, POINT const& A3)
    {
      real max_sum = norm_1(A1);
      real sum     = norm_1(A2);
      max_sum = (max_sum < sum ? sum : max_sum);
      sum          = norm_1(A3);
      max_sum = (max_sum < sum ? sum : max_sum);
      return max_sum;
    }
  /*! \brief Calculate the 1-norm \f$ \|A\|_1 = \max_j \sum_i |a_{ij}| \f$

   */
  static real matrixnorm_1(matrix_type const& A)
  { return matrixnorm_1(A.a[0], A.a[1], A.a[2]);}


  // the 2-norm is the magnitude of the larges eigenvalue.
  // This is not yet implemented.

  /*! \brief Frobenius norm of \f$ A = (A_1,A_2,A_3) \f$
   */
  static real matrixnorm_frobenius(POINT const& A1, POINT const& A2, POINT const& A3)
    {
      return sqrt(  squared_norm_2(A1) 
		  + squared_norm_2(A2)
		  + squared_norm_2(A3));
    }
  /*! \brief Frobenius norm of \c A
   */
  static real matrixnorm_frobenius(matrix_type const& A)
  { return matrixnorm_frobenius(A.a[0], A.a[1], A.a[2]);}

  struct Norm_frobenius {
    real operator()(POINT const& A1, POINT const& A2, POINT const& A3) const
      { return matrixnorm_frobenius(A1,A2,A3);}
    real operator()(matrix_type const& A) const { return matrixnorm_frobenius(A);}
  };
  typedef Norm_frobenius Norm_F;

  struct Norm_1 {
    real operator()(POINT const& A1, POINT const& A2, POINT const& A3) const
      { return matrixnorm_1(A1,A2,A3);}
    real operator()(matrix_type const& A) const { return matrixnorm_1(A);}
  };
  typedef Norm_1 Norm_columnsum;

  struct Norm_infinity {
    real operator()(POINT const& A1, POINT const& A2, POINT const& A3) const
      { return matrixnorm_infinity(A1,A2,A3);}
    real operator()(matrix_type const& A) const { return matrixnorm_infinity(A);}
  };
  typedef Norm_infinity Norm_oo;
  typedef Norm_infinity Norm_rowsum;

  /*! \brief Compute condition number of matrix \f$ A = (A_1,A_2,A_3) \f$
       
      Condition number is computed with respect to matrix norm \c N.
  */
  template<class NORM>
  static real condition(POINT const& A1, POINT const& A2, POINT const& A3,
			NORM const& N)
    {
      POINT inv[3];
      for(int i = 0; i<3; ++i)
	pt::ConstructWithDim(3,inv[i]);
      // FIXME: should be
      // if (det3(inv[0],inv[1],inv[2]) <= numeric_traits<real>::epsilon()*max(|Ai|)
      //   return numeric_traits<real>::infinity();
      real eps = 1.0/1.e10;
      if(fabs(det3(A1,A2,A3)) <= (eps*(norm_2(A1) + norm_2(A2) + norm_2(A3))))
	return 1.0e100;
     
      invert(A1,A2,A3,inv[0],inv[1],inv[2]);
      return N(A1,A2,A3) * N(inv[0],inv[1],inv[2]);
    }
  template<class NORM>
  static real condition(matrix_type const& A,
			NORM const& N)
  { return condition(A.a[0], A.a[1], A.a[2], N);}


};

} // namespace GrAL 


#endif





