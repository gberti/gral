

/* ------------------------------------------------------------

    Copyright (C) 2004 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


/*! \file

*/

#include "Container/integer-range.h"

#include <fstream>
#include <vector>

namespace GrAL { namespace sequence {
  template class integer_range<int>;
}}

int main() {
  using namespace std;
  typedef GrAL::sequence::integer_range<int> range_type;
  ifstream in("range.dat");


  range_type range;   REQUIRE_ALWAYS( range.empty(), "",1);
  vector<range_type> ranges;
  while(in >> range) {
    REQUIRE_ALWAYS(!range.empty(), "",1);
    ranges.push_back(range);
    cout << range << endl;
  }
  int a[] = { 0, 1, 2, -3, -2,  4, 6, 5 };

  ranges.push_back(range_type(a,a+sizeof(a)/sizeof(int)));
  cout << ranges.back() << endl;

  for(unsigned r = 0; r < ranges.size(); ++r)
    for(range_type::const_iterator x(ranges[r].begin()); x != ranges[r].end(); ++x)
      REQUIRE_ALWAYS(ranges[r].contains(*x), "x=" << *x  << "(x.i = " << x.i << ")" 
		     << "  ranges[" << r << "]=" << ranges[r],1);

}
