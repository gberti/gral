#ifndef NMWR_GB_SET_PRIMITIVES_H
#define NMWR_GB_SET_PRIMITIVES_H

//----------------------------------------------------------------
//   (c) Guntram Berti, 1998
//   Chair for Numerical Mathematics & Scientific Computing (NMWR)
//   TU Cottbus - Germany
//   http://math-s.math.tu-cottbus.de/NMWR
//   
//----------------------------------------------------------------

#include "Container/set-traits.h"

//----------------------------------------------------------------
//
// This file is intended to collect routines related to 
// set operations.
// Currently contained functions:
//  * bool is_subset(begin,end,S) : predicate function
//  * is_element_of_pred<It,T>    : predicate class
//    is_element_of(b,e)          : corresponding helper function
//
//----------------------------------------------------------------



//------------------- is_subset(b,e,S) ---------------------------
//
//  DESCRIPTION:
//  is_subset(b,e,s2) tests the sequence [b,e)  for inclusion in the set s2.
//  is_subset(b,e,s2) == true    <=>   [b,e) \subset s2
//
//  CONTRAINTS  for template parameters:
//  * It is a forward iterator (-> STL)
//  * S2 is a set with a membership testing fct:
//      + bool is_member(const It::value_type x)
//        is_member(x) = true  <=> x \in S2
//
//  PRECONDITIONS:
//  * end must be reachable from beg, i.e. \exists n >= 0: beg+n = end
//    
//----------------------------------------------------------------

// set_traits are not used, because it requires partial specialization
// in order to work in a useful manner.

template<class It, class S2>
inline bool is_subset(It beg, It end, const S2& s2)
{
  bool is_sub = true; // empty seq. is subset
  while(is_sub && beg != end) {
    //  is_sub &&= set_traits<S2>::is_member(*beg,s2);
    is_sub =  ( is_sub && s2.is_member(*beg));
    ++beg;
  }
  return is_sub;
}


//----------------------------------------------------------------
//
//  template<class It, class T>  class is_element_of_pred:
//
//  A predicate class for testing of a value t (type T) for inclusion
//  in a range [begin,end)
//
//  CONSTRAINTS on template parameters:
//  * It: is a forward iterator
//  * T : is equality-comparable
// 
//----------------------------------------------------------------

// T could be derived from It 
// (T == iterator_traits<It>::value_type)
template<class It, class T>
class is_element_of_pred {
private:
  It begin;
  It end;
public:
  //  is_element_of_pred() {}
  is_element_of_pred(It b, It e) : begin(b), end(e) {}

  typedef T    argument_type;
  typedef bool result_type;

  bool operator()(const T& t) const {
    bool found = false;
    It i = begin;
    while( !found && (i != end)) {
      found = ((*i) == t);
      ++i;
    }
    return found;
  }
};


template<class It,class T>
inline is_element_of_pred<It,T> 
is_element_of(It b, It e, const T* /*dummy */)
{ return is_element_of_pred<It,T>(b,e);}

template<class Cont>
inline is_element_of_pred<typename Cont::const_iterator, typename Cont::value_type>
is_element_of(const Cont& c)
{ 
  typedef typename Cont::const_iterator it;
  typedef typename Cont::value_type     val;
  return  is_element_of_pred<it,val>(c.begin(),c.end());
} 

#endif
