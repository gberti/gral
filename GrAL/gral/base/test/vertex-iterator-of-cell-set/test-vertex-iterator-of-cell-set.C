/*! \file
    Testing vertex_iterator_of_cell_set<> 
*/

#include "Gral/Iterators/vertex-iterator-of-cell-set.h"

#include "Gral/Grids/Cartesian2D/cartesian-grid2d.h"
#include "Gral/Grids/Cartesian2D/partial-grid-functions.h"

#include "Gral/Test/test-vertex-iterator.h"

#include "Gral/Base/extend-grid-types.h"

// make sure all members are instantiated
typedef grid_types<cartesian2d::CartesianGrid2D> gt;
template class vertex_iterator_of_cell_set<gt::CellIterator>;

int main() {

  gt::grid_type R(2,2);
  typedef vertex_iterator_of_cell_set<gt::CellIterator> markedVertexIterator;

  markedVertexIterator v(R.FirstCell());
  int nv = 0;
  while(! v.IsDone()) {
    ++v;
    ++nv;
  }


  REQUIRE_ALWAYS( nv == R.NumOfVertices(), 
		  "R.NumOfVertices()=" << R.NumOfVertices() << " nv=" << nv, 1);

  typedef xgt<gt, override_VertexIterator<markedVertexIterator> > mygt;
  test_vertex_iterator(R,cout, mygt());
}
