#ifndef NMWR_GB_GRAL_TEST_CELL_ON_VERTEX_ITERATOR_H
#define NMWR_GB_GRAL_TEST_CELL_ON_VERTEX_ITERATOR_H

// $LICENSE

/*! \file  

    Test of cell-on-vertex iterators (models of $GrAL CellOnVertexIterator).

*/

#include <iostream>


#ifdef NMWR_INCLUDE_TEMPLATE_DEFS
#include "Gral/Test/test-cell-on-vertex-iterator.C"

#else

/*! \brief Test a cell-on-vertex iterator
 
    Test a model of $GrAL CellOnVertexIterator

    \ingroup incidenceiteratortests
 */
template<class G>
bool test_cell_on_vertex_iterator(G const& g, std::ostream & out);

/*! \brief Test a cell-on-vertex iterator
 
    Test a model of $GrAL CellOnVertexIterator

    \ingroup incidenceiteratortests
 */template<class G, class GT>
bool test_cell_on_vertex_iterator(G const& g, std::ostream & out, GT);

#endif

#endif
