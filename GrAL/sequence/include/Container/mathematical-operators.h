#ifndef GRAL_GB_SEQUENCE_MATHEMATICAL_OPERATORS_H
#define GRAL_GB_SEQUENCE_MATHEMATICAL_OPERATORS_H


// $LICENSE_NEC_2007

#include <cmath>

namespace GrAL {

  namespace mathematical_operators {

    struct floor {
      template<class T> T operator()(T const& t) const { return std::floor(t);}
    };
  
    struct ceil {
      template<class T> T operator()(T const& t) const { return std::ceil(t);}
    };

    struct round {
      template<class T> T operator()(T const& t) const { return std::ceil(t-0.5);}
    };

    template<class U, class T> 
    inline T clamp(U const& low, U const& high, T const& t) 
    { return (t < low ? T(low) : (t > high ? T(high) : t)); }

    
  } // namespace mathematical_operators

} // namespace GrAL


#endif
