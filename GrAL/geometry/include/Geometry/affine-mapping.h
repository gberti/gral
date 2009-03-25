#ifndef GRAL_GB_GEOMETRY_AFFINE_MAPPING_H
#define GRAL_GB_GEOMETRY_AFFINE_MAPPING_H


/* ------------------------------------------------------------

    Copyright (C) 2001 - 2009 Guntram Berti, 
    Contact: guntram AT users DOT berlios DOT de

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */
_2003

#include "Geometry/point-traits.h"
#include "Geometry/matrix-traits.h"
#include "Geometry/algebraic-primitives.h"

#include <iostream>
#include <cmath>


namespace GrAL {

/*! \brief Affine coordinate mapping

    \ingroup linear_algebra

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

  typedef point_traits<argument_type>      apt;
  typedef point_traits<result_type >       rpt;
  typedef matrix_traits<matrix_type>       mt;
  typedef algebraic_primitives<coord_type> ap;
  typedef typename rpt::component_type           scalar_type;
  typedef typename real_type<scalar_type>::type  real;
private:
  matrix_type A;
  coord_type  T; 
public:
  affine_mapping() {} // : A(identity_matrix<matrix_type>()), T(pt::Origin()) {}
  affine_mapping(matrix_type const& AA) : A(AA), T(rpt::Origin()) {}
  affine_mapping(matrix_type const& AA, coord_type const& TT) : A(AA), T(TT) {}

  matrix_type const& TheMatrix()      const { return A;}
  coord_type  const& TheTranslation() const { return T;}
  matrix_type      & TheMatrix()            { return A;}
  coord_type       & TheTranslation()       { return T;}

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


  /*! \brief Linear part of affine mapping
   */
  static self linear_part(self const& A) { return self(A.TheMatrix()); }

  /*! \brief Linear part of this mapping
   */
  self linear_part() const { return self::linear_part(*this);}

  /*! \brief Transposed of linear part
   */
  static self transposed(self const& A) { return self(mt::transpose(A.TheMatrix()));}

  /*! \brief Transposed of linear part of this mapping
   */ 
  self transposed() const { return self::transposed(*this);}

  /*! \brief Construct identity mapping
   */
  static self identity() 
  {
    typedef matrix_traits<matrix_type> mt;
    matrix_type sc(0.0);
    for(int i = mt::LowerRowIndex(sc); i <= mt::UpperRowIndex(sc); ++i)
      sc(i,i) = 1;
    return self(sc);
  }

  /*! \brief Construct translation mapping
   */
  static self translation(coord_type t)
  {
    typedef matrix_traits<matrix_type> mt;
    matrix_type sc(0.0);
    for(int i = mt::LowerRowIndex(sc); i <= mt::UpperRowIndex(sc); ++i)
      sc(i,i) = 1;
    return self(sc, t);
  } 

  /*! brief Construct scaling mapping with diagonal \c s
   */
  static self scaling(coord_type s) 
  {
    typedef matrix_traits<matrix_type> mt;
    matrix_type sc(0.0);
    for(int i = mt::LowerRowIndex(sc); i <= mt::UpperRowIndex(sc); ++i)
      sc(i,i) = s[i - mt::LowerRowIndex(sc) + apt::LowerIndex(s)];
    return self(sc);
  }

  static self scaling(scalar_type s) { return scaling(coord_type(s));}

  /*! \brief Return the inverse of \c scaling(s)
   */
  static self inverse_scaling(coord_type s) 
  {
    typedef matrix_traits<matrix_type> mt;
    matrix_type sc(0.0);
    for(int i = mt::LowerRowIndex(sc); i <= mt::UpperRowIndex(sc); ++i)
      sc(i,i) = 1/s[i - mt::LowerRowIndex(sc) + apt::LowerIndex(s)];
    return self(sc);
  }

  /*! \brief Generate a rotation about \c axis0 counterclockwise

      \param axis0 rotation axis
      \param angle rotation angle in radians
   */
  static self rotation3d(coord_type axis0, real angle) 
  {
    ap::normalize(axis0);
    scalar_type ca = cos(angle);
    scalar_type sa = sin(angle);
    coord_type axis1 = ap::any_perp(axis0);
    ap::normalize(axis1);
    coord_type axis2 = ap::vectorproduct(axis0,axis1);

    coord_type cols[3]; // columns of rotation matrix
    //  unit vectors
    coord_type e[3] = { coord_type(0.0), coord_type(0.0), coord_type(0.0)};
    int li = rpt::LowerIndex(e[0]);
    e[0][li+0] = 1;
    e[1][li+1] = 1;
    e[2][li+2] = 1;
    
    // columns are images of unit vectors
    coord_type rotaxis1 =  ca*axis1+sa*axis2;
    coord_type rotaxis2 = -sa*axis1+ca*axis2;
    cols[0] = ap::dot(axis0, e[0])*axis0 + ap::dot(axis1, e[0])*rotaxis1 + ap::dot(axis2, e[0])*rotaxis2;
    cols[1] = ap::dot(axis0, e[1])*axis0 + ap::dot(axis1, e[1])*rotaxis1 + ap::dot(axis2, e[1])*rotaxis2;
    cols[2] = ap::dot(axis0, e[2])*axis0 + ap::dot(axis1, e[2])*rotaxis1 + ap::dot(axis2, e[2])*rotaxis2;

    matrix_type R(0.0);
    int lr = mt::LowerRowIndex(R);
    int lc = mt::LowerColIndex(R);
    for(int i = 0; i < 3; ++i)
      for(int j = 0; j < 3; ++j)
	R(lr+i, lc+j) = cols[j][li+i];

    return self(R);
  }

  /*! \brief When is an angle considered zero?
   */
  static real  angle_eps() { return std::numeric_limits<real>::epsilon(); }

  /*! \brief Return mapping rotating \c x to \c Ax
   */ 
  static self rotate_to(coord_type x, coord_type Ax)
  {
    x  = ap::normalization( x);
    Ax = ap::normalization(Ax);
    if(ap::distance(x,Ax) < self::angle_eps())
      return self::identity();
    coord_type     axis  = ap::vectorproduct(x,Ax);
    real           angle = ap::angle(x,Ax);
    if(fabs(angle) < self::angle_eps() )
      return self::identity();
    else if( fabs(fabs(angle) - M_PI) < self::angle_eps()) {
      coord_type perp = ap::any_perp(x);
      return self::rotation3d(perp, angle);
    }
    else
      return rotation3d(axis,angle);
  }

  /*! \brief Inverse mapping of \c A

     This works only if the mapping is invertible.
  */
  static self inverse(self const& A) {
    typedef matrix_traits<matrix_type> mt;
    matrix_type I = mt::inverse(A.TheMatrix());
    return self(I, - (I*A.TheTranslation()));
  }

  //! \brief Inverse of this mapping
  self inverse() const { return inverse(*this);}
  
  //! \brief Return \f$ A^{-1} b \f$
  coord_type solve(coord_type const& b) {
    return inverse(*this)(b);
  }


};

template<class MATRIX, class ARGTYPE, class RESTYPE>
std::ostream& operator<<(::std::ostream& out, affine_mapping<MATRIX,ARGTYPE,RESTYPE> const& M)
{
  typedef matrix_traits<MATRIX> mt;
  typedef point_traits<RESTYPE> pt;
  int lr = mt::LowerRowIndex(M.TheMatrix());
  int lc = mt::LowerColIndex(M.TheMatrix());
  int li = pt::LowerIndex(M.TheTranslation());

  for(int i = 0; i < (int)mt::NumOfRows(M.TheMatrix()); ++i) {
    for(int j = 0; j < (int)mt::NumOfCols(M.TheMatrix()); ++j)
      out << M.TheMatrix()(i+lr, j+lc) << ' ';
    out << "  " << M.TheTranslation()[i+li] << '\n';
  }
  return out;
}

template<class MATRIX, class ARGTYPE, class RESTYPE>
std::istream& operator>>(std::istream& in, affine_mapping<MATRIX,ARGTYPE,RESTYPE> & M)
{
  typedef matrix_traits<MATRIX> mt;
  typedef point_traits<RESTYPE> pt;
  int lr = mt::LowerRowIndex(M.TheMatrix());
  int lc = mt::LowerColIndex(M.TheMatrix());
  int li = pt::LowerIndex(M.TheTranslation());

  for(int i = 0; i < (int)mt::NumOfRows(M.TheMatrix()); ++i) {
    for(int j = 0; j < (int)mt::NumOfCols(M.TheMatrix()); ++j)
      in >> M.TheMatrix()(i+lr, j+lc);
    in >> M.TheTranslation()[i+li];
  }
  return in;
}



} // namespace GrAL 

#endif
