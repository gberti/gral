#ifndef NMWR_GB_GRAL_TEST_VERTEX_ITERATOR_H
#define NMWR_GB_GRAL_TEST_VERTEX_ITERATOR_H


/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


/*! \file  

    Test of vertex iterators (models of $GrAL VertexIterator).

*/


#include <iostream>
#include "Gral/Base/common-grid-basics.h"


#ifdef NMWR_INCLUDE_TEMPLATE_DEFS
#include "Gral/Test/test-vertex-iterator.C"
#else

namespace GrAL {

/*! \brief Test of vertex iterators

    Models of $GrAL VertexIterator are tested.
    
    \ingroup sequenceiteratortest
*/
template<class G> 
bool test_vertex_iterator(G const& g, ::std::ostream & out);

/*! \brief Test of vertex iterators

    Models of $GrAL VertexIterator are tested.
    
    \ingroup sequenceiteratortest
*/
template<class G, class GT = grid_types<G> >
bool test_vertex_iterator(G const& g, ::std::ostream & out, GT);

} // namespace GrAL 

#endif

#endif
