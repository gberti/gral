#include <iostream>
#include <fstream>

#include "Gral/Grids/Cartesian3D/all.h"

#include "Gral/Test/test-iterators.h"

#include "Utility/pre-post-conditions.h"

int main() {

  ofstream* out = new ofstream("vertex-iterator.out");
  int NX = 4, NY = 4, NZ = 4;
  for(int nx = 2; nx < NX; ++nx) {
    for(int ny = 2; ny < NY; ++ny) {
      for(int nz = 2; nz < NZ; ++nz) {
	CartesianGrid3D R(nx,ny,nz);
	*out << "CartesianGrid2D(" << nx << ',' << ny << ',' << nz << ")\n"
	     << R.NumOfCells()    << " cells\n"
	     << R.NumOfVertices() << " vertices\n";
	// REQUIRE_ALWAYS( (R.NumOfVertices() + R.NumOfCells() == R.NumOfEdges() + 1),
	//                "Euler formula does not hold!\n",1);
	test_vertex_iterator(R,*out);
	test_vertex_on_cell_iterator(R,*out);
	*out << endl;
      }
    }
  }
  delete out;
  return 0;
}
