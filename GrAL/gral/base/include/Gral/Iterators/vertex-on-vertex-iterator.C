#ifndef GRAL_GB_ITERATORS_VERTEX_ON_VERTEX_ITERATOR_C
#define GRAL_GB_ITERATORS_VERTEX_ON_VERTEX_ITERATOR_C


/* ------------------------------------------------------------

    Copyright (C) 2004 - 2009 Guntram Berti, 
    Contact: guntram AT users DOT berlios DOT de

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include "Gral/Iterators/vertex-on-vertex-iterator.h"

namespace GrAL {

template<class G, class GT>
std::map<G const*, ref_ptr<typename  vertex_on_vertex_iterator<G,GT>::vov_table> >
vertex_on_vertex_iterator<G,GT>::ctxt;

} // namespace GrAL 

#endif
