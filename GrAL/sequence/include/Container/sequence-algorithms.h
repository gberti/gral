#ifndef GRAL_GB_SEQUENCE_ALGORITHMS_H
#define GRAL_GB_SEQUENCE_ALGORITHMS_H

// $LICENSE

#include "Utility/pre-post-conditions.h"

#include <iterator>
#include <functional>
#include <algorithm>
#include <numeric>

namespace GrAL {

  /*! \brief Sequence-related algorithms and helpers
   */
namespace sequence {

  /*! \defgroup helperfunctors Helper functors for common tasks 
      \ingroup  algorithms 
  */

  /*! \brief helper predicate for comparing pairs
  
       \ingroup helperfunctors
  */
  struct less_first  {
    typedef bool result_type;
    template<class Pair>
    bool operator()(Pair const& p1, Pair const& p2) const { return p1.first < p2.first;}
  };

  /*! \brief helper predicate for comparing pairs
  
       \ingroup helperfunctors
  */
  struct less_second  {
    typedef bool result_type;
    template<class Pair>
    bool operator()(Pair const& p1, Pair const& p2) const { return p1.second < p2.second;}
  };

  /*! \brief helper predicate for comparing pairs
  
       \ingroup helperfunctors
  */
  struct greater_first  {
    typedef bool result_type;
    template<class Pair>
    bool operator()(Pair const& p1, Pair const& p2) const { return p1.first > p2.first;}
  };

  /*! \brief helper predicate for comparing pairs
  
       \ingroup helperfunctors
  */
  struct greater_second  {
    typedef bool result_type;
    template<class Pair>
    bool operator()(Pair const& p1, Pair const& p2) const { return p1.second > p2.second;}
  };

  /*! \brief helper functor for getting size
      
       \ingroup helperfunctors
   */
  struct size_functor {
    template<class T>
    typename T::size_type operator()(T const& t) const { return t.size();}
  };




  /*!
      \ingroup algorithms

      \see histogram_table
  */
  template<class ForwardIterator, class Map>
  void compute_histogram(ForwardIterator begin, ForwardIterator end, Map & hist)
  {
    while(begin != end) {
      hist[*begin]++;
      ++begin;
    }
  }
 /*!
      \ingroup algorithms

      \see histogram_table
  */
  template<class ForwardIterator, class Map, class F>
  void compute_histogram(ForwardIterator begin, ForwardIterator end, Map & hist, F f)
  {
    while(begin != end) {
      hist[f(*begin)]++;
      ++begin;
    }
  }



  /*! \brief find iterator with maximum value in map

      \ingroup algorithms
  */

  template<class Map>
  typename Map::const_iterator arg_max_map(Map const& map)
  {
    typename Map::const_iterator it    = map.begin();
    typename Map::const_iterator end   = map.end();
    typename Map::const_iterator max_iter = it;

    while(it != end) {
      if(max_iter->second < it->second) {
	max_iter = it;
      }
      ++it;
    } 
    return max_iter;
  }

  /*! \brief Check whether any item \c i in \c [first,end[ exist with <tt> p(i) == true </tt>
      \ingroup algorithms
      \see \ref test-sequence-algorithms.C
  */
 template <typename InputIterator, typename Predicate>
  inline bool exists(InputIterator first, InputIterator end, Predicate p) {
    while(first != end) {
      if(p(*first))
	return true;
      ++first;
    }
    return false; 
  }
  /*! \brief Check whether for each  item \c i in \c [first,end[ the condition <tt> p(i) == true </tt> holds
      \ingroup algorithms
      \see \ref test-sequence-algorithms.C
  */
 template <typename InputIterator, typename Predicate>
  inline bool forall(InputIterator first, InputIterator end, Predicate p) {
   bool res = true;
    while(first != end) {
      res = res && p(*first);
      ++first;
    }
    return res;
  }

  /*! \brief Check whether \c v  is contained in the set  \c [first,end[
      \ingroup algorithms
      \see \ref test-sequence-algorithms.C
  */
 template <typename InputIterator, typename Value>
 inline bool contains(InputIterator first, InputIterator end, Value const& v) {
   return ::std::find(first,end,v) != end;
 }


  /*! \brief Calculate sum of the sequence
      \ingroup algorithms
      \see \ref test-sequence-algorithms.C
   */ 
  template <typename InputIterator>
  inline typename ::std::iterator_traits<InputIterator>::value_type
  sum(InputIterator first, InputIterator end) 
  {
    return ::std::accumulate(first,end, 0, ::std::plus<typename ::std::iterator_traits<InputIterator>::value_type>());
  }

  /*! \brief The classic bubble sort.

    \see \ref test-bubblesort.C
    \ingroup algorithms
  */
  template<typename ForwardIterator, typename Less>
  int bubble_sort(ForwardIterator begin, ForwardIterator end, Less less)
  {
    if(begin == end) return 0;
    ForwardIterator next = begin; 
    ++next;
    if(next == end) return 0;

    int num_transpositions = 0;
    bool ordered = false;
    while(ordered == false) {
      ordered = true;
      while(next != end) {
	if(! less(*begin, *next)) {
	  ::std::swap(*begin, *next);
	  ordered = false;
	  ++num_transpositions;
	}
	++begin;
	++next;
      }
    } 
    return num_transpositions;
  } // bubble_sort

  template<typename ForwardIterator>
  int bubble_sort(ForwardIterator begin, ForwardIterator end)
  { return bubble_sort(begin, end, ::std::less<typename ::std::iterator_traits<ForwardIterator>::value_type>());}


  /*! \brief Check wether the two sequences have the same permutation sign
    \ingroup algorithms

    \pre The two sequences must be identical as sets
    \post The two sequences will be sorted
  */
  template<typename ForwardIterator1, typename ForwardIterator2>
  bool same_permutation_sign(ForwardIterator1 begin1, ForwardIterator1 end1,
			     ForwardIterator2 begin2, ForwardIterator2 end2)
  { 
    int t1 = bubble_sort(begin1, end1);
    int t2 = bubble_sort(begin2, end2);
    REQUIRE(::std::equal(begin1, end1, begin2), "", 1);
    return (t1%2) == (t2%2);
  }


 
  /*! \brief Make the container unique & sorted
    \ingroup algorithms

  */
  template<class Container>
  void sort_and_make_unique(Container & container)
  {
    std::sort(container.begin(), container.end());
    container.erase(unique(container.begin(), container.end()), container.end());
  }

  /*! \brief Functor to advance input iterator

  */
  template<class InputIt>
  inline InputIt next(InputIt i)
  { ++i; return i;}

  /*! \brief Functor to advance input iterator n steps
   */
  template<class InputIt>
  inline InputIt next(InputIt i, unsigned n)
  { 
    while(n > 0) {
      ++i; 
      --n;
    }
    return i;
  }

  /*! \brief copy [b,e) to [dest, ...), mapping values with f.
    \ingroup algorithms
    
    The output is the sequence f(*b), ... f(*(e-1)).
    
    This algorithm could also be expressed using ::std::copy
    and an iterator-adaptor of InputIt using Filter.
    However, as a filter rather operates on the
    corresponding value_type, this version seems to be 
    more natural.

   \todo Use ::std::transform(b,e,dest,f) instead.
  */
  template<class InputIt, class OutputIt, class Filter>
  inline
  OutputIt copy_filter(InputIt b, InputIt e, OutputIt dest, Filter f)
  {
    while( b != e) {
      *dest = f(*b);
      ++b; ++dest;
    }
    return dest;
  }
  
  /*! \brief find_if_preference tries to find an element that satisfies both predicates.
    \ingroup algorithms
    
    Return value:
    -# i, where i is the first in [b,e) that satisfies must_be && should_be
    -# i, where i is the first in [b,e) that satisfies must_be, 
    and no element in [b,e) satisfies should_be
    -# e, if no element in [b,e) satisfies must_be.
  */
  template<class FwdIt, class P1, class P2>
  inline
  FwdIt find_if_preference(FwdIt b, FwdIt e, const P1& must_be, const P2& should_be)
  {
    FwdIt found = e;
    while( b != e) {
      if(must_be(*b)) {
	if(should_be(*b))
	  return b;
	else
	  found = b;
      }
      ++b;
    }
    return found;
  }
 
  /*! \brief Copy a mapping by iterating through its domain
    \ingroup algorithms

    <b> Template parameters </b>
    - M1: operator[]
    - M2:
    - operator[]
    - typedef domain_type
    - domain_type domain()
  */
  template<class M1, class M2>
  void mapping_assign(M1& dest, M2 const& src)
  {
    typedef typename M2::domain_type dom_type;
    typedef typename dom_type::const_iterator dom_iter;
    dom_iter end = src.domain().end();
    for(dom_iter d = src.domain().begin(); d != end; ++d)
      dest[*d] = src[*d];
  }
 

} // namespace sequence

} // namespace GrAL 


#endif
