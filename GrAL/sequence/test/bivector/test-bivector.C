#include "Container/bivector.h"
#include "Utility/pre-post-conditions.h"

#include <iostream>

int main() {

  bivector<int> bv1;
  REQUIRE_ALWAYS( (0  == bv1.push_back (0)),  "", 1);
  REQUIRE_ALWAYS( (-1 == bv1.push_front(-1)), "", 1);
  REQUIRE_ALWAYS( ( 1 == bv1.push_back ( 1)), "", 1);
  REQUIRE_ALWAYS( bv1.begin_index() == -1, "", 1);
  REQUIRE_ALWAYS( bv1.back_index () ==  1, "", 1);
  REQUIRE_ALWAYS( bv1.end_index  () ==  2, "", 1);
  REQUIRE_ALWAYS( bv1.size() == 3, "", 1);
  REQUIRE_ALWAYS( ! bv1.empty(), "", 1);

  REQUIRE_ALWAYS( bv1.front() == -1, "", 1);
  REQUIRE_ALWAYS( bv1.back () ==  1, "", 1);

  for(int i = -1; i <= 1; ++i)
    REQUIRE_ALWAYS( bv1[i] == i, "", 1);

  bv1.pop_front();
  REQUIRE_ALWAYS(bv1.front() == 0, "",1);
  REQUIRE_ALWAYS(bv1.begin_index() == 0, "",1);

  bv1.pop_back();
  REQUIRE_ALWAYS(bv1.front() == 0, "",1);
  REQUIRE_ALWAYS(bv1.back()  == 0, "",1);
  REQUIRE_ALWAYS(bv1.begin_index() == 0, "",1);
  REQUIRE_ALWAYS(bv1.back_index()  == 0, "",1);
  REQUIRE_ALWAYS(bv1.end_index()   == 1, "",1);

  bv1.pop_back();
  REQUIRE_ALWAYS(bv1.begin_index() == 0, "",1);
  REQUIRE_ALWAYS(bv1.back_index()  == -1, "",1);
  REQUIRE_ALWAYS(bv1.end_index()   == 0, "",1);
  REQUIRE_ALWAYS(bv1.empty(), "", 1);

  bivector<int> bv2(bv1);
  bivector<int> bv3(10);
  for(int i = 0; i < 10; ++i)
    bv3[i] = i;

  for(int i = 0; i < 10; ++i)
    REQUIRE_ALWAYS( bv3[i] == i, "", 1);


}
