/*! \file 

    Test harness for histogram_table and compute_histogram
 */

/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti,
    Contact: guntram AT users DOT berlios DOT de

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include "Container/histogram-table.h"
#include "Container/sequence-algorithms.h"
#include "Utility/pre-post-conditions.h"

#include <iostream>

int main() {
  using namespace GrAL;

  histogram_table<int> H;
  H[0]++;
  H[2]++;
  H[-3]++;

  for(int i = -4; i <= 4; ++i)
    std::cout << "H[" << i << "] = " << H(i) << '\n';

  int a[6] = { 1, 1, 1, 2, 2, 3};
  histogram_table<int> H2;
  sequence::compute_histogram(a,a+6,H2);
  REQUIRE_ALWAYS(H2(1) == 3, "",1);
  REQUIRE_ALWAYS(H2(2) == 2, "",1);
  REQUIRE_ALWAYS(H2(3) == 1, "",1);

  for(histogram_table<int>::const_iterator i = H2.begin(); i != H2.end(); ++i)
    std::cout << "H2[" << i->first << "] = " << i->second << '\n';

  histogram_table<int>::const_iterator mx = sequence::arg_max_map(H2);
  std::cout << "Max. item: " << mx->first << std::endl;

  REQUIRE_ALWAYS((sequence::arg_max_map(H2)->first == 1), "", 1);

  int b[6] = { 3, 2, 2, 1, 1, 1};
  histogram_table<int> H3(b,b+6);
  REQUIRE_ALWAYS(H3.max_entry() == 1, "max_entry()=" << H3.max_entry(),1);


}
