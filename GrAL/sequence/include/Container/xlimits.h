#ifndef GRAL_GB_SEQUENCE_XLIMITS_H
#define GRAL_GB_SEQUENCE_XLIMITS_H

/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */

#include <limits>

namespace GrAL {

  template<class T, bool IS_INTEGER> struct xnumeric_limits_aux {};

  template<class T> struct  xnumeric_limits_aux<T, true>
    : public std::numeric_limits<T> {
    static T lower_bound() { return std::numeric_limits<T>::min();}
    static T upper_bound() { return std::numeric_limits<T>::max();}
  };
  template<class T> struct  xnumeric_limits_aux<T, false>
    : public std::numeric_limits<T> {
    static T lower_bound() { return - std::numeric_limits<T>::max();}
    static T upper_bound() { return   std::numeric_limits<T>::max();}
  };

  /*! \brief Extended numeric limits

      There is no uniform way to access the smallest value of a given type.
      std::numeric_limits<T>::min() returns the smallest positive value for
      floating point types and the smallest value for integral types.
   */
  template<class T> struct xnumeric_limits 
    : public xnumeric_limits_aux<T, std::numeric_limits<T>::is_integer>  {};
}


#endif
