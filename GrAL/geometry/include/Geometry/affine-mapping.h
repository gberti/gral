#ifndef GRAL_GB_GEOMETRY_AFFINE_MAPPING_H
#define GRAL_GB_GEOMETRY_AFFINE_MAPPING_H

// $LICENSE_NEC_2003

#include "Geometry/point-traits.h"

/*
template<class MATRIX>
inline identity(unsigned N) { 
  typedef matrix_traits<MATRIX> mt;
  MATRIX Id;
  mt::ConstructZeroMatrix(Id,N,N);
  for(int i = mt::LowerRowIndex(Id); i < mt::UpperRowIndex(Id); ++i)
    mt::at(Id,i,i) = 0;
}
*/

/*! \brief Affine coordinate mapping

    \see test-affine-mapping.C
 */
template<class MATRIX, class ARGTYPE, class RESTYPE = ARGTYPE>
class affine_mapping {
  typedef affine_mapping<MATRIX, ARGTYPE, RESTYPE> self;
public:
  typedef MATRIX matrix_type;

  typedef ARGTYPE    argument_type;
  typedef RESTYPE    result_type;
  typedef RESTYPE    coord_type;

  typedef point_traits<argument_type> apt;
  typedef point_traits<result_type >  rpt;
private:
  matrix_type A;
  coord_type  T; 
public:
  affine_mapping() {} // : A(identity_matrix<matrix_type>()), T(pt::Origin()) {}
  affine_mapping(matrix_type const& AA) : A(AA), T(rpt::Origin()) {}
  affine_mapping(matrix_type const& AA, coord_type const& TT) : A(AA), T(TT) {}

  result_type operator()(argument_type const& x) const 
  { 
    result_type res = T;
    for(int i = rpt::LowerIndex(res); i <= rpt::UpperIndex(res); ++i)
      for(int j = apt::LowerIndex(x); j <= apt::UpperIndex(x); ++j)
	res[i] += x[j]*A(i,j);
    return res;
  }

  static self scaling(coord_type s) 
  {
    matrix_type sc(0.0);
    for(int i = apt::LowerIndex(s); i <= apt::UpperIndex(s); ++i)
      sc(i,i) = s[i];
    return self(sc);
  }
  static self inverse_scaling(coord_type s) 
  {
    matrix_type sc(0.0);
    for(int i = apt::LowerIndex(s); i <= apt::UpperIndex(s); ++i)
      sc(i,i) = 1/s[i];
    return self(sc);
  }
  // static self inverse();
};

#endif
