/*! \file
    Test cases for the bubblesort algorithm and permutation sign determination.

*/

#include "Container/sequence-algorithms.h"
#include "Utility/pre-post-conditions.h"
#include <numeric>

namespace sequence {
  template<class ForwardIterator>
  bool increasing(ForwardIterator begin, ForwardIterator end)
  { return std::accumulate(begin,end, true, less_equal<typename std::iterator_traits<ForwardIterator>::value_type>());}

  template<class ForwardIterator, class LessEqual>
  bool increasing(ForwardIterator begin, ForwardIterator end, LessEqual less_equal)
  { return std::accumulate(begin,end, true, less_equal);}

  template<class ForwardIterator>
  bool decreasing(ForwardIterator begin, ForwardIterator end)
  { return std::accumulate(begin,end, true, greater_equal<typename std::iterator_traits<ForwardIterator>::value_type>());}
}


int main() {

  for(int i = 0; i <= 3; ++i) {
    int a[3] = { 3, 1, 2};
    int exp_trans[4] = { 0, 0, 1, 2};
    int ntrans = sequence::bubble_sort(a, a+i);
    REQUIRE_ALWAYS(sequence::increasing(a, a+i),"",1);
    REQUIRE_ALWAYS(ntrans == exp_trans[i],"i= " << i << " ntrans=" << ntrans, 1);
  }

  {
    int a[3] = { 3, 1, 2};
    int b[3] = { 1, 2, 3};
    REQUIRE_ALWAYS(sequence::same_permutation_sign(a, a+3, b, b+3), "", 1);
  }
  {
    int a[3] = { 3, 1, 2};
    int b[3] = { 1, 3, 2};
    REQUIRE_ALWAYS(!sequence::same_permutation_sign(a, a+3, b, b+3), "", 1);
  }
  

}
