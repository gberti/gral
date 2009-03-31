#ifndef NMWR_GB_GRAL_TEST_FACET_ON_CELL_ITERATOR_H
#define NMWR_GB_GRAL_TEST_FACET_ON_CELL_ITERATOR_H


/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


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
