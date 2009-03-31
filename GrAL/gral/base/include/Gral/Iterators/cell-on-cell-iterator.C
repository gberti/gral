#ifndef GRAL_GB_BASE_CELL_ON_CELL_ITERATOR_C
#define GRAL_GB_BASE_CELL_ON_CELL_ITERATOR_C


/* ------------------------------------------------------------

    Copyright (C) 2001 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include "Gral/Iterators/cell-on-cell-iterator.h"
#include "Gral/Algorithms/cell-neighbor-search.h"

namespace GrAL {

  template<class G, class NBTABLE, class GT>
  typename cell_on_cell_iterator<G,NBTABLE,GT>::context_table
  * cell_on_cell_iterator<G,NBTABLE,GT>::ctxt;

  template<class G, class NBTABLE, class GT>
  void cell_on_cell_iterator<G,NBTABLE,GT>::init()
  {
    static int count = 0;
    if(count == 0) {
      ++count;
      ctxt = new context_table;
    }
  }

  template<class G, class NBTABLE, class GT>
  void cell_on_cell_iterator<G,NBTABLE,GT>::init(G const& g_)
  {
    init();
    remove(g_);
    ref_ptr<NBTABLE> nbs = new_ref_ptr(new NBTABLE(g_));
    CalculateNeighborCells(* nbs, g_);
    (*ctxt)[&g_] = nbs;
  }

  template<class G, class NBTABLE, class GT>
  void cell_on_cell_iterator<G,NBTABLE,GT>::remove(G const& g_)
  {
    if(ctxt != 0) {
      typename context_table::iterator it = ctxt->find(&g_);
      if(it != ctxt->end()) {
	//it->second = 0;
	(*ctxt)[&g_] = ref_ptr<NBTABLE const>(0);
	ctxt->erase(it);
      }
    }
  }


  template<class G, class NBTABLE, class GT>
  void cell_on_cell_iterator<G,NBTABLE,GT>::map_nb_table(grid_type const& g_, NBTABLE const& nbs_) 
  { 
    init();
    (*ctxt)[&g_] = const_ref_to_ref_ptr(nbs_);
  }

} // namespace GrAL 

#endif
