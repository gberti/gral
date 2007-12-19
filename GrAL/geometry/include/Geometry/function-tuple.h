#ifndef GRAL_GB_GEOMETRY_FUNCTION_TUPLE_H
#define GRAL_GB_GEOMETRY_FUNCTION_TUPLE_H

// $LICENSE_NEC_2007

#include "Geometry/differentiation.h"
#include "Geometry/algebraic-primitives.h"

#include <limits>
#include <cmath>

namespace GrAL {

  template<class F, class tuple_factory>
  class function_tuple {
    typedef F                                               single_function_type;
    typedef typename F::value_type                          single_value_type;
    typedef typename tuple_factory::template apply<F>::type  table_type;
  public:
    typedef typename tuple_factory::template apply<single_value_type>::type value_type;
    typedef value_type                                      result_type;
    typedef typename F::argument_type                       argument_type;
    
    typedef point_traits<value_type>         pt;
    typedef algebraic_primitives<value_type> ap;
    typedef typename ap::real                real_type;
  private:
    table_type f;
  public:
    function_tuple() {}
    function_tuple(table_type const& ff) : f(ff) {}
    template<class IT>
    function_tuple(IT f_beg, IT f_end) :f(f_beg, f_end) {}

    single_function_type      & operator[](int i)       { return f[i];}
    single_function_type const& operator[](int i) const { return f[i];}

    value_type operator()(argument_type t) const {
      value_type res;
      for(int i = pt::LowerIndex(res); i <= pt::UpperIndex(res); ++i)
	res[i] = f[i](t);
      return res;
    }

    value_type d(argument_type t) const {
      value_type res;
      for(int i = pt::LowerIndex(res); i <= pt::UpperIndex(res); ++i)
	res[i] = f[i].d(t);
      return res;
    }

    value_type dd(argument_type t) const {
      value_type res;
      for(int i = pt::LowerIndex(res); i <= pt::UpperIndex(res); ++i)
	res[i] = f[i].dd(t);
      return res;
    }

    value_type tangent(argument_type t) const { return ap::normalization(d(t));}
    value_type normal (argument_type t) const { 
      value_type ddt  = dd(t);
      value_type tang = tangent();
      return ddt - ap::dot(tang, ddt) * tang;
    }

    real_type curvature(argument_type t) const { 
      value_type dt   = d(t);
      real_type n_dt = ap::norm_2(dt);
      //      return (n_dt < eps() ? 0 : ap::norm_2(ap::vectorproduct(dt, dd(t)))/ pow(n_dt,3));
      return (n_dt < eps() ? 0 : 2* ap::triangle_area(pt::Origin(), dt, dd(t)) / pow(n_dt,3));
    }
    real_type eps() const { return std::numeric_limits<real_type>::epsilon();}
    
  };

} // namespace GrAL


#endif
