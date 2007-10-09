#ifndef GRAL_GB_GEOMETRY_NUMERIC_GRADIENT_H
#define GRAL_GB_GEOMETRY_NUMERIC_GRADIENT_H

// $LICENSE_NEC_2006

#include "Utility/ref-ptr.h"
#include "Geometry/point-traits.h"
#include "Container/tuple.h"
#include "Container/tuple-point-traits.h"


namespace GrAL {

  template<class F>
  class numeric_gradient {
  public:
    typedef typename F::argument_type argument_type;
    typedef typename F::argument_type result_type;
    typedef typename F::result_type   scalar_type;
  private:
    ref_ptr<const F> f;
    scalar_type      h;

    typedef point_traits<argument_type> pt;
  public:
    /*! \brief Construct with function \c ff and stepsize \c hh

      \todo The stepsize should be computed from the eps of the \c scalar_type
    */
    explicit numeric_gradient(F const& ff, scalar_type hh = 1.0e-5) : f(ff), h(hh) {}
    
    scalar_type d(argument_type x, int i) const 
    { 
      argument_type xh = x;
      xh[i] += h;
      return ((*f)(xh) - (*f)(x)) * 1.0/h;
    }
    /*! \brief  The total derivative
     */ 
    result_type operator()(argument_type x) const {
      result_type res = pt::Origin();
      for(int i = pt::LowerIndex(); i <= pt::UpperIndex(); ++i)
	res[i] = d(x,i);
      return res;
    }
  }; // numeric_gradient


}

#endif
