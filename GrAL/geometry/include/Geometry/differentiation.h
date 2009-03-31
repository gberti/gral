#ifndef GRAL_GB_GEOMETRY_DIFFERENTIATION_H
#define GRAL_GB_GEOMETRY_DIFFERENTIATION_H


/* ------------------------------------------------------------

    Copyright (C) 2006 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include "Utility/ref-ptr.h"
#include "Geometry/point-traits.h"
#include "Geometry/numeric-gradient.h"


namespace GrAL { namespace differentiation {

    struct true_type {}; struct false_type {};

    /*! \defgroup derivatives_grp Derivatives of functions 

        A framework (still embryonic) to guide the computation of derivatives of function objects.
    */

    /*! \brief Primary template, to be specialized if built-in gradient supported
   
      \ingroup derivatives_grp
       In case of a built-in gradient function, one has to specialize the 
       \c builtin_gradient template, unless it conforms to the syntax 
       <tt> F::d(argument_type x) </tt>
       
     */
    template<class F>  struct has_builtin_gradient { typedef false_type type;};

    template<class F> class builtin_gradient {
    public:
      typedef typename F::argument_type argument_type;
      typedef             argument_type result_type;

    private:
      ref_ptr<F const> f;
    public:
      builtin_gradient(F const& ff) : f(ff) {}
      result_type operator()(argument_type x) const { return f->d(x);}
    };

    /*! \brief Numerical gradient of \c F

      \ingroup derivatives_grp

       \todo This is a very unreliable implementation. 
       Should use extrapolation methods.
     */
    template<class F>
    class num_gradient {
    public:
      typedef typename F::argument_type argument_type;
      typedef argument_type             result_type;

      typedef point_traits<argument_type> pt;
      //typedef typename pt::scalar_type real_type;
      typedef typename pt::component_type real_type;
    private:
      GrAL::numeric_gradient<F>        df;
    public:
      explicit num_gradient(F const& f, real_type h = 1.0e-5) : df(f) {}
      result_type operator()(argument_type x) const { return df(x); }
    };


    /*! \brief Gradient of a function type \c F

        \ingroup derivatives_grp
        If \c F has a built-in gradient functionality, the template
	\c has_builtin_gradient<> should be specialized. 
        In this case, it is assumed that the derivative of \c F can be accessed 
	by the member function \c F::d. 
        If it is named differently, the class
	builtin_gradient has to be specialized for \c F.
     */
    template<class F, class TAG = typename has_builtin_gradient<F>::type > 
    class gradient  : public num_gradient<F> 
    {
      typedef num_gradient<F> base;
    public:
      explicit gradient(F const& f) : base(f) {}
    };

    template<class F>
    class gradient<F, true_type> : public builtin_gradient<F> 
    {
      typedef builtin_gradient<F> base;
    public:
      explicit gradient(F const& f) : base(f) {}
    };

  // This may be inefficient - may be overloaded.
  // FIXME: This is a global unconstrained template ...
    template<class F> 
    typename F::argument_type grad(F& f, typename F::argument_type x) { gradient<F> g(f); return g(x); }


}} // namespace GrAL { namespace differentiation 

#endif
