#ifndef GRAL_GB_BASE_CELL_ON_CELL_ITERATOR_C
#define GRAL_GB_BASE_CELL_ON_CELL_ITERATOR_C

// $LICENSE_NEC

#include "Gral/Iterators/cell-on-cell-iterator.h"
#include "Gral/Algorithms/cell-neighbor-search.h"

namespace GrAL {

template<class G, class NBTABLE, class GT>
std::map<G const*, NBTABLE const*>
cell_on_cell_iterator<G,NBTABLE,GT>::ctxt;

template<class G, class NBTABLE, class GT>
void cell_on_cell_iterator<G,NBTABLE,GT>::init(G const& g_)
{
  NBTABLE * nbs = new NBTABLE(g_);
  CalculateNeighborCells(* nbs, g_);
  ctxt[&g_] = nbs;
}

template<class G, class NBTABLE, class GT>
void cell_on_cell_iterator<G,NBTABLE,GT>::remove(G const& g_)
{
    typename context_table::iterator it = ctxt.find(&g_);
    if(it != ctxt.end()) {
      delete it->second;
      ctxt.erase(it);
    }
}

} // namespace GrAL 

#endif
