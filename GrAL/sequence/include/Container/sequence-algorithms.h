#ifndef GRAL_GB_SEQUENCE_ALGORITHMS_H
#define GRAL_GB_SEQUENCE_ALGORITHMS_H


#include "Utility/pre-post-conditions.h"

#include <iterator>
#include <functional>
#include <algorithm>

namespace sequence {

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

  /*! \brief helper predicate for comparing pairs
  
       \ingroup algorithms
  */
  struct less_first  {
    typedef bool result_type;
    template<class Pair>
    bool operator()(Pair const& p1, Pair const& p2) const { return p1.first < p2.first;}
  };

  /*! \brief helper predicate for comparing pairs
  
       \ingroup algorithms
  */
  struct less_second  {
    typedef bool result_type;
    template<class Pair>
    bool operator()(Pair const& p1, Pair const& p2) const { return p1.second < p2.second;}
  };

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
      \see test-sequence-algorithms.C
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
      \see test-sequence-algorithms.C
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
      \see test-sequence-algorithms.C
  */
 template <typename InputIterator, typename Value>
 inline bool contains(InputIterator first, InputIterator end, Value const& v) {
   return std::find(first,end,v) != end;
 }


  /*! \brief The classic bubble sort.

    \see test-bubblesort.C
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
	  std::swap(*begin, *next);
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
  { return bubble_sort(begin, end, std::less<typename std::iterator_traits<ForwardIterator>::value_type>());}


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
    REQUIRE(equal(begin1, end1, begin2), "", 1);
    return (t1%2) == (t2%2);
  }

} // namespace sequence

#endif
