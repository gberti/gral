#ifndef NMWR_GB_GRAL_TEST_VERTEX_ITERATOR_H
#define NMWR_GB_GRAL_TEST_VERTEX_ITERATOR_H

// $LICENSE

/*! \file  

    Test of vertex iterators (models of $GrAL VertexIterator).

*/


#include <iostream>
#include "Gral/Base/common-grid-basics.h"


#ifdef NMWR_INCLUDE_TEMPLATE_DEFS
#include "Gral/Test/test-vertex-iterator.C"
#else

/*! \brief Test of vertex iterators

    Models of $GrAL VertexIterator are tested.
    
    \ingroup sequenceiteratortest
*/
template<class G> 
bool test_vertex_iterator(G const& g, std::ostream & out);

/*! \brief Test of vertex iterators

    Models of $GrAL VertexIterator are tested.
    
    \ingroup sequenceiteratortest
*/
template<class G, class GT = grid_types<G> >
bool test_vertex_iterator(G const& g, std::ostream & out, GT);

#endif

#endif
