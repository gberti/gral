#ifndef NMWR_GB_GRAL_TEST_EDGE_ON_FACET_ITERATOR_H
#define NMWR_GB_GRAL_TEST_EDGE_ON_FACET_ITERATOR_H


/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


/*! \file  

    Test of edge-on-facet iterators (models of $GrAL EdgeOnFacetIterator).

*/

#include <iostream>



#ifdef NMWR_INCLUDE_TEMPLATE_DEFS
#include "Gral/Test/test-edge-on-facet-iterator.C"

#else

namespace GrAL {

/*! \brief Test a edge-on-facet iterator
 
    Test a model of $GrAL EdgeOnFacetIterator

    \ingroup incidenceiteratortests
 */
template<class G>
extern bool test_edge_on_facet_iterator(G const& g, ::std::ostream & out);


/*! \brief Test a edge-on-facet iterator
 
    Test a model of $GrAL EdgeOnFacetIterator

    \ingroup incidenceiteratortests
 */
template<class G, class GT>
extern bool test_edge_on_facet_iterator(G const& g, ::std::ostream & out, GT);

} // namespace GrAL 

#endif

#endif
