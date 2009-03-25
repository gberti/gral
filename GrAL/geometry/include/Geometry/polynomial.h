#ifndef GRAL_GB_GEOMETRY_POLYNOMIAL_H
#define GRAL_GB_GEOMETRY_POLYNOMIAL_H


/* ------------------------------------------------------------

    Copyright (C) 2005 - 2009 Guntram Berti, 
    Contact: guntram AT users DOT berlios DOT de

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include "Container/tuple.h"
#include "Geometry/differentiation.h"

#include <functional>

namespace GrAL {
  
  template<class ARG, class IT>
  typename std::iterator_traits<IT>::value_type
  horner_eval(ARG t, IT beg, IT end) {
    typedef typename std::iterator_traits<IT>::value_type value_type;
    value_type res(0);
    while(end != beg) {
      --end;
      res = t*res + *end;
    }
    return res;
  }
  
  /*! \brief Polynomial as an unary function
    
     Let p be a polynomial<N,ARG,VAL>
     \templateparams
     - \c N is the degree of p
     - \c ARG is the argument type of p
     - \c VAL is the value type of p. \c VAL must model a field or ring.
          For instance, \c ARG may be \c double, and \c VAL may be a vector type.

     \TODO define algebraic operators, composition etc.
  */
  template<int N, class ARG, class VAL = ARG>
  class polynomial : public std::unary_function<ARG,VAL> {
  public:
    typedef ARG argument_type;
    typedef VAL result_type;
    typedef VAL value_type;

    typedef tuple<result_type,(unsigned)(N+1)> coefficients_type;
    // FIXME: End recursion for N==0.
    typedef polynomial<N-1, argument_type, result_type> derivative_type;
  private:
    coefficients_type coefficients;

  public:
    polynomial() : coefficients(result_type(0)) {}
    polynomial(coefficients_type const& coeff) : coefficients(coeff) {}
    polynomial(result_type (&a)[N+1]) : coefficients(a+0, a+N+1) {}
    result_type & operator[](int i)       { return coefficients[i];}
    result_type   operator[](int i) const { return coefficients[i];}

    value_type operator()(argument_type t) const 
    {
      return horner_eval(t, coefficients.begin(), coefficients.end());
    }

    /*
    // composition
    template<unsigned K>
    polynomial<result_type, N*K> operator()(polynomial<result_type, K> const& p) const
    {
        
    }
    */


    derivative_type d() const { 
      // tuple<result_type,N> d_coefficients;
      typename derivative_type::coefficients_type d_coefficients;
      for(int i = 1; i <= N; ++i)
	d_coefficients[i-1] = argument_type(i)*coefficients[i];
      return derivative_type(d_coefficients);
    }
    //FIXME: inefficient
    value_type d (argument_type t) const { return d()(t);}
    value_type dd(argument_type t) const { return d().d()(t);}
    
  };

  
  namespace differentiation {
    template<int N, class ARG, class VAL>  
    struct has_builtin_gradient<polynomial<N,ARG,VAL> > { typedef true_type type; };
  }  //   namespace differentiation 


} // namespace GrAL

#endif
