#ifndef NMWR_GB_MAPPED_VALUE_ITERATOR_H
#define NMWR_GB_MAPPED_VALUE_ITERATOR_H

//----------------------------------------------------------------
//   (c) Guntram Berti, 1998
//   Chair for Numerical Mathematics & Scientific Computing (NMWR)
//   TU Cottbus - Germany
//   http://math-s.math.tu-cottbus.de/NMWR
//   
//----------------------------------------------------------------


//----------------------------------------------------------------
/*! \file
  The class templates define iterator adaptors working on the
  output of operator*() of an iterator, that is, they provide
  a map of the iterators value_type.
  Examples: select a field, as in get_first / get_second below
*/
//----------------------------------------------------------------

/*! \defgroup accessors Accessors, Function Objects and Iterator Adaptors
    \ingroup sequences

    This module defines custom function objects and iterators.
    - get_first, get_second: Get first/second element of a pair.
    - mapped_value_const_iterator<F,It>, mapped_value_iterator<F,It>:
    Compose the value (dereference operator*) 
    of an iterator It with a function F.
 */



/*! \brief Get first element of pair
    \ingroup  accessors 

   <TT> get_first </TT>, <TT> get_second </TT> are
    simple function objects to select fields of pairs.

    Target type for parameter P is  <TT> pair<T1,T2> </TT>.
   
    <TT> get_first </TT>, <TT> get_second </TT> are models of 
    Adaptable Unary Function.

*/
template<class P>
struct get_first {
  typedef P                       argument_type;
  typedef typename P::first_type  result_type;
  typedef typename P::first_type  value_type;

  // this does not work if pair<const T1, T2> (used in hash_map)
  //  static       value_type& value(      P& p) { return p.first;}
  static 
  value_type const& value(const P& p)            { return p.first;}
  value_type const& operator()(P const& p) const { return p.first;}
};

/*! \brief get second element of P. 

   \see get_first
   \ingroup  accessors
*/
template<class P>
struct get_second {
  typedef P                       argument_type;
  typedef typename P::second_type result_type;
  typedef typename P::second_type value_type;

  static 
  value_type const& value(const P& p)            { return p.second;}
  value_type const& operator()(P const& p) const { return p.second;}
};



/*! \brief Forward iterator mapping the value of <TT>It::op*</TT> with F
    (const version).
  \ingroup accessors

  Semantics:
  - <TT>op*</TT> = F o <TT>It::op*</TT>, 

   Parameters:
   - <TT> It </TT> A model of Forward Iterator
   - <TT> F  </TT> A model of  Unary Function
 */
template<class It, class F>
class mapped_value_const_iterator {
public:
  typedef typename F::value_type value_type;
private:
  It it;
  typedef mapped_value_const_iterator<It,F> self;
public:
  mapped_value_const_iterator(const It& i) : it(i) {}

  self& operator++() { ++it; return *this;}
  const value_type&  operator*() const { return F::value(*it);}
  //  friend
  bool operator==(const self& rs) const
    { return (it == rs.it);}
};


template<class It, class F>
inline mapped_value_const_iterator<It,F>
map_iter_c(It i, const F& f) { return mapped_value_const_iterator<It,F>(i);}

template<class It>
inline mapped_value_const_iterator<It,get_first<typename It::value_type> >
get1st_c(It i) {
  typedef typename It::value_type v_type;
  return mapped_value_const_iterator<It,get_first<v_type> >(i);
}

template<class It>
inline mapped_value_const_iterator<It,get_second<typename It::value_type> >
get2nd_c(It i) { 
  typedef typename It::value_type v_type;
  return mapped_value_const_iterator<It,get_second<v_type> >(i);
}

/*! \brief Forward iterator mapping the value of T>It::op*</TT> with <TT> F </TT>
  (non-const version).
  \ingroup accessors

  Semantics:
  - <TT>op*</TT> = F o <TT>It::op*</TT>, 
 
   Parameters:
   - <TT> It </TT> A model of Forward Iterator
   - <TT> F  </TT> A model of  Unary Function
        - <TT> F::value() </TT> must return <TT> value_type& </TT>
*/
template<class It, class F>
class mapped_value_iterator {
private:
  It it;
  typedef mapped_value_iterator<It,F>  self;
public:
  typedef typename F::value_type value_type;

  mapped_value_iterator(const It& i) : it(i) {}

  self& operator++() { ++it; return *this;}
  value_type&  operator*() const { return F::value(*it);}
  //friend 
  bool operator==(const self& rs) const
    { return (it == rs.it);}
};



template<class It, class F>
inline mapped_value_iterator<It,F>
map_iter(It i, const F& f) { return mapped_value_iterator<It,F>(i);}

template<class It>
inline mapped_value_iterator<It,get_first<typename It::value_type> >
get1st(It i) { 
  typedef typename It::value_type v_type;
  return mapped_value_iterator<It,get_first<v_type> >(i);
}

template<class It>
inline mapped_value_iterator<It,get_second<typename It::value_type> >
get2nd(It i) { 
  typedef typename It::value_type v_type;
  return mapped_value_iterator<It,get_second<v_type> >(i);
}

//----------------------------------------------------------------
//        iterator traits for the above templates
//----------------------------------------------------------------

// requires partial specialization -- disabled for the moment
// template<class It, class F>
// struct iterator_traits<mapped_value_const_iterator<It,F> >
// {
//   typedef iterator_traits<It> bt;
//   typedef typename bt::iterator_category iterator_category;
//   typedef typename bt::difference_type   difference_type;

//   typedef typename F::result_type        value_type;
//   typedef const value_type&              reference;
//   typedef const value_type*              pointer;
// };

// template<class It, class F>
// struct iterator_traits<mapped_value_iterator<It,F> >
// {
//   typedef iterator_traits<It> bt;
//   typedef typename bt::iterator_category iterator_category;
//   typedef typename bt::difference_type   difference_type;

//   typedef typename F::result_type        value_type;
//   typedef value_type&                    reference;
//   typedef value_type*                    pointer;
// };


#endif
