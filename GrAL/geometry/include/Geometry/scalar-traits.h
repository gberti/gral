#ifndef GRAL_GB_GEOMETRY_SCALAR_TRAITS_H
#define GRAL_GB_GEOMETRY_SCALAR_TRAITS_H

// $LICENSE

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
