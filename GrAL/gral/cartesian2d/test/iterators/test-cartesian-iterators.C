#include <iostream>
#include <fstream>

#include "Gral/Grids/Cartesian2D/all.h"

#include "Gral/Test/test-iterators.h"

#include "Utility/pre-post-conditions.h"

int main() {

  ofstream* out = new ofstream("vertex-iterator.out");
  int NX = 4, NY = 4;
  for(int nx = 1; nx < NX; ++nx) {
    for(int ny = 1; ny < NY; ++ny) {
      RegGrid2D R(nx,ny);
      *out << "RegGrid2D(" << nx << ',' << ny << ")\n"
           << R.NumOfCells()    << " cells\n"
           << R.NumOfEdges()    << " edges\n"
           << R.NumOfVertices() << " vertices\n";
      REQUIRE_ALWAYS( (R.NumOfVertices() + R.NumOfCells() == R.NumOfEdges() + 1),
                      "Euler formula does not hold!\n",1);
      test_vertex_iterator(R,*out);
      test_vertex_on_cell_iterator(R,*out);
      *out << endl;
    }
  }
  delete out;
  return 0;
}
