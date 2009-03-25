#ifndef GRAL_GB_VIEWS_RESTRICTED_GRID_VIEW_C
#define GRAL_GB_VIEWS_RESTRICTED_GRID_VIEW_C


/* ------------------------------------------------------------

    Copyright (C) 2001 - 2009 Guntram Berti, 
    Contact: guntram AT users DOT berlios DOT de

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include "Gral/Views/restricted-grid-view.h"
#include <queue>

namespace GrAL {

namespace restricted_grid_view {

  template<class CFG>
  void grid_view<CFG>::init_num_cells() const 
  { 
    num_of_cells = 0;
    for(CellIterator c(*this); !c.IsDone(); ++c)
      ++num_of_cells;
  }

  
  template<class CFG>
  void grid_view<CFG>::init_vertices() const {
    if(!vertices_initialized) {
      grid_function<baseVertex, bool> visited(BaseGrid(), false);
      for(CellIterator c(*this); !c.IsDone(); ++c)
	for(typename basegt::VertexOnCellIterator vc(*c); !vc.IsDone(); ++vc)
	  if(! visited(*vc)) {
	    visited[*vc] = true;
	    range.push_back(*vc);
	  }
      vertices_initialized = true;
    }
  }

} // namespace restricted_grid_view
} // namespace GrAL 

#endif
