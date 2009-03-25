#ifndef NMWR_GB_GRAL_TEST_VERTEX_ON_VERTEX_ITERATOR_H
#define NMWR_GB_GRAL_TEST_VERTEX_ON_VERTEX_ITERATOR_H


/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti, 
    Contact: guntram AT users DOT berlios DOT de

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include <iostream>


#ifdef NMWR_INCLUDE_TEMPLATE_DEFS
#include "Gral/Test/test-vertex-on-vertex-iterator.C"
#else

namespace GrAL {

template<class G>
bool test_vertex_on_vertex_iterator(G const& g, ::std::ostream & out);

} // namespace GrAL 

#endif

#endif
