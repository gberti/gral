/*! \file
    Testing facet_iterator_of_cell_set<> 
*/

#include "Gral/Iterators/facet-iterator-of-cell-set.h"

#include "Gral/Grids/Cartesian2D/cartesian-grid2d.h"
#include "Gral/Grids/Cartesian2D/partial-grid-functions.h"

#include "Gral/Test/test-facet-iterator.h"
#include "Gral/Base/extend-grid-types.h"

// make sure all members are instantiated
typedef grid_types<cartesian2d::CartesianGrid2D> gt;
template class facet_iterator_of_cell_set<gt::CellIterator>;


int main() {

  gt::grid_type R(2,2);
  typedef facet_iterator_of_cell_set<gt::CellIterator> markedFacetIterator;

  markedFacetIterator v(R.FirstCell());
  int nf = 0;
  while(! v.IsDone()) {
    ++v;
    ++nf;
  }


  REQUIRE_ALWAYS( nf == R.NumOfFacets(), 
		  "R.NumOfFacets()=" << R.NumOfFacets() << " nf=" << nf, 1);


   typedef xgt<gt, override_FacetIterator<markedFacetIterator> > mygt;
   test_facet_iterator(R,cout, mygt());
}
