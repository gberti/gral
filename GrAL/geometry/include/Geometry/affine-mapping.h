#ifndef GRAL_GB_GEOMETRY_AFFINE_MAPPING_H
#define GRAL_GB_GEOMETRY_AFFINE_MAPPING_H

// $LICENSE_NEC_2003

#include "Geometry/point-traits.h"
#include "Geometry/matrix-traits.h"

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
  typedef matrix_traits<matrix_type>  mt;
private:
  matrix_type A;
  coord_type  T; 
public:
  affine_mapping() {} // : A(identity_matrix<matrix_type>()), T(pt::Origin()) {}
  affine_mapping(matrix_type const& AA) : A(AA), T(rpt::Origin()) {}
  affine_mapping(matrix_type const& AA, coord_type const& TT) : A(AA), T(TT) {}

  /*! Evaluation operator
   */
  result_type operator()(argument_type const& x) const 
  { 
    result_type res = T;
    int lr = rpt::LowerIndex(res);
    int la = apt::LowerIndex(x);
    int lm = mt::LowerRowIndex(A);

    for(int i = lr; i <= rpt::UpperIndex(res); ++i)
      for(int j = la; j <= apt::UpperIndex(x); ++j)
	res[i] += x[j]*A(i-lr+lm,j-la+lm);
    return res;
  }
  /*! \brief Composition operator
   */
  self operator()(self const& B) const { return self(A*B.A, T + A*B.T);}


  static self identity() 
  {
    matrix_type sc(0.0);
    for(int i = apt::LowerIndex(); i <= apt::UpperIndex(); ++i)
      sc(i,i) = 1;
    return self(sc);
  }

  static self translation(coord_type t)
  {
     matrix_type sc(0.0);
    for(int i = apt::LowerIndex(t); i <= apt::UpperIndex(t); ++i)
      sc(i,i) = 1;
    return self(sc, t);
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
