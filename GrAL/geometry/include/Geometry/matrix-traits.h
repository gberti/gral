#ifndef GRAL_GB_GEOMETRY_MATRIX_TRAITS_H
#define GRAL_GB_GEOMETRY_MATRIX_TRAITS_H

// $LICENSE_NEC_2004

#include "Utility/pre-post-conditions.h"

#include <vector>

namespace GrAL {

/*! To be specialized for concrete matrix types
 */
template<class MATRIX>
struct matrix_traits 
{
#ifdef DOXYGEN
  //!
  typedef MATRIX matrix_type;
  //! For matrices with compile-time dimensions
  static unsigned NumOfRows();
  //! For matrices with compile-time dimensions
  static unsigned NumOfCols();
  //!
  static unsigned NumOfRows(matrix_type const&);
  //!
  static unsigned NumOfCols(matrix_type const&);

  static inverse_type inverse(matrix_type const&);
  static transpose_type transpose(matrix_type const&);
#endif
};


template<class MATRIX, class COMP>
struct matrix_traits_base 
{
  typedef MATRIX matrix_type;
  typedef COMP   component_type;
  typedef MATRIX transpose_type;
  typedef MATRIX inverse_type;

  static component_type const& at(matrix_type const& M, int i, int j) { return M(i,j);}
  static component_type      & at(matrix_type      & M, int i, int j) { return M(i,j);}

};



template<class MT, class TT>
struct matrix_traits_transpose 
{
private:
  typedef typename MT::matrix_type      matrix_type;
  //  typedef typename MT::transpose_type   transpose_type;
  typedef typename TT::matrix_type      transpose_type;
  // typedef matrix_traits<transpose_type> TT;

public:
  static transpose_type transpose(matrix_type const& M)
  {
    transpose_type T;
    TT::ConstructWithDim(T, MT::NumOfCols(M), MT::NumOfRows(M));
    for(int i = MT::LowerRowIndex(M); i <= MT::UpperRowIndex(M); ++i)
      for(int j = MT::LowerColIndex(M); j <= MT::UpperColIndex(M); ++j)
	TT::at(T,j,i) = MT::at(M,i,j);
    return T;
  }
};

template<class MT, class IT>
struct matrix_traits_inverse
{
private:
  typedef typename MT::matrix_type      matrix_type;
  //  typedef typename MT::transpose_type   transpose_type;
  typedef typename IT::matrix_type      inverse_type;
  typedef typename MT::component_type   component_type;
  // typedef matrix_traits<transpose_type> TT;



  static void get_pivot_row(int k, int& p_k, matrix_type const& M)
  {
    // get pivot row
    int N = MT::NumOfCols(M);
    component_type mx = 0;
    p_k               = 0;
    for(int i = k; i <= N; ++i) {
      // get row sum
      component_type sum = 0;
      for(int j = k; j <= N; ++j)
	sum += fabs(IT::at1(M,i,j));
      component_type q = fabs(IT::at1(M,i,k))/sum;
      if(q > mx) {
	mx   = q;
	p_k  = i;
      }
    }
  }

  static void swap_rows(int k1, int k2, matrix_type & M) 
  {
    if(k1 != k2) {
      int N = MT::NumOfCols(M);
      for(int j = 1; j <= N; ++j)
	std::swap(MT::at1(M,k1,j), MT::at1(M,k2,j));
    }
  }

  static void swap_cols(int k1, int k2, matrix_type & M)
  {
    if(k1 != k2) {
      int N = MT::NumOfRows(M);
      for(int j = 1; j <= N; ++j)
	std::swap(MT::at1(M,j,k1), MT::at1(M,j,k2));
    }
  }


public:
  static component_type det(matrix_type const& M) {
    REQUIRE_ALWAYS(MT::NumOfCols(M) == MT::NumOfRows(M), "",1);
    if(false && MT::NumOfCols(M) == 2)
      return MT::at1(M,1,1)*MT::at1(M,2,2) - MT::at1(M,1,2) * MT::at1(M,2,1);
    else if(false && MT::NumOfCols(M) == 3)
      return MT::at1(M,1,1)*MT::at1(M,2,2)*MT::at1(M,3,3)
	+    MT::at1(M,1,2)*MT::at1(M,2,3)*MT::at1(M,3,1)
	+    MT::at1(M,1,3)*MT::at1(M,2,1)*MT::at1(M,3,2)
	-    MT::at1(M,3,1)*MT::at1(M,2,2)*MT::at1(M,1,3)
	-    MT::at1(M,3,2)*MT::at1(M,2,3)*MT::at1(M,1,1)
	-    MT::at1(M,3,3)*MT::at1(M,2,1)*MT::at1(M,1,2);
    else {
      // See H.R. Schwarz, Numerische Mathematik, Teubner 1988, p. 22
      component_type det = 1;
      matrix_type T = M;
      int N = MT::NumOfCols(T);
      std::vector<int> p(N+1,0); 
      for(int k = 1; k <= N; ++k) {
	get_pivot_row(k,p[k], T);
	if(p[k] != k) {
	  det = -det;
	  swap_rows(k,p[k],T);
	}
	det *= MT::at1(T,k,k);
	for(int i = k+1; i <= N; ++i) {
	  MT::at1(T,i,k) /= MT::at1(T,k,k);
	  for(int j = k+1; j <= N; ++j)
	    MT::at1(T,i,j) -= MT::at1(T,i,k) * MT::at1(T,k,j);
	}
      }
      det *= MT::at1(T,N,N);
      return det;
    }
  }

  static inverse_type inverse(matrix_type const& M)
  {
    REQUIRE_ALWAYS(MT::NumOfCols(M) == MT::NumOfRows(M), "",1);
    inverse_type I;
    IT::ConstructWithDim(I, MT::NumOfCols(M), MT::NumOfRows(M));
    if(false && MT::NumOfCols(M) == 2) {
      component_type d = det(M);
      IT::at1(I,1,1) =  MT::at1(M,2,2) / d;
      IT::at1(I,2,2) =  MT::at1(M,1,1) / d;
      IT::at1(I,1,2) = -MT::at1(M,1,2) / d;
      IT::at1(I,2,1) = -MT::at1(M,2,1) / d;
      return I;
    }
    // general 
    // See H.R. Schwarz, Numerische Mathematik, Teubner 1988, p. 52
    else {
      I = M;
      int N = IT::NumOfCols(M);
      std::vector<int> p(N+1, 0); // permutation, indexing from 1

      for(int k = 1; k <= N; ++k) {
	get_pivot_row(k,p[k], I);
	REQUIRE(p[k] > 0, "Matrix not invertible!",1);

	swap_rows(k,p[k], I);

	// linear exchange step
	component_type pivot = IT::at1(I,k,k);
	for(int j = 1; j <= N; ++j) {
	  if(j != k) {
	    IT::at1(I,k,j) *= -1.0/pivot;
	    for(int i = 1; i <= N; ++i) {
	      if(i != k)
		IT::at1(I,i,j) += IT::at1(I,i,k) * IT::at1(I,k,j);
	    }
	  }
	}
	for(int i = 1; i <= N; ++i) {
	  IT::at1(I,i,k) /= pivot;
	}
	IT::at1(I,k,k) = 1/pivot;
      }
     
      for(int k = N; k >= 1; --k)
	swap_cols(k, p[k], I);

      return I;
    }
  }

};




template<unsigned m, unsigned n, class MATRIX, class COMP, int off = 0>
struct matrix_traits_fixed_dim_base : public matrix_traits_base<MATRIX, COMP> {
  typedef MATRIX matrix_type;
  typedef COMP   component_type;

  static unsigned NumOfRows() { return m;}
  static unsigned NumOfCols() { return n;}
  static unsigned NumOfRows(matrix_type const&) { return m;}
  static unsigned NumOfCols(matrix_type const&) { return n;}
  static int LowerRowIndex()                   { return       off;}
  static int LowerColIndex()                   { return       off;}
  static int UpperRowIndex()                   { return m-1 + off;}
  static int UpperColIndex()                   { return n-1 + off;}
  static int LowerRowIndex(matrix_type const&) { return       off;}
  static int LowerColIndex(matrix_type const&) { return       off;}
  static int UpperRowIndex(matrix_type const&) { return m-1 + off;}
  static int UpperColIndex(matrix_type const&) { return n-1 + off;}

  //! 0-based indexing
  static component_type const& at0(matrix_type const& M, int i, int j) { return M(i+LowerRowIndex(M), 
										  j+LowerColIndex(M)); }

  //! 0-based indexing
  static component_type      & at0(matrix_type      & M, int i, int j) { return M(i+LowerRowIndex(M), 
										  j+LowerColIndex(M)); }

  //! 1-based indexing
  static component_type const& at1(matrix_type const& M, int i, int j) { return M(i-1+LowerRowIndex(M), 
										  j-1+LowerColIndex(M)); }

  //! 1-based indexing
  static component_type      & at1(matrix_type      & M, int i, int j) { return M(i-1+LowerRowIndex(M), 
										  j-1+LowerColIndex(M)); }


  enum { num_of_rows = m, num_of_cols = n};
};

} // namespace GrAL

#endif
