
// $LICENSE_NEC_2004

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
  for(unsigned r = 0; r < ranges.size(); ++r)
    for(range_type::const_iterator rge(ranges[r].begin()); rge != ranges[r].end(); ++rge)
      for(int x = rge->low(); x <= rge->high(); ++x)
	REQUIRE_ALWAYS(ranges[r].contains(x), "x=" << x << "  ranges[r]=" << ranges[r],1);

}
