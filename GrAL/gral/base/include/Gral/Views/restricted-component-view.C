#ifndef GRAL_GB_VIEWS_RESTRICTED_COMPONENT_VIEW_C
#define GRAL_GB_VIEWS_RESTRICTED_COMPONENT_VIEW_C

// $LICENSE_NEC

#include "Gral/Views/restricted-component-view.h"
#include <queue>

namespace GrAL {

namespace restricted_grid_component_view {

  template<class GRID, class INSIDE_PRED, class GT>
  grid_view<GRID, INSIDE_PRED, GT>::grid_view
  (typename grid_view<GRID, INSIDE_PRED, GT>::grid_type const& gg,
   typename grid_view<GRID, INSIDE_PRED, GT>::pred_type        ins,
   typename grid_view<GRID, INSIDE_PRED, GT>::baseCell  const& grm) 
      : g(&gg), inside(ins), germs(gg), range(gg),  
	cells_initialized(false),
	vertices_initialized(false) 
    {
      germs.push_back(grm);
    }

  template<class GRID, class INSIDE_PRED, class GT>
  template<class IT>
  grid_view<GRID, INSIDE_PRED, GT>::grid_view
  (typename grid_view<GRID, INSIDE_PRED, GT>::grid_type const& gg,
   typename grid_view<GRID, INSIDE_PRED, GT>::pred_type        ins,
   IT begin_germ, IT end_germ)
      : g(&gg), inside(ins), germs(gg), range(gg),  
	cells_initialized(false),
	vertices_initialized(false) 
    {
      while(begin_germ != end_germ) {
	germs.push_back(*begin_germ);
	++begin_germ;
      }
    }



  template<class GRID, class INSIDE_PRED, class GT>
  void grid_view<GRID, INSIDE_PRED, GT>::init
  (typename grid_view<GRID, INSIDE_PRED, GT>::grid_type const& gg,
   typename grid_view<GRID, INSIDE_PRED, GT>::pred_type        ins,
   typename grid_view<GRID, INSIDE_PRED, GT>::baseCell  const& grm) 
  {
    clear();
    g = ref_ptr<grid_type const>(gg); 
    inside = ins;
    range.init(gg);
    germs.init(gg);
    germs.push_back(grm);
  }


  template<class GRID, class INSIDE_PRED, class GT>
  template<class IT>
  void grid_view<GRID, INSIDE_PRED, GT>::init
  (typename grid_view<GRID, INSIDE_PRED, GT>::grid_type const& gg,
   typename grid_view<GRID, INSIDE_PRED, GT>::pred_type        ins,
   IT begin_germ, IT end_germ)
  {
    clear();
    g = ref_ptr<grid_type const>(gg); 
    inside = ins;
    range.init(gg);
    germs.init(gg);
    while(begin_germ != end_germ) {
      germs.push_back(*begin_germ);
      ++begin_germ;
    }
  }



  template<class GRID, class INSIDE_PRED, class GT>
  void grid_view<GRID, INSIDE_PRED, GT>::clear()
  {
    g = ref_ptr<const grid_type>();
    range.clear();
    germs.clear();
    cells_initialized = false;
    vertices_initialized = false;
  }


  template<class GRID, class INSIDE_PRED, class GT>
  void grid_view<GRID, INSIDE_PRED, GT>::init_cells() const {
    if(! cells_initialized) {
      std::queue<baseCell>  front;
      partial_grid_function<baseCell, bool> found(*g,false);
      typedef grid_types<range_type> rgt;
      for(typename rgt::CellIterator germ(germs); !germ.IsDone(); ++germ) {
	if(inside(*germ)) {
	  front.push(*germ);
	  found[*germ] = true;
	}
      }
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
