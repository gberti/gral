#ifndef NMWR_GB_SET_TRAITS_H
#define NMWR_GB_SET_TRAITS_H

//----------------------------------------------------------------
//   (c) Guntram Berti, 1998
//   Chair for Numerical Mathematics & Scientific Computing (NMWR)
//   TU Cottbus - Germany
//   http://math-s.math.tu-cottbus.de/NMWR
//   
//----------------------------------------------------------------


//----------------------------------------------------------------
//
//  empty frame for generic access to functionality of mathematical
//  sets.
//  This has to be specialized for user classes.
//  In fact many classes may be seen a sets in different ways,
//  so this opens the door to use generic set predicates on them,
//  without having to change the classes themselves.
//
//  cf. also: point_traits<> (Geometry/point-traits.h)
//
//----------------------------------------------------------------


template<class S> class set_traits {
  // to define:
  // typedef ... element_type
  // bool is_member(const element_type&);
};

#endif
