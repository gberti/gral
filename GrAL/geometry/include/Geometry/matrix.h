#ifndef NMWR_GB_MATRIX_NM_H
#define NMWR_GB_MATRIX_NM_H

// $LICENSE


#include <iostream>
#include "Utility/pre-post-conditions.h"
#include "Geometry/coords.h"
#include "Geometry/matrix-traits.h"

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

 // public vector_space_category<matrix<N,M> >, 
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


template<unsigned N, unsigned M, int OFF>
inline matrix<M,N,OFF> operator+(const matrix<M,N,OFF>& ls, const matrix<M,N,OFF> rs)
{ matrix<M,N,OFF> tmp(ls); return (tmp += rs);}

 
template<unsigned N, unsigned M, int OFF>
inline matrix<M,N,OFF> operator-(const matrix<M,N,OFF>& ls, const matrix<M,N,OFF> rs)
{ matrix<M,N,OFF> tmp(ls); return (tmp -= rs);}

template<unsigned N, unsigned M, int OFF>
inline matrix<M,N,OFF> operator*(const matrix<M,N,OFF>& ls, coord_N_component rs)
{ matrix<M,N,OFF> tmp(ls); return (tmp *= rs);}


// commutative multiplication of component required !!!
template<unsigned N, unsigned M, int OFF>
inline matrix<M,N,OFF> operator*( coord_N_component ls, const matrix<M,N,OFF>& rs)
{ matrix<M,N,OFF> tmp(rs); return (tmp *= ls);}




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

template<unsigned K, unsigned L, unsigned M, int OFF>
inline matrix<K,M> operator*(const matrix<K,L, OFF>& ls, const matrix<L,M, OFF>& rs)
{ 
  matrix<K,M> res;
  mul(res,ls,rs);
  return res;
}


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

template<unsigned N, unsigned M, int OFF>
inline coordN<M> operator*(const matrix<M,N,OFF>& ls, const coordN<N>& rs) 
{
  coordN<M> res;
  mul(res,ls,rs);
  return res;
}


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

template<unsigned N, unsigned M, int OFF>
inline std::istream& operator>>(std::istream& in, matrix<M,N,OFF>& rs)
{
  for(unsigned i = OFF; i< M+OFF; i++) {
    for(unsigned j = OFF; j < N+OFF; j++)
      in >> rs(i,j);
  }
  return in;
}



template<unsigned N, unsigned M, int OFF>
struct matrix_traits<matrix<M,N,OFF> >
  : public matrix_traits_fixed_dim_base<M,N, matrix<M,N,OFF> > 
{
  typedef matrix<M,N,OFF> matrix_type;
  typedef double component_type;
  
  static int LowerRowIndex()  { return OFF;}
  static int UpperRowIndex()  { return OFF+M;}
  static int LowerColIndex()  { return OFF;}
  static int UpperColIndex()  { return OFF+N;}
  static int LowerRowIndex(matrix_type const&)  { return OFF;}
  static int UpperRowIndex(matrix_type const&)  { return OFF+M;}
  static int LowerColIndex(matrix_type const&)  { return OFF;}
  static int UpperColIndex(matrix_type const&)  { return OFF+N;}

  // matrix_type identity();
  // template<class P>
  // matrix_type scaling(P s);
};


#endif
