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
//
//  The class templates define iterator adaptors working on the
//  output of operator*() of an iterator, that is, they provide
//  a map of the iterators value_type.
//  Examples: select a field, as in get_first / get_second below
//
//----------------------------------------------------------------

// simple function objects to select fields of pairs.
// target is P = pair<T1,T2>

template<class P>
struct get_first {
  typedef P                       argument_type;
  typedef typename P::first_type  result_type;
  typedef typename P::first_type  value_type;

  // this does not work if pair<const T1, T2> (used in hash_map)
  //  static       value_type& value(      P& p) { return p.first;}
  static const value_type& value(const P& p) { return p.first;}
};

template<class P>
struct get_second {
  typedef P                       argument_type;
  typedef typename P::second_type result_type;
  typedef typename P::second_type value_type;

  //  static       value_type& value(      P& p) { return p.first;}
  static const value_type& value(const P& p) { return p.second;}
};



// F o It::op*
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

// F o It::op*
// F::value() must return value_type&
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
