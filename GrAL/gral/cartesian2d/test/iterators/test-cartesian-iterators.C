#include <iostream>
#include <fstream>

#include "Gral/Grids/Cartesian2D/all.h"

#include "Gral/Test/test-iterators.h"

#include "Utility/pre-post-conditions.h"

int main() {
  using namespace std;
  using namespace cartesian2d;

  checkgt<grid_types<cartesian2d::CartesianGrid2D> >(cout);

  REQUIRE_ALWAYS( RegGrid2D::BeginArchetype()->NumOfVertices() == 4, "", 1);

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
      *out << endl;

      test_vertex_iterator(R,*out);
      test_edge_iterator  (R,*out);
      test_cell_iterator  (R,*out);
      test_face_iterator  (R,*out);
      test_facet_iterator  (R,*out);

      test_vertex_on_vertex_iterator(R,*out);
      test_cell_on_vertex_iterator  (R,*out);

      test_vertex_on_edge_iterator (R,*out);
      test_vertex_on_facet_iterator(R,*out);
      test_vertex_on_face_iterator (R,*out);
      test_vertex_on_cell_iterator (R,*out);

      test_edge_on_cell_iterator  (R,*out);
      test_edge_on_face_iterator  (R,*out);
      test_cell_on_cell_iterator  (R,*out);

    }
  }
  delete out;
  return 0;
}
