#ifndef NMWR_GB_MATRIX_NM_H
#define NMWR_GB_MATRIX_NM_H


/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti, 
    Contact: guntram AT users DOT berlios DOT de

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */



#include <iostream>
#include "Utility/pre-post-conditions.h"
#include "Geometry/coords.h"
#include "Geometry/matrix-traits.h"

namespace GrAL {

template<unsigned M, unsigned N, int offset>
struct fixed_row_mjr_subcriptor {
  static int index(unsigned i, unsigned j) { 
    REQUIRE( (offset <= i && i <= M + offset -1
	     && offset <= i && i <= N + offset -1),
	     "fixed_row_mjr_subcriptor<" << M << ',' << N << ',' << offset << '>'
	     << " : index (" << i << ',' << j << ") out of range!",1);
    return (i-offset)*N + j-offset;
  }
};

/*! \defgroup matrixalg Simple matrix class and operators

 */

/*! \brief Simple matrix class with compile-time determined dimensions

    \ingroup matrixalg

    The \c OFF parameter determines the indexing base (typically 0 or 1).
    This class is not tuned towards high performance.    

    Tested in  test-matrix.C
*/
template<unsigned M, unsigned N, int OFF = 1>
class matrix :  public coordN<N*M> {
public:
  typedef coordN<N*M> base_vec;
  typedef fixed_row_mjr_subcriptor<M,N,OFF> subscriptor;
  typedef typename base_vec::component component;
  
  matrix() {}
  matrix(component c) : base_vec(c) {}

  component& operator()(unsigned i, unsigned j) 
    { return base_vec::operator[](1+subscriptor::index(i,j));}

  const component& operator()(unsigned i, unsigned j) const
    { return base_vec::operator[](1+subscriptor::index(i,j));}

  matrix<M,N,OFF>& operator +=(const matrix<M,N,OFF>& rs) 
    { base_vec::operator+=(rs); return *this;}

  matrix<M,N,OFF>& operator -=(const matrix<M,N,OFF>& rs) 
    { base_vec::operator-=(rs); return *this;}

  matrix<M,N,OFF>& operator *=(const component& rs)
    { base_vec::operator*=(rs); return *this;}

};

/*! \brief matrix addition
    \ingroup matrixalg
*/
template<unsigned N, unsigned M, int OFF>
inline matrix<M,N,OFF> operator+(const matrix<M,N,OFF>& ls, const matrix<M,N,OFF> rs)
{ matrix<M,N,OFF> tmp(ls); return (tmp += rs);}

 
/*! \brief matrix subtraction
    \ingroup matrixalg
*/
template<unsigned N, unsigned M, int OFF>
inline matrix<M,N,OFF> operator-(const matrix<M,N,OFF>& ls, const matrix<M,N,OFF> rs)
{ matrix<M,N,OFF> tmp(ls); return (tmp -= rs);}

/*! \brief matrix 
    \ingroup matrixalg
*/
template<unsigned N, unsigned M, int OFF>
inline matrix<M,N,OFF> operator*(const matrix<M,N,OFF>& ls, coord_N_component rs)
{ matrix<M,N,OFF> tmp(ls); return (tmp *= rs);}


/*! \brief matrix-scalar multiplication
    \ingroup matrixalg
*/
template<unsigned N, unsigned M, int OFF>
inline matrix<M,N,OFF> operator*( coord_N_component ls, const matrix<M,N,OFF>& rs)
{ matrix<M,N,OFF> tmp(rs); return (tmp *= ls);}




/*! \brief matrix-matrix multiplication
    \ingroup matrixalg
*/
template<unsigned K, unsigned L, unsigned M, int OFF>
inline void mul(matrix<K,M, OFF>& res,
		const matrix<K,L, OFF>& ls, const matrix<L,M, OFF>& rs)
{
  for(unsigned k = OFF; k < K+OFF; k++)
    for(unsigned m = OFF; m < M+OFF; m++) {
      res(k,m) = 0.0;
      for(unsigned l = OFF; l < L+OFF; l++)
	res(k,m) += ls(k,l) * rs(l,m);
    }
}

/*! \brief matrix-matrix multiplication 
    \ingroup matrixalg
*/
template<unsigned K, unsigned L, unsigned M, int OFF>
inline matrix<K,M> operator*(const matrix<K,L, OFF>& ls, const matrix<L,M, OFF>& rs)
{ 
  matrix<K,M> res;
  mul(res,ls,rs);
  return res;
}


/*! \brief matrix-vector multiplication
    \ingroup matrixalg
*/
template<unsigned N, unsigned M, int OFF>
inline void mul(coordN<M> & res,
		const matrix<M,N,OFF>& ls, const coordN<N>& rs) 
{
  for(unsigned i = OFF; i < M+OFF; i++) {
    res[i-OFF+1] = 0.0;
    for(unsigned j = OFF; j < N+OFF; j++)
      res[i-OFF+1] += ls(i,j) * rs[j-OFF+1];
  }
}

/*! \brief matrix-vector multiplication
    \ingroup matrixalg
*/
template<unsigned N, unsigned M, int OFF>
inline coordN<M> operator*(const matrix<M,N,OFF>& ls, const coordN<N>& rs) 
{
  coordN<M> res;
  mul(res,ls,rs);
  return res;
}


/*! \brief matrix-vector multiplication
    \ingroup matrixalg
*/
template<unsigned N, int OFF, class P>
inline P operator*(const matrix<N,N,OFF>& ls, P rs)
{ 
  typedef point_traits<P> pt;
  P res(pt::Origin(N));
  for(unsigned m = OFF; m < N+OFF; ++m)
    for(unsigned n = OFF; n < N+OFF; ++n)
      res[m-OFF+pt::LowerIndex(rs)] += ls(m,n) * rs[n-OFF+pt::LowerIndex(rs)];
  return res;
}


// disambuigate
template<unsigned N, int OFF>
inline coordN<N> operator*(const matrix<N,N,OFF>& ls,const coordN<N>& rs) 
{ 
  coordN<N> res;
  mul(res,ls,rs);
  return res;
}
// disambuigate
template<unsigned N, int OFF>
inline matrix<N,N,OFF> operator*(const matrix<N,N,OFF>& ls, const matrix<N,N,OFF>& rs)
{ 
  matrix<N,N,OFF> res;
  mul(res,ls,rs);
  return res;
}

// disambuigate
template<unsigned N, int OFF>
inline matrix<N,N,OFF> operator*(const matrix<N,N,OFF>& ls, coord_N_component rs)
{ 
  matrix<N,N,OFF> res = ls;
  return res *= rs;
}

// disambuigate
template<unsigned N, int OFF>
inline matrix<N,N,OFF> operator*(coord_N_component ls, const matrix<N,N,OFF>& rs)
{ 
  matrix<N,N,OFF> res = rs;
  return res *= ls;
}



//----------------------- IO ------------------------------------

/*! \brief matrix output
    \ingroup matrixalg
*/
template<unsigned N, unsigned M, int OFF>
inline std::ostream& operator<<(std::ostream& out, const matrix<M,N,OFF>& rs)
{
  for(unsigned i = OFF; i< M+OFF; i++) {
    for(unsigned j = OFF; j < N+OFF; j++)
      out << rs(i,j) << ' ';
    out << '\n';
  }
  return out;
}

/*! \brief matrix input
    \ingroup matrixalg
*/
template<unsigned N, unsigned M, int OFF>
inline std::istream& operator>>(std::istream& in, matrix<M,N,OFF>& rs)
{
  for(unsigned i = OFF; i< M+OFF; i++) {
    for(unsigned j = OFF; j < N+OFF; j++)
      in >> rs(i,j);
  }
  return in;
}



/*! \brief matrix traits specialization 
    \ingroup matrixalg
*/

template<unsigned N, unsigned M, int OFF>
struct matrix_traits_matrixNMOFF
  : public matrix_traits_fixed_dim_base<M,N, matrix<M,N,OFF>, double, OFF > 
{
  typedef matrix<M,N,OFF> matrix_type;
  typedef matrix<N,M,OFF> transpose_type;
  typedef double          component_type;

  static void ConstructWithDim(matrix_type &, int, int) {}
  /*
  static int LowerRowIndex()  { return OFF;}
  static int UpperRowIndex()  { return OFF+M-1;}
  static int LowerColIndex()  { return OFF;}
  static int UpperColIndex()  { return OFF+N-1;}
  static int LowerRowIndex(matrix_type const&)  { return OFF;}
  static int UpperRowIndex(matrix_type const&)  { return OFF+M-1;}
  static int LowerColIndex(matrix_type const&)  { return OFF;}
  static int UpperColIndex(matrix_type const&)  { return OFF+N-1;}
  */

  // matrix_type identity();
  // template<class P>
  // matrix_type scaling(P s);
};

template<unsigned N, unsigned M, int OFF>
struct matrix_traits<matrix<M,N,OFF> > :
//  : public matrix_traits_transpose<matrix_traits_matrixNMOFF<M,N,OFF> >
  
  public matrix_traits_matrixNMOFF<M,N,OFF>,
  public matrix_traits_transpose<matrix_traits_matrixNMOFF<M,N,OFF>,
				 matrix_traits_matrixNMOFF<N,M,OFF> >,
  public matrix_traits_inverse  <matrix_traits_matrixNMOFF<M,N,OFF>,
				 matrix_traits_matrixNMOFF<M,M,OFF> >
{

};

} // namespace GrAL


#endif
