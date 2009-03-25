#ifndef NMWR_GB_GRAL_TEST_EDGE_ITERATOR_H
#define NMWR_GB_GRAL_TEST_EDGE_ITERATOR_H


/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti, 
    Contact: guntram AT users DOT berlios DOT de

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


/*! \file  

    Test of edge iterators (models of $GrAL EdgeIterator).

*/

#include <iostream>



#ifdef NMWR_INCLUDE_TEMPLATE_DEFS
#include "Gral/Test/test-edge-iterator.C"

#else

namespace GrAL {

/*! \brief Test of edge iterators

    Models of $GrAL EdgeIterator are tested.
    
    \ingroup sequenceiteratortest
*/
template<class G>
bool test_edge_iterator(G const& g, ::std::ostream & out);

/*! \brief Test of edge iterators

    Models of $GrAL EdgeIterator are tested.
    
    \ingroup sequenceiteratortest
*/
template<class G, class GT>
bool test_edge_iterator(G const& g, ::std::ostream & out, GT);

} // namespace GrAL 

#endif

#endif
