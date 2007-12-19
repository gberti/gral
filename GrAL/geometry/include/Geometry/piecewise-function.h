#ifndef GRAL_GB_GEOMETRY_PIECEWISE_FUNCTION_H
#define GRAL_GB_GEOMETRY_PIECEWISE_FUNCTION_H

// $LICENSE_NEC_2007

#include "Utility/checked-istream.h"
#include "Utility/pre-post-conditions.h"

#include "Container/mathematical-operators.h"
#include "Geometry/differentiation.h"

#include <vector>


namespace GrAL {

  /*! \brief Piecewise function 

     Represents a piecewise defined function \e p.
     The input for the constructor of \e p is a sequence of \e n functions \f$ f_0, \ldots f_{n-1} \f$.
     It is assumed that these functions are parameterised (at least)
     over [0,1] (typical e.g. for the individual cubics composing a cubic spline).
     Then the parameter range for \e p is [0,n] where \f$ p(t) = f_i(t-i) \f$ for \f$ t \in [i,i+1)\f$ 
     with \f$ i = \lfloor t \rfloor \f$. 

     \see Tested in \ref test-piecewise-function.C
   */
  template<class F>
  class piecewise_function {
  public:
    typedef F                           function_type;
    typedef typename F::derivative_type function_derivative_type;
    typedef piecewise_function<function_derivative_type> derivative_type;
    typedef typename F::argument_type   argument_type;
    typedef typename F::result_type     result_type;
    typedef result_type                 value_type;
  private:
    std::vector<function_type> f;
  public:
    piecewise_function() {}

    template<class F_IT>
    explicit piecewise_function(F_IT function_begin, F_IT function_end)
      : f(function_begin, function_end) {}

    struct index_arg {
      int        index;
      value_type arg;
    };
    index_arg get_index_and_arg(argument_type t) const {
      index_arg res;
      res.index = mathematical_operators::clamp(0, (int)(f.size()-1), (int) floor(t));
      res.arg = t-res.index;
      return res;
    }
    result_type operator()(argument_type t) const {
      //int i = mathematical_operators::clamp(0, (int)(f.size()-1), (int) floor(t));
      index_arg x = get_index_and_arg(t);
      return f[x.index](x.arg);
    }

    function_type& operator[](int i) { return f[i];}

    derivative_type d() const {
      // not very efficient
      derivative_type deriv(f.begin(), f.end());
      for(int i = 0; i < f.size(); ++i)
	deriv[i] = f[i].d();
      return deriv;
    }

    // FIXME: should use generic gradient function for F   
    result_type d(argument_type t) const { 
      index_arg x = get_index_and_arg(t);
      return f[x.index].d(x.arg);  
    }
    
    result_type dd(argument_type t) const { 
      index_arg x = get_index_and_arg(t);
      return f[x.index].dd(x.arg);  
    }


  };

  namespace differentiation {
    template<class F>
    struct has_builtin_gradient<piecewise_function<F> > : public has_builtin_gradient<F> {};
  } // namespace differentiation


} // namespace GrAL


#endif
