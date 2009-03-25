

/* ------------------------------------------------------------

    Copyright (C) 2001 - 2009 Guntram Berti, 
    Contact: guntram AT users DOT berlios DOT de

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


/*! \file

    Testing \c facet_iterator_of_cell_set
*/

#include "Gral/Iterators/closure-iterator.h"

#include "Gral/Grids/Cartesian2D/all.h"

#include "Gral/Test/test-vertex-iterator.h"
#include "Gral/Test/test-facet-iterator.h"
#include "Gral/Base/extend-grid-types.h"

// make sure all members are instantiated
using namespace GrAL;

typedef grid_types<cartesian2d::CartesianGrid2D> gt;
template class closure_iterator<gt::CellIterator, gt::FacetOnCellIterator>;
template class closure_iterator<gt::CellIterator, gt::VertexOnCellIterator>;
template class closure_iterator<gt::FacetOnCellIterator, gt::VertexOnFacetIterator>;


int main() {
 

  gt::grid_type R(3,3);

  {
    typedef closure_iterator<gt::CellIterator, gt::FacetOnCellIterator> markedFacetIterator;

    grid_function<gt::Facet, gt::facet_handle> fh(R);
    int nf = 0;
    for(markedFacetIterator f(R.FirstCell()); !f.IsDone(); ++f, ++nf)
      fh[*f] = f.handle();

    for(gt::FacetIterator f(R); !f.IsDone(); ++f)
      REQUIRE_ALWAYS(fh(*f) == f.handle(), "", 1);
    
    REQUIRE_ALWAYS( nf == R.NumOfFacets(), 
		    "R.NumOfFacets()=" << R.NumOfFacets() << " nf=" << nf, 1);
    
    typedef xgt<gt, override_FacetIterator<markedFacetIterator> > mygt;
    test_facet_iterator(R,std::cout, mygt());
  }

  {
    typedef closure_iterator<gt::CellIterator, gt::VertexOnCellIterator> markedVertexIterator;

    grid_function<gt::Vertex, gt::vertex_handle> vh(R);
    int n = 0;
    for(markedVertexIterator v(R.FirstCell()); !v.IsDone(); ++v, ++n)
      vh[*v] = v.handle();

    for(gt::VertexIterator v(R); !v.IsDone(); ++v)
      REQUIRE_ALWAYS(vh(*v) == v.handle(), "", 1);
    
    REQUIRE_ALWAYS( n == R.NumOfVertices(), 
		    "R.NumOfVertices()=" << R.NumOfVertices() << " n=" << n, 1);
    
    typedef xgt<gt, override_VertexIterator<markedVertexIterator> > mygt;
    test_vertex_iterator(R,std::cout, mygt());
  }



}
