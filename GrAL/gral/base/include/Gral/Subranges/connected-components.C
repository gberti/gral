#ifndef GRAL_GB_CONNECTED_COMPONENTS_C
#define GRAL_GB_CONNECTED_COMPONENTS_C


/* ------------------------------------------------------------

    Copyright (C) 2001 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include "Gral/Subranges/connected-components.h"
#include <queue>

namespace GrAL {
namespace connected_components {

  template<class GT>
  void component<GT>::init() const
  {
    if(num_of_cells == 0) {
      for(CellIterator c(*this); !c.IsDone(); ++c)
	++num_of_cells;
    }
  }


  template<class GT>
  template<class PRED>
  void component_list<GT>::init(PRED inside) const 
  {
    if(! initialized) {
      comps.init(*g,-1);
      size_type current_component = 0;
      for(CellIterator c(TheGrid()); !c.IsDone(); ++c) {   
	if(inside(*c) && comps(*c)  == -1 ) { // new component: BFS-visit all cells reachable from c
	  std::queue<cell_handle> Q;
	  Q.push(c.handle());
	  comps[*c] = current_component;
	  germs       .push_back(*c); // <=> germs[current_component] = *c;
	  num_of_cells.push_back(1);  // <=> num_of_cells[current_component] = 1; 
	  while(! Q.empty()) {
	    Cell d(TheGrid(), Q.front());
	    Q.pop();
	    for(CellOnCellIterator cd(d); ! cd.IsDone(); ++cd) {
	      if(inside(*cd) && comps(*cd) == -1) { // not yet visited
		comps[*cd] = current_component;
		num_of_cells[current_component]++;
		Q.push(cd.handle());
	      }
	    }
	  }
	  current_component++;
	} // new component
      } // for(CellIterator c ... )
      num_of_components = current_component;
      initialized = true;
    }
  }

} // namespace connected_components
} // namespace GrAL 

#endif
