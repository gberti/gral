#ifndef NMWR_GB_ALGEBRAIC_PRIMITIVES_3D_H
#define NMWR_GB_ALGEBRAIC_PRIMITIVES_3D_H


// $LICENSE



/*! geometric functions specific to 3D

 This can be used to specialize dimension_dependent_primitives<POINT>
 to dimension_dependent_primitives_3d<POINT> for 3D points.
*/



template<class POINT>
struct dimension_dependent_primitives_3d 
  : public basic_algebraic_primitives<POINT> {

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
      return (POINT(p[lp+2]*q[lq+3]-p[lp+3]*q[lq+2], 
		    p[lp+3]*q[lq+1]-p[lp+1]*q[lq+3], 
		    p[lp+1]*q[lq+2]-p[lp+2]*q[lq+1]));
    }

  /*! \brief Calculate inverse by Cramer's rule
    
     The inverse I = (i1,i2,i3) of the matrix (r1,r2,r3) 
     with the rows ri is calculated using Cramer's rule.
   */
  static void inverse(POINT const& r1, POINT const& r2, POINT const& r3,
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

  /*! \brief Solve \f$ (A1,A2,A3)x = b \f$ for \f$ x \f$

      The implementation uses Cramer's rule. 
   */
  static void solve(POINT const& A1, POINT const& A2, POINT const& A3,
		    POINT      & x,  POINT const& b)
    {
      // solve by Cramer's rule
      scalar d = det3(A1,A2,A3);
      REQUIRE_ALWAYS( (d != 0.0), "Matrix singular!\n",1);
      scalar d_inv = 1.0/d;
      int li = pt::LowerIndex(A1);
      x[li+0] = d_inv * det3(b, A2,A3);
      x[li+1] = d_inv * det3(A1,b ,A3);
      x[li+2] = d_inv * det3(A1,A2,b );
    }

  static real matrixnorm_infinity(POINT const& A1, POINT const& A2, POINT const& A3)
    {
      real max_sum = 0;
      for(int i = pt::LowerIndex(A1); i <= pt::UpperIndex(A1); ++i) {
	real sum = fabs(A1[i]) + fabs(A2[i]) + fabs(A3[i]);
	max_sum = (max_sum < sum ? sum : max_sum);
      }
      return max_sum;
    }

  /*! Calculate the 1-norm \f$ \|A\|_1 = \max_j \sum_i |a_{ij}| \f$

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

  // the 2-norm is the magnitude of the larges eigenvalue.
  // This is not yet implemented.
  static real matrixnorm_frobenius(POINT const& A1, POINT const& A2, POINT const& A3)
    {
      return sqrt(  squared_norm_2(A1) 
		  + squared_norm_2(A2)
		  + squared_norm_2(A3));
    }

  struct Norm_frobenius {
    real operator()(POINT const& A1, POINT const& A2, POINT const& A3) const
      { return matrixnorm_frobenius(A1,A2,A3);}
  };
  typedef Norm_frobenius Norm_F;

  struct Norm_1 {
    real operator()(POINT const& A1, POINT const& A2, POINT const& A3) const
      { return matrixnorm_1(A1,A2,A3);}
  };
  typedef Norm_1 Norm_columnsum;

  struct Norm_infinity {
    real operator()(POINT const& A1, POINT const& A2, POINT const& A3) const
      { return matrixnorm_infinity(A1,A2,A3);}
  };
  typedef Norm_infinity Norm_oo;
  typedef Norm_infinity Norm_rowsum;

  template<class NORM>
  static real condition(POINT const& A1, POINT const& A2, POINT const& A3,
			NORM const& N)
    {
      POINT inv[3];
      for(int i = 0; i<3; ++i)
	pt::ConstructWithDim(3,inv[i]);
      inverse(A1,A2,A3,inv[0],inv[1],inv[2]);
      return N(A1,A2,A3) * N(inv[0],inv[1],inv[2]);
    }


};



#endif





