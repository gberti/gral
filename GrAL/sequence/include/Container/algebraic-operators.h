#ifndef GRAL_GB_UTILITY_ALGEBRAIC_OPERATORS_H
#define GRAL_GB_UTILITY_ALGEBRAIC_OPERATORS_H

#include <algorithm>  // min/max
#include "Utility/numeric-types.h"

// $LICENSE_NEC

namespace GrAL {

/*! \brief Binary algebraic operators to be used as template arguments

    \ingroup operators

    These types provide an \c operator() and support mixed argument types, using type promotion traits.
*/

namespace algebraic_operators {

  namespace nt = numeric_types;

  /*! \brief Multiplication

  */
  struct multiply { 
    template<class T,class U>  typename nt::promote<T,U>::type operator()(T const& t, U const& u) const { return t*u;} 
  };

  /*! Division.

      In case of integral types, this is integer division (with reminder).
   */
  struct divide { 
    template<class T,class U>  typename nt::promote<T,U>::type operator()(T const& t, U const& u) const { return t/u;} 
  };

  /*! \brief Addition
   */
  struct add { 
    template<class T,class U>  typename nt::promote<T,U>::type operator()(T const& t, U const& u) const { return t+u;} 
  };

  /*! \brief Subtraction
   */
  struct subtract { 
    template<class T,class U>  typename nt::promote<T,U>::type operator()(T const& t, U const& u) const { return t-u;} 
  };


  struct min { 
    template<class T,class U>  typename nt::promote<T,U>::type operator()(T const& t, U const& u) const { return ::std::min(t,u); } 
  };

  struct max { 
    template<class T,class U>  typename nt::promote<T,U>::type operator()(T const& t, U const& u) const { return ::std::max(t,u); } 
  };

} // namespace algebraic_operators

} // namespace GrAL {

#endif
