#ifndef GRAL_GB_GEOMETRY_SCALAR_TRAITS_H
#define GRAL_GB_GEOMETRY_SCALAR_TRAITS_H


/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


/*! \file
 */

#include <complex>

namespace GrAL {


  /*! \brief Metafunction for returning real type of a scalar
    
      The real type of a scalar is the type of the absolute value (magnitude) of a scalar.
      For types \c S modeling real numbers (or a subset of it), 
      like \c float and \c double, this is \c S itself.  
      For complex numbers, this is the underlying base type.
   */
  template<class S> struct real_type { typedef S type; };

  template<class S> struct real_type<std::complex<S> > { typedef typename real_type<S>::type type; };

} //


#endif
