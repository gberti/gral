#ifndef NMWR_GB_OPERATORS_H
#define NMWR_GB_OPERATORS_H

//----------------------------------------------------------------
//   (c) Guntram Berti, 1998
//   Chair for Numerical Mathematics & Scientific Computing (NMWR)
//   TU Cottbus - Germany
//   http://math-s.math.tu-cottbus.de/NMWR
//   
//----------------------------------------------------------------



/*!  \defgroup operators
     \ingroup accessors
*/

//! \ingroup operators 

template<class T>
struct assign {
  void operator()(T& ls, const T& rs) { ls = rs;}
};

//! \ingroup operators 

template<class T>
struct add_assign {
  void operator()(T& ls, const T& rs) { ls += rs;}
};

//! \ingroup operators 

template<class T>
struct mult_assign {
  void operator()(T& ls, const T& rs) { ls *= rs;}
};


#endif
