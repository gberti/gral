/*! \file 

  Test cases for some sequence algorithms
*/

#include "Container/sequence-algorithms.h"

#include "Utility/pre-post-conditions.h"

#include <vector>


int main() {
  using namespace GrAL;
  using namespace std;

  namespace seq = sequence;
  int a[3] = { 1, 2, 3};
  int b[3] = { 2, 1, 3};

  REQUIRE_ALWAYS( seq::is_increasing(a+0, a+sizeof(a)/sizeof(int)), "",1);
  REQUIRE_ALWAYS(!seq::is_increasing(b+0, b+sizeof(b)/sizeof(int)), "",1);
  REQUIRE_ALWAYS( seq::is_increasing(a+0, a+sizeof(a)/sizeof(int), less   <int>()), "",1);
  REQUIRE_ALWAYS(!seq::is_increasing(a+0, a+sizeof(a)/sizeof(int), greater<int>()), "",1);
  

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

  {
    int a[] = { 1, 2, 3};
    std::vector<int> va(a+0, a+3);
    int c[] = { 2 };
    std::vector<int> vc(c+0, c+1);
    seq::remove_set(va,vc);
    REQUIRE_ALWAYS(va.size() == 2, "",1);
    REQUIRE_ALWAYS(va[0] == 1 && va[1] == 3, "", 1);
  }
}
