/*! \file 

  Test cases for some sequence algorithms
*/

#include "Container/sequence-algorithms.h"

#include "Utility/pre-post-conditions.h"



int main() {
  using namespace std;

  namespace seq = sequence;
  int a[3] = { 1, 2, 3};


  REQUIRE_ALWAYS( seq::contains(a, a+sizeof(a)/sizeof(int), 1), "",1);
  REQUIRE_ALWAYS( seq::contains(a, a+sizeof(a)/sizeof(int), 2), "",1);
  REQUIRE_ALWAYS( seq::contains(a, a+sizeof(a)/sizeof(int), 3), "",1);
  REQUIRE_ALWAYS(!seq::contains(a, a+sizeof(a)/sizeof(int), 4), "",1);

  REQUIRE_ALWAYS( seq::exists(a, a+sizeof(a)/sizeof(int), bind1st(equal_to<int>(), 1)), "",1);
  REQUIRE_ALWAYS( seq::exists(a, a+sizeof(a)/sizeof(int), bind1st(equal_to<int>(), 2)), "",1);
  REQUIRE_ALWAYS( seq::exists(a, a+sizeof(a)/sizeof(int), bind1st(equal_to<int>(), 3)), "",1);
  REQUIRE_ALWAYS(!seq::exists(a, a+sizeof(a)/sizeof(int), bind1st(equal_to<int>(), 4)), "",1);

  REQUIRE_ALWAYS( seq::forall(a, a+sizeof(a)/sizeof(int), bind2nd(greater<int>(), 0)), "",1);
  REQUIRE_ALWAYS(!seq::forall(a, a+sizeof(a)/sizeof(int), bind2nd(greater<int>(), 1)), "",1);
  REQUIRE_ALWAYS(!seq::forall(a, a+sizeof(a)/sizeof(int), bind2nd(greater<int>(), 2)), "",1);
  REQUIRE_ALWAYS(!seq::forall(a, a+sizeof(a)/sizeof(int), bind2nd(greater<int>(), 3)), "",1);

  REQUIRE_ALWAYS(seq::sum(a,a+sizeof(a)/sizeof(int)) == 6, " result=" << seq::sum(a,a+sizeof(a)/sizeof(int)),1);

}
