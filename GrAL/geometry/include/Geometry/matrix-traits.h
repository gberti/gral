#ifndef GRAL_GB_GEOMETRY_MATRIX_TRAITS_H
#define GRAL_GB_GEOMETRY_MATRIX_TRAITS_H

// $LICENSE_NEC_2003

/*! To be specialized for concrete matrix types
 */
template<class MATRIX>
struct matrix_traits {};


template<class MATRIX>
struct matrix_traits_base 
{
  typedef MATRIX matrix_type;
};

template<unsigned m, unsigned n, class MATRIX>
struct matrix_traits_fixed_dim_base : public matrix_traits_base<MATRIX> {
  typedef MATRIX matrix_type;

  static unsigned NumOfRows() { return m;}
  static unsigned NumOfCols() { return n;}
  static unsigned NumOfRows(matrix_type const&) { return m;}
  static unsigned NumOfCols(matrix_type const&) { return n;}

  enum { num_of_rows = m, num_of_cols = n};
};

#endif
