#ifndef GRAL_GB_BASE_CELL_ON_CELL_ITERATOR_C
#define GRAL_GB_BASE_CELL_ON_CELL_ITERATOR_C

// $LICENSE_NEC

#include "Gral/Iterators/cell-on-cell-iterator.h"

template<class G, class NBTABLE, class GT>
std::map<G const*, NBTABLE const*>
cell_on_cell_iterator<G,NBTABLE,GT>::ctxt;

#endif
