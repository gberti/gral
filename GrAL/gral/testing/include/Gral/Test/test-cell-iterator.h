#ifndef NMWR_GB_GRAL_TEST_CELL_ITERATOR_H
#define NMWR_GB_GRAL_TEST_CELL_ITERATOR_H

// $LICENSE

/*! \file  

    Test of cell iterators (models of $GrAL CellIterator).

*/

#include <iostream>

#ifdef NMWR_INCLUDE_TEMPLATE_DEFS
#include "Gral/Test/test-cell-iterator.C"
#else

/*! \brief Test of cell iterators

    Models of $GrAL CellIterator are tested.
    
    \ingroup sequenceiteratortest
*/
template<class G>
bool test_cell_iterator(G const& g, std::ostream & out);

#endif



#endif
