

/* ------------------------------------------------------------

    Copyright (C) 2001 - 2009 Guntram Berti, 
    Contact: guntram AT users DOT berlios DOT de

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


/*! \file
 
    Testing \c vertex_iterator_of_cell_set
*/

#include "Gral/Iterators/vertex-iterator-of-cell-set.h"

#include "Gral/Grids/Cartesian2D/cartesian-grid2d.h"
#include "Gral/Grids/Cartesian2D/partial-grid-functions.h"

#include "Gral/Test/test-vertex-iterator.h"

#include "Gral/Base/extend-grid-types.h"

// make sure all members are instantiated

using namespace GrAL;

//namespace GrAL {
  typedef grid_types<cartesian2d::CartesianGrid2D> gt;
  template class vertex_iterator_of_cell_set<gt::CellIterator>;
//}

int main() {
  // using namespace GrAL;

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
  test_vertex_iterator(R, std::cout, mygt());
}
