#ifndef NMWR_GB_FUNCTION_ADAPTER_H
#define NMWR_GB_FUNCTION_ADAPTER_H


// $LICENSE

#include "Utility/pre-post-conditions.h"

/*! \brief Adapter for enforcing reference semantic 
    in case of "heavy" function objects.
    \ingroup accessors
   
   Model of Adaptable Unary Function.
   
   Function objects are often passed by value in STL algorithms,
   e.g. in unary_compose<F1,F2>. 
   This is not wanted in the case of 'heavy' function objects,
   thus, a references wrapper is passed by value.

   \b Example:
   
   \code
   bijective_mapping<int, int> f;
   // fill f ...
   int v[100]; iota(v,v+100,0); // v = {0,1, ..., 99}
   int w[100];
   copy_filter(v,v+100, w, make_unary_fct_ref(f));
   \endcode
   (Cf. SGI iota.)

   \todo
   The problem of passing function objects by reference vs. by value
   should be handled in some more general way,
   because it places the burden on the algorithm user which
   usually does not know whether copying is intended or not
   (at least not at the point of calling the algorithm).   
 

   \see \ref make_unary_fct_ref()
   \see \ref accessors module
*/
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

/*! creator function for unary_fct_ref
 \ingroup accessors
*/
template<class F>
inline unary_fct_ref<F> make_unary_fct_ref(const F& f) 
{ return unary_fct_ref<F>(f); }


/*! \brief Composition of two heavy maps, using reference semantics.
    \ingroup accessors

    \f$ M_1 \circ M_2 \f$, model of Adaptable Unary Function.

   \see \ref accessors module
   \see \ref compose_map
   \see  unary_fct_ref
 */
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

  //! this = \f$ (mm_1 \circ mm_2) \f$
  unary_map_composition(M1 const& mm1,M2 const& mm2) : m1(&mm1), m2(&mm2) {}

  //! \f$ (m_1 \circ m_2)(x) \f$
  result_type operator()(argument_type const& x) const { return (*m1)((*m2)(x));}

  /*! \brief \f$ \dom(m_1 \circ m_2) \f$

   Only correct if \f$ \dom(m_1) \supset \ran(m_2) \f$, because
   \f$ \dom(m_1 \circ m_2) = \dom(m_2) \cap m_2^{-1}(\ran(m_2) \cap \dom(m_1)) \f$ 
  */
  domain_type const& domain() const { return m2->domain();}
  
  //! range = m1( m1.domain())
  range_type  const& range()  const { return m1->range();}
};


/*! \brief creator function for unary_map_composition
    \ingroup accessors
 */
template<class M1, class M2>
inline
unary_map_composition<M1,M2>
compose_map( M1 const& m1, M2 const& m2)
{ return unary_map_composition<M1,M2>(m1,m2);}
 
/*! \brief Constant function

*/

template<class ARG, class RES> 
class constant : public std::unary_function<ARG,RES> {
  RES r;
public:
  constant() {}
  constant(RES rr) : r(rr) {}

  RES operator()(ARG const&) const { return r;}
};



template<class F, class ARG = typename F::argument_type, class RES = typename F::result_type>
class map_is_equal {
public:
  typedef F   mapping_type;
  typedef ARG argument_type;
  typedef RES map_result_type;
  typedef bool result_type;

private:
  ref_ptr<mapping_type const> f;
  map_result_type             res;
public:
  map_is_equal() {}
  map_is_equal(mapping_type const& ff, map_result_type r) : f(ff), res(r) {}
  bool operator()(argument_type const& a) const { return (*f)(a) == res;}
};



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
