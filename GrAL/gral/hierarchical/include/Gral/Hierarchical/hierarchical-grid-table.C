#ifndef GRAL_GB_HIERARCHICAL_GRID_TABLE_C
#define GRAL_GB_HIERARCHICAL_GRID_TABLE_C

// $LICENSE_NEC

#include "Gral/Hierarchical/hierarchical-grid-table.h"
#include "Utility/pre-post-conditions.h"

namespace hierarchical {

  template<class HGRID, class GE>
  hier_grid_table<HGRID,GE>::hier_grid_table() {}

  template<class HGRID, class GE>
  hier_grid_table<HGRID,GE>::hier_grid_table(typename hier_grid_table<HGRID,GE>::hier_grid_type const& gg) 
    : g(&gg)
  {
    init(*g);
  }

  template<class HGRID, class GE>
  hier_grid_table<HGRID,GE>::hier_grid_table(ref_ptr<typename hier_grid_table<HGRID,GE>::hier_grid_type const> gg) 
    : g(gg)
  {
    init(*g);
  }

  template<class HGRID, class GE>
  void hier_grid_table<HGRID,GE>::set_grid(typename hier_grid_table<HGRID,GE>::hier_grid_type const& gg) 
  {
    if(g != 0)
      clear();
    g = &gg;
    init(gg);
  }

  template<class HGRID, class GE>
  void hier_grid_table<HGRID,GE>::set_grid(ref_ptr<typename hier_grid_table<HGRID,GE>::hier_grid_type const> gg) 
  {
    if(g != 0)
      clear();
    g = gg;
    init(*g);
  }

  template<class HGRID, class GE>
  void hier_grid_table<HGRID,GE>::clear()
  {
    entities.clear();
    g = static_cast<hier_grid_type const*>(0);
  }



  template<class HGRID, class GE>
  void hier_grid_table<HGRID,GE>::update()
  {
    while(coarsest_level() > g->coarsest_level())
      add_coarser_level();
    while(coarsest_level() < g->coarsest_level())
      remove_coarsest_level();
    while(finest_level()   < g->finest_level())
      add_finer_level();
    while(finest_level()   > g->finest_level())
      remove_finest_level();

  }

  template<class HGRID, class GE>
  void hier_grid_table<HGRID,GE>::init(typename hier_grid_table<HGRID,GE>::hier_grid_type const& gg) 
  {
    /* this is too low level (and specific to bivector<>): we should really have some high-level option
       to copy the structure of the level hierarchy of g, e.g.
       entities.init(g->grids->structure(), entity_initializer(&hier_grid_type::FlatGrid, g));
       where entitiy_initializer is e.g.
       struct entity_initializer { 
          ref_ptr<const hier_grid_type> g; 
	  ref_ptr<const flat_grid_type> operator()(level_handle lev) const { return g->FlatGrid(lev);}
       }; 
    */ 
    entities.init_begin_index(g->table()->begin_index()); 
    for(level_handle lev = g->coarsest_level(); lev <= g->finest_level(); ++lev)
      // FIXME: do not copy large grid entities!
      entities.push_back(grid_entity_type(* g->FlatGrid(lev)));
  }

  template<class HGRID, class GE>
  typename  hier_grid_table<HGRID,GE>::level_handle
  hier_grid_table<HGRID,GE>::add_finer_level()
  {
    REQUIRE_ALWAYS( (finest_level() < g->finest_level()), "",1);
    level_handle newlev = g->next_finer_level(finest_level());
    entities.push_back(grid_entity_type(* g->FlatGrid(newlev)));
    return newlev;
  }

  template<class HGRID, class GE>
  typename  hier_grid_table<HGRID,GE>::level_handle
  hier_grid_table<HGRID,GE>::add_coarser_level()
  {
    REQUIRE_ALWAYS( (coarsest_level() > g->coarsest_level()), "",1);
    level_handle newlev = g->next_coarser_level(coarsest_level());
    entities.push_front(grid_entity_type(* g->FlatGrid(newlev)));
    return newlev;
  }


  template<class HGRID, class GE>
  void hier_grid_table<HGRID,GE>::remove_finest_level()
  {
    REQUIRE_ALWAYS( (g->empty() || (finest_level() > g->finest_level())), "",1);
    entities.pop_back();
  }
  template<class HGRID, class GE>
  void hier_grid_table<HGRID,GE>::remove_coarsest_level()
  {
    REQUIRE_ALWAYS( (g->empty() || (coarsest_level() < g->coarsest_level())), "",1);
    entities.pop_front();
  }



} // namespace hierarchical

#endif
