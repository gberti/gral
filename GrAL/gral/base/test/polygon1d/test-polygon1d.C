#include "Gral/Base/polygon.h"
#include "Gral/Test/test-iterators.h"

#include <iostream>


int main() {
  using namespace std;

  polygon1d::polygon p(3);
  typedef grid_types<polygon1d::polygon> gt;

  ostream* out = &cout;
  test_vertex_iterator(p, *out);
  test_cell_iterator  (p, *out);
  test_vertex_on_cell_iterator(p, *out);

  checkgt<gt>(*out);
}
