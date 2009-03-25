#ifndef GRAL_GB_ITERATORS_CELL_ON_VERTEX_ITERATOR_C
#define GRAL_GB_ITERATORS_CELL_ON_VERTEX_ITERATOR_C


/* ------------------------------------------------------------

    Copyright (C) 2001 - 2009 Guntram Berti, 
    Contact: guntram AT users DOT berlios DOT de

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include "Gral/Iterators/cell-on-vertex-iterator.h"

namespace GrAL {

  template<class G, class GT>
  typename cell_on_vertex_iterator<G,GT>::context_table
  * cell_on_vertex_iterator<G,GT>::ctxt;

  template<class G, class GT>
  void cell_on_vertex_iterator<G,GT>::init()
  {
    static int count = 0;
    if(count == 0) {
      ++count;
      ctxt = new context_table();
    }
  }

  template<class G, class GT>
  void cell_on_vertex_iterator<G,GT>::init(grid_type const& g) 
  { 
    init();
    remove(g);
    ref_ptr<cov_table> t = new_ref_ptr(new cov_table(g)); // , ref_ptr_base::shared);
    for(typename gt::CellIterator c(g); ! c.IsDone(); ++c)
      for(typename gt::VertexOnCellIterator vc(*c); ! vc.IsDone(); ++vc)
	(*t)[*vc].push_back(c.handle());
    (*ctxt)[&g] = t;
  }

  template<class G, class GT>
  void cell_on_vertex_iterator<G,GT>::remove(grid_type const& g) 
  {
    if(ctxt != 0) {
      typename context_table::iterator it = ctxt->find(&g);
      if(it != ctxt->end()) {
	it->second.clear();
	ctxt->erase(it);
      }
    }
  }

} // namespace GrAL 

#endif
