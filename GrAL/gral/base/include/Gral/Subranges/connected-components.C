#ifndef GRAL_GB_CONNECTED_COMPONENTS_C
#define GRAL_GB_CONNECTED_COMPONENTS_C

// $LICENSE_NEC

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
      int current_component = 0;
      for(CellIterator c(TheGrid()); !c.IsDone(); ++c) {   
	if(inside(*c) && comps(*c)  == -1 ) { // new component: BFS-visit all cells reachable from c
	  /*
	  //component<GT, comp_table_type> Comp(*c, comps, -1);
	  germs       .push_back(*c); // <=> germs[current_component] = *c;
	  num_of_cells.push_back(0); 
	  component<GT> Comp(*c);
	  for(typename component<GT>::CellIterator cc(Comp); ! cc.IsDone(); ++cc) {
	    comps[*cc] = current_component;
	    num_of_cells[current_component]++;
	  }
	  */

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
