#ifndef NMWR_GB_GRAL_TEST_FACET_ON_CELL_ITERATOR_H
#define NMWR_GB_GRAL_TEST_FACET_ON_CELL_ITERATOR_H

// $LICENSE

/*! \file  

    Test of facet-on-cell iterators (models of $GrAL FacetOnCellIterator).
*/

#include <iostream>

#ifdef NMWR_INCLUDE_TEMPLATE_DEFS
#include "Gral/Test/test-facet-on-cell-iterator.C"
#else

namespace GrAL {

/*! \brief Test a facet-on-cell iterator
 
    Test a model of $GrAL FacetOnCellIterator

    \ingroup incidenceiteratortests
 */
template<class G>
bool test_facet_on_cell_iterator(G const& g, ::std::ostream & out);

/*! \brief Test a facet-on-cell iterator
 
    Test a model of $GrAL FacetOnCellIterator

    \ingroup incidenceiteratortests
 */
template<class G, class GT>
bool test_facet_on_cell_iterator(G const& g, ::std::ostream & out, GT);

} // namespace GrAL 


#endif

#endif
