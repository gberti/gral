#ifndef NMWR_GB_GRAL_TEST_ALL_H
#define NMWR_GB_GRAL_TEST_ALL_H

// $LICENSE

/*! \file  

    Convenience header for including all testing functions.

 */

/*! \defgroup testfunctions Test functions for grid components

    All test functions in this module come in two version: A simple version 
    and a version with an additional \c GT parameter. This parameter is normally
    a generalization of the \c grid_types template and can be used
    as an explicit template parameter to provide additional types to test, for example 
    generic iterators.

    In the example, we assume that the grid type \c MyGrid has no $GraL CellOnVertexIterator
    type implement. Instead, we use a generic cell-on-vertex iterator, which we want to 
    test:
    \code
    #include "Gral/Base/extend-grid-types.h" // xgt, override_xxx
    #include "Gral/Iterators/cell-on-vertex-iterator.h"
    #include "Gral/Test/test-cell-on-vertex-iterator.h"
    #include "MyGrid.h"

    MyGrid  g;
    typedef grid_types<MyGrid> gt;
    typedef cell_on_vertex_iterator<MyGrid> cov_iter;
    typedef xgt<gt, override_CellOnVertexIterator<cov_iter> > gt_with_cov;

    // test the generic cell-on-vertex-iterator
    test_cell_on_vertex_iterator(g, cout, gt_with_cov());
    \endcode
 */

/*! \defgroup gridfunctiontests Tests for grid functions
    \ingroup testfunctions
*/

/*! \defgroup sequenceiteratortest Tests for sequence iterators

    \ingroup testfunctions
*/

/*! \defgroup incidenceiteratortests Tests for incidence iterators
     
     Functions in this module test models of $GrAL IncidenceIterator.

     \todo Implement tests for 
       - $GrAL CellOnEdgeIterator 
       - $GrAL CellOnFaceIterator 
       - $GrAL CellOnFacetIterator 
       - $GrAL FaceOnEdgeIterator 
       - $GrAL FaceOnVertexIterator
       - $GrAL FacetOnEdgeIterator
       - $GrAL FacetOnVertexIterator
  
     \ingroup testfunctions
 */


/*! \defgroup archetypestests Tests for grid archetypes
     \ingroup testfunctions
 */



#include "Gral/Test/test-iterators.h"
#include "Gral/Test/test-grid-functions.h"
#include "Gral/Test/test-partial-grid-functions.h"
#include "Gral/Test/test-archetypes.h"

#endif
