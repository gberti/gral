#ifndef NMWR_GB_FUNCTION_ADAPTER_H
#define NMWR_GB_FUNCTION_ADAPTER_H

//----------------------------------------------------------------
//   (c) Guntram Berti, 1998
//   Chair for Numerical Mathematics & Scientific Computing (NMWR)
//   TU Cottbus - Germany
//   http://math-s.math.tu-cottbus.de/NMWR
//   
//----------------------------------------------------------------

#include "Utility/pre-post-conditions.h"

//----------------------------------------------------------------
//
// Adapter for enforcing reference semantic 
// in case of "heavy" function objects
// (NOTE: Function objects are often passed by value in STL algorithms,
//  e.g. in unary_compose<F1,F2>.)
//
//----------------------------------------------------------------

template<class F>
class unary_fct_ref {
private:
  const F* f;
public:
  typedef typename F::argument_type argument_type;
  typedef typename F::result_type   result_type;

  unary_fct_ref() :            f(0) {}
  unary_fct_ref(const F& ff) : f(&ff) {}

  result_type operator()(const argument_type& x) const  { 
    REQUIRE((f != 0), "No function!\n",1);
    return (*f)(x);
  }
};

template<class F>
inline unary_fct_ref<F> make_unary_fct_ref(const F& f) 
{ return unary_fct_ref<F>(f); }


template<class M1, class M2>
class unary_map_composition {
private:
  M1 const* m1;
  M2 const* m2;

public:
  typedef typename M2::argument_type argument_type;
  typedef typename M1::result_type   result_type;
  typedef typename M2::domain_type   domain_type;
  typedef typename M1::range_type    range_type;

  unary_map_composition(M1 const& mm1,M2 const& mm2) : m1(&mm1), m2(&mm2) {}

  result_type operator()(argument_type const& x) const { return (*m1)((*m2)(x));}

  // mot correct: domain = m1.domain \cap m1^{-1}( m1.range \cap m2.domain)
  domain_type const& domain() const { return m2->domain();}
  // range = m2( m1.range)
  range_type  const& range()  const { return m1->range();}
};

template<class M1, class M2>
inline
unary_map_composition<M1,M2>
compose_map( M1 const& m1, M2 const& m2)
{ return unary_map_composition<M1,M2>(m1,m2);}
 

/*
template<class Map>
class set_value_mutator {
private:
  typedef typename Map::value_type value_type;
  typedef typename Map::index_type index_type;
  Map* m;
  value_type t;
  
public:
  set_value_mutator(Map& mm, const value_type& tt) : m(&mm), t(tt) {}
  void operator()(const index_type& i) { (*m)[i] = t;}
};
*/

#endif
