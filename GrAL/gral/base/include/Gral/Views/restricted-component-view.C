#ifndef GRAL_GB_VIEWS_RESTRICTED_COMPONENT_VIEW_C
#define GRAL_GB_VIEWS_RESTRICTED_COMPONENT_VIEW_C

// $LICENSE_NEC

#include "Gral/Views/restricted-component-view.h"
#include <queue>

namespace restricted_grid_component_view {

  template<class GRID, class INSIDE_PRED, class GT>
  void grid_view<GRID, INSIDE_PRED, GT>::init() const {
    if(! initialized) {
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
      range.construct(found.FirstCell()); 
      initialized = true;
    }
  }

} // namespace restricted_grid_component_view

#endif
