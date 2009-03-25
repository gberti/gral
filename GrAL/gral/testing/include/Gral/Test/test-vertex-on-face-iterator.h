#ifndef NMWR_GB_GRAL_TEST_VERTEX_ON_FACE_ITERATOR_H
#define NMWR_GB_GRAL_TEST_VERTEX_ON_FACE_ITERATOR_H


/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti, 
    Contact: guntram AT users DOT berlios DOT de

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


/*! \file  

    Test of vertex-on-face iterators (models of $GrAL VertexOnFaceIterator).
*/

#include <iostream>



#ifdef NMWR_INCLUDE_TEMPLATE_DEFS
#include "Gral/Test/test-vertex-on-face-iterator.C"
#else

namespace GrAL {

/*! \brief Test a vertex-on-face iterator
 
    Test a model of $GrAL VertexOnFaceIterator

    \ingroup incidenceiteratortests
*/
template<class G>
bool test_vertex_on_face_iterator(G const& g, ::std::ostream & out);

/*! \brief Test a vertex-on-face iterator
 
    Test a model of $GrAL VertexOnFaceIterator

    \ingroup incidenceiteratortests
*/
template<class G, class GT>
bool test_vertex_on_face_iterator(G const& g, ::std::ostream & out, GT);

} // namespace GrAL 

#endif

#endif
