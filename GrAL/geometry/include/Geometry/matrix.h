#ifndef NMWR_GB_MATRIX_NM_H
#define NMWR_GB_MATRIX_NM_H

// $LICENSE


#include <iostream>
#include "Utility/pre-post-conditions.h"
#include "Geometry/coords.h"

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
  for(unsigned k = 1; k <= K; k++)
    for(unsigned m = 1; m <= M; m++) {
      res(k,m) = 0.0;
      for(unsigned l = 1; l <= L; l++)
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
inline void mul(coordN<N> & res,
		const matrix<M,N,OFF>& ls, const coordN<N>& rs) 
{
  for(unsigned i = 1; i <= M; i++) {
    res[i] = 0.0;
    for(unsigned j = 1; j <= M; j++)
      res[i] += ls(i,j) * rs[j];
  }
}

template<unsigned N, unsigned M, int OFF>
inline coordN<M> operator*(const matrix<M,N,OFF>& ls, const coordN<N>& rs) 
{
  coordN<M> res;
  mul(res,ls,rs);
  return res;
}


//----------------------- IO ------------------------------------

template<unsigned N, unsigned M, int OFF>
inline std::ostream& operator<<(std::ostream& out, const matrix<M,N,OFF>& rs)
{
  for(unsigned i = 1; i<= M; i++) {
    for(unsigned j = 1; j <= N; j++)
      out << rs(i,j) << ' ';
    out << '\n';
  }
  return out;
}

template<unsigned N, unsigned M, int OFF>
inline std::istream& operator>>(std::istream& in, matrix<M,N,OFF>& rs)
{
  for(unsigned i = 1; i<= M; i++) {
    for(unsigned j = 1; j <= N; j++)
      in >> rs(i,j);
  }
  return in;
}




#endif
