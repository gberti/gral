#ifndef NMWR_GB_DUMMY_MAPPING_H
#define NMWR_GB_DUMMY_MAPPING_H

//----------------------------------------------------------------
//   (c) Guntram Berti, 1998
//   Chair for Numerical Mathematics & Scientific Computing (NMWR)
//   TU Cottbus - Germany
//   http://math-s.math.tu-cottbus.de/NMWR
//   
//----------------------------------------------------------------

/*! \brief dummy mapping to substitute for an unneeded mapping parameter.
    \ingroup mappings

    sample use: see Grids/Complex2D/generic/complex2d-construct.C
*/

template<class T1, class T2>
class dummy_mapping {
private:
  T2 dummy;
public:
  dummy_mapping() {}
 
  const T2& operator()(const T1&) const { return dummy;}
  T2&       operator[](const T1&)       { return dummy;}

  typedef T1 argument_type;
  typedef T2 result_type;
};

#endif
