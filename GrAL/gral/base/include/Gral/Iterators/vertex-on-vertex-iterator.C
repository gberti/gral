#ifndef GRAL_GB_ITERATORS_VERTEX_ON_VERTEX_ITERATOR_C
#define GRAL_GB_ITERATORS_VERTEX_ON_VERTEX_ITERATOR_C

// $LICENSE_NEC_2004

#include "Gral/Iterators/vertex-on-vertex-iterator.h"

namespace GrAL {

template<class G, class GT>
std::map<G const*, ref_ptr<typename  vertex_on_vertex_iterator<G,GT>::vov_table> >
vertex_on_vertex_iterator<G,GT>::ctxt;

} // namespace GrAL 

#endif
