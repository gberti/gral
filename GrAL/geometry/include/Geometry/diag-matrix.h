
// $LICENSE

#ifndef NMWR_GB_DIAG_MATRIX_NM
#define NMWR_GB_DIAG_MATRIX_NM

#include "Geometry/matrix.h"

namespace GrAL {

template<unsigned N>
class diag_matrix : public coordN<N> {
  typedef coordN<N> base_vec;
public:
  diag_matrix() {}
  diag_matrix(component c) : base_vec(c) {}
  diag_matrix(const base_vec& rs) : base_vec(rs) {}

  component operator()(unsigned i, unsigned j) const {
   return (i == j ? operator[](i) : component(0.0));
  }

  diag_matrix<N>& operator*=(const diag_matrix<N>& rs)
    { 
      for(unsigned i = 1; i <= N; i++)
	(*this)[i] *= rs[i];
      return *this;
    }

  diag_matrix<N>& operator+=(const diag_matrix<N>& rs)
    { base_vec::operator+=(rs); return *this;}

  diag_matrix<N>& operator-=(const diag_matrix<N>& rs)
    { base_vec::operator-=(rs); return *this;}

  diag_matrix<N>& operator*=(component rs)
    { base_vec::operator*=(rs); return *this;}
};

 
template<unsigned N>
inline diag_matrix<N> operator*(const diag_matrix<N>& ls, const diag_matrix<N>& rs)
{ diag_matrix<N> tmp(ls); return (tmp *= rs);}

template<unsigned N>
inline diag_matrix<N> operator+(const diag_matrix<N>& ls, const diag_matrix<N>& rs)
{ diag_matrix<N> tmp(ls); return (tmp += rs);}

template<unsigned N>
inline diag_matrix<N> operator-(const diag_matrix<N>& ls, const diag_matrix<N>& rs)
{ diag_matrix<N> tmp(ls); return (tmp -= rs);}


template<unsigned N, unsigned M>
void mul(matrix<N,M> & res,
	 const diag_matrix<M>& D, const matrix<M,N>& A)
{
  for(unsigned i = 1; i <= M; i++)
    for(unsigned j = 1; j <= N; j++)
      res(i,j) = D(i,i) * A(i,j);
}
template<unsigned N, unsigned M>
inline matrix<M,N> operator*(const diag_matrix<M>& D, const matrix<M,N>& A)
{
  matrix<M,N> res;
  mul(res,D,A);
  return res;
}


template<unsigned N, unsigned M>
void mul(matrix<N,M> & res,
	 const matrix<M,N>& A, const diag_matrix<N>& D)
{
  for(unsigned int i = 1; i <= M; i++)
    for(unsigned int j = 1; j <= N; j++)
      res(i,j) = A(i,j) * D(j,j);
}
template<unsigned N, unsigned M>
inline matrix<M,N> operator*(const matrix<M,N>& A,const diag_matrix<N>& D)
{
  matrix<M,N> res;
  mul(res,A,D);
  return res;
}


// commutative multiplication of component_type required !!!
template<unsigned N>
inline coordN<N> operator*(const diag_matrix<N>& ls, const coordN<N>& rs)
{ diag_matrix<N> tmp(rs); return (tmp*= ls);}

template<unsigned N>
inline ostream& operator<<(ostream& out, const diag_matrix<N>& rs)
{ return (out << coordN<N>(rs)); }

template<unsigned N>
inline istream& operator>>(istream& in, diag_matrix<N>& rs)
{ return (in >> (coordN<N>&)(rs)); }

} // namespace GrAL {

#endif
