#ifndef GRAL_GB_SEQUENCE_ALGORITHMS_H
#define GRAL_GB_SEQUENCE_ALGORITHMS_H

namespace sequence {

  template<class ForwardIterator, class Map>
  void compute_histogram(ForwardIterator begin, ForwardIterator end, Map & hist)
  {
    while(begin != end) {
      hist[*begin]++;
      ++begin;
    }
  }

  struct less_first  {
    typedef bool result_type;
    template<class Pair>
    bool operator()(Pair const& p1, Pair const& p2) const { return p1.first < p2.first;}
  };
  struct less_second  {
    typedef bool result_type;
    template<class Pair>
    bool operator()(Pair const& p1, Pair const& p2) const { return p1.second < p2.second;}
  };


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


 template <typename InputIterator, typename Predicate>
  inline bool exists(InputIterator first, InputIterator last, Predicate p) {
    while(first != last) {
      if(p(*first))
	return true;
      ++first;
    }
    return false; 
  }

} // namespace sequence

#endif
