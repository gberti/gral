#ifndef GRAL_GB_VIEWS_RESTRICTED_COMPONENT_VIEW_C
#define GRAL_GB_VIEWS_RESTRICTED_COMPONENT_VIEW_C

// $LICENSE_NEC

#include "Gral/Views/restricted-component-view.h"
#include <queue>

namespace GrAL {

namespace restricted_grid_component_view {


  template<class GRID, class INSIDE_PRED, class GT>
  void grid_view<GRID, INSIDE_PRED, GT>::init
  (typename grid_view<GRID, INSIDE_PRED, GT>::grid_type const& gg,
   typename grid_view<GRID, INSIDE_PRED, GT>::pred_type        ins,
   typename grid_view<GRID, INSIDE_PRED, GT>::baseCell  const& grm) 
  {
    clear();
    g = ref_ptr<grid_type const>(gg); 
    range.init(gg);
    inside = ins;
    germ   = grm;
  }

  template<class GRID, class INSIDE_PRED, class GT>
  void grid_view<GRID, INSIDE_PRED, GT>::clear()
  {
    g = ref_ptr<const grid_type>();
    range.clear();
    cells_initialized = false;
    vertices_initialized = false;
  }


  template<class GRID, class INSIDE_PRED, class GT>
  void grid_view<GRID, INSIDE_PRED, GT>::init_cells() const {
    if(! cells_initialized) {
      std::queue<baseCell>  front;
      partial_grid_function<baseCell, bool> found(*g,false);
      front.push(germ);
      found[germ] = true;
      while(! front.empty()) {
	baseCell c = front.front();
	front.pop();
	for(baseCellOnCellIterator cc(c); ! cc.IsDone(); ++cc)
	  if (! found(*cc) && inside(*cc) ) {
	    found[*cc] = true;
	    front.push(*cc);
	  }
      }
      for(typename partial_grid_function<baseCell, bool>::CellIterator c = found.FirstCell(); !c.IsDone(); ++c)
	range.append_cell(*c);

      cells_initialized = true;
    }
  }
  
  template<class GRID, class INSIDE_PRED, class GT>
  void grid_view<GRID, INSIDE_PRED, GT>::init_vertices() const {
    if(!vertices_initialized) {
      if(! cells_initialized)
	init_cells();
      // range.init();
      vertices_initialized = true;
    }
  }

} // namespace restricted_grid_component_view
} // namespace GrAL 

#endif
