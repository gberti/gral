#ifndef NMWR_GB_GRAL_TEST_CELL_ON_CELL_ITERATOR_H
#define NMWR_GB_GRAL_TEST_CELL_ON_CELL_ITERATOR_H

// $LICENSE

/*! \file  

    Test of cell-on-cell iterators (models of $GrAL CellOnCellIterator).

*/


#include <iostream>

#ifdef NMWR_INCLUDE_TEMPLATE_DEFS
#include "Gral/Test/test-cell-on-cell-iterator.C"
#else

/*! \brief Test a cell-on-cell iterator
 
    Test a model of $GrAL CellOnCellIterator

    \ingroup incidenceiteratortests
 */
template<class G>
bool test_cell_on_cell_iterator(G const& g, std::ostream & out);

/*! \brief Test a cell-on-cell iterator
 
    Test a model of $GrAL CellOnCellIterator.

    \ingroup incidenceiteratortests
 */
template<class G, class GT>
bool test_cell_on_cell_iterator(G const& g, std::ostream & out, class GT);

#endif

#endif
