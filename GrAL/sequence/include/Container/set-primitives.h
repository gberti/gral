#ifndef NMWR_GB_SET_PRIMITIVES_H
#define NMWR_GB_SET_PRIMITIVES_H


// $LICENSE

#include "Container/set-traits.h"
#include <iterator>

namespace GrAL {

/*! \defgroup setalgorithms Set Algorithms
    \ingroup  algorithms

   This module  collects routines related to 
   set operations.
*/



//------------------- is_subset(b,e,S) ---------------------------
/*! \brief  Tests a sequence [b,e)  for inclusion in the set s2.
  \ingroup setalgorithms

   <tt> is_subset(b,e,s2) == true </tt>  \f$ \iff [b,e) \subset s2 \f$

  
  <b> Template parameters: </b>

  - It is a Forward Iterator
  - S2 is a set with a membership testing fct:
      -  bool is_member(const It::value_type x)
      -  is_member(x) = true  \f$ \iff x \in S2 \f$

  \b Preconditions:
   -  end must be \e reachable from beg, i.e. \f$ \exists n \geq 0: beg+n = end \f$

  \todo 
  Enable set_traits.
 */    
//----------------------------------------------------------------


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
/*! \brief  Predicate class for testing if \f$ t \in [b,e) \f$
     \ingroup setalgorithms

  <b> Template parameters: </b>
  - It : ForwardIterator


*/ 
//----------------------------------------------------------------

template<class It>
class is_element_of_pred {
private:
  It begin;
  It end;
public:
  //  is_element_of_pred() {}
  is_element_of_pred(It b, It e) : begin(b), end(e) {}

  typedef std::iterator_traits<It> it;
  typedef typename it::value_type argument_type;
  //  typedef T    argument_type;
  typedef bool result_type;

  /*! returns false \f$ \iff \f$ ::std::find(begin,end,t) == end. 
  */
  bool operator()(const argument_type& t) const {
    bool found = false;
    It i = begin;
    while( !found && (i != end)) {
      found = ((*i) == t);
      ++i;
    }
    return found;
  }
};

/*! \brief creator function for is_element_of_pred<It,T>
   \ingroup setalgorithms
   \relates is_element_of_pred
 */
template<class It>
inline is_element_of_pred<It>
is_element_of(It b, It e)
{ return is_element_of_pred<It>(b,e);}


/*! \brief creator function for is_element_of_pred<It,T>
   \ingroup setalgorithms
   \relates is_element_of_pred
*/
template<class Cont>
inline is_element_of_pred<typename Cont::const_iterator>
is_element_of(const Cont& c)
{ 
  typedef typename Cont::const_iterator it;
  typedef typename Cont::value_type     val;
  return  is_element_of_pred<it>(c.begin(),c.end());
} 

} // namespace GrAL 

#endif
