/*! \file */

// $LICENSE_NEC_2007

#include "Gral/Geometry/bucketing.h"

#include "Utility/pre-post-conditions.h"
#include "Container/range.h"

#include <iostream>

namespace GrAL {
  template class bucket_table<int,2>;
}


int main() {
  using namespace std;

  typedef GrAL::bucket_table<int, 2>::coord_type c_type;
  typedef GrAL::bucket_table<c_type, 2>  bucket_type;
  typedef bucket_type::grid_type      grid_type;
  typedef GrAL::grid_types<grid_type> gt;
  typedef bucket_type::index_type index_type;
  typedef bucket_type::coord_type coord_type;

  coord_type min_x(1.0,2.0);
  coord_type max_x(5.0,6.0);
  index_type n(2,4);
  bucket_type b(n, min_x, max_x);
  cout << "Bucket: cells [" << b.TheGrid().low_cell_index()  << "  " << b.TheGrid().high_cell_index() <<"]\n";
  cout << "Bucket: range [" << b.min() << ", " << b.max() << "]\n";

  
  index_type N(10,10);
  coord_type delta = quotient(max_x-min_x, coord_type(N));
  for(int x = 0; x < N[0]; ++x)
    for(int y = 0; y < N[1]; ++y) {
      coord_type xy = min_x + product(delta, coord_type(x,y));
      b[xy].push_back(xy);
      REQUIRE_ALWAYS(b(xy).back() == xy, "", 1);
    }


  //  for(gt::CellIterator c = GrAL::begin_x(b.TheGrid()); c != GrAL::end_x(b.TheGrid()); ++c) {
  for(gt::CellIterator c(b.TheGrid()); !c.IsDone(); ++c) {
    cout << "bucket " << c.index() << ": " << GrAL::range_c(b.get(*c), ", ", "(", ")") << "\n";
  }
  

  for(gt::CellIterator c(b.TheGrid()); !c.IsDone(); ++c) {
    cout  << "bucket " << c.index() << ": " << b.get(*c).size() << " entries, "
	  << "neighborhood 1: " << b.get_neighborhood(*c, 1).size() << " entries.\n";
  }


}
