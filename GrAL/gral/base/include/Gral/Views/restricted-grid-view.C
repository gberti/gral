#ifndef GRAL_GB_VIEWS_RESTRICTED_GRID_VIEW_C
#define GRAL_GB_VIEWS_RESTRICTED_GRID_VIEW_C

// $LICENSE_NEC

#include "Gral/Views/restricted-grid-view.h"
#include <queue>

namespace GrAL {

namespace restricted_grid_view {

  template<class GRID, class INSIDE_PRED, class GT>
  void grid_view<GRID, INSIDE_PRED, GT>::init_num_cells() const 
  { 
    num_of_cells = 0;
    for(CellIterator c(*this); !c.IsDone(); ++c)
      ++num_of_cells;
  }

  
  template<class GRID, class INSIDE_PRED, class GT>
  void grid_view<GRID, INSIDE_PRED, GT>::init_vertices() const {
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
