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
    init(gg);
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
  void hier_grid_table<HGRID,GE>::clear()
  {
    entities.clear();
    g = 0;
  }

  template<class HGRID, class GE>
  void hier_grid_table<HGRID,GE>::init(typename hier_grid_table<HGRID,GE>::hier_grid_type const& gg) 
  {
    // this is to low level: we should really have some high-level option
    // to copy the structure of the level hierarchy of g
    for(level_handle lev = 0; lev <= g->finest_level(); ++lev)
      entities.push_back(GE(g->FlatGrid(lev)));
    for(level_handle lev = -1; lev >= g->coarsest_level(); --lev)
      entities.push_front(GE(g->FlatGrid(lev)));
  }

  template<class HGRID, class GE>
  typename  hier_grid_table<HGRID,GE>::level_handle
  hier_grid_table<HGRID,GE>::add_finer_level()
  {
    REQUIRE_ALWAYS( (finest_level() < g->finest_level()), "",1);
    level_handle newlev = g->next_finer_level(finest_level());
    entities.push_back(value_type(g->FlatGrid(newlev)));
    return newlev;
  }

  template<class HGRID, class GE>
  typename  hier_grid_table<HGRID,GE>::level_handle
  hier_grid_table<HGRID,GE>::add_coarser_level()
  {
    REQUIRE_ALWAYS( (coarsest_level() > g->coarsest_level()), "",1);
    level_handle newlev = g->next_coarser_level(coarsest_level());
    entities.push_front(value_type(g->FlatGrid(newlev)));
    return newlev;
  }


  template<class HGRID, class GE>
  void hier_grid_table<HGRID,GE>::remove_finest_level()
  {
    REQUIRE_ALWAYS( (finest_level() > g->finest_level()), "",1);
    entities.pop_back();
  }
  template<class HGRID, class GE>
  void hier_grid_table<HGRID,GE>::remove_coarsest_level()
  {
    REQUIRE_ALWAYS( (coarsest_level() < g->coarsest_level()), "",1);
    entities.pop_front();
  }



} // namespace hierarchical

#endif
