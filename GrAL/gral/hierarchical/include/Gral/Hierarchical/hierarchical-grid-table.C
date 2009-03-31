#ifndef GRAL_GB_HIERARCHICAL_GRID_TABLE_C
#define GRAL_GB_HIERARCHICAL_GRID_TABLE_C


/* ------------------------------------------------------------

    Copyright (C) 2001 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include "Gral/Hierarchical/hierarchical-grid-table.h"
#include "Utility/pre-post-conditions.h"

namespace GrAL {

namespace hierarchical {

  template<class HGRID, class GE>
  hier_grid_table<HGRID,GE>::hier_grid_table() {}

  template<class HGRID, class GE>
  hier_grid_table<HGRID,GE>::hier_grid_table(typename hier_grid_table<HGRID,GE>::hier_grid_type const& gg) 
    : base(&gg),  g(gg)
  {
    init(*g);
  }

  template<class HGRID, class GE>
  hier_grid_table<HGRID,GE>::hier_grid_table(ref_ptr<typename hier_grid_table<HGRID,GE>::hier_grid_type const> gg) 
    : base(&(*gg)), g(gg)
  {
    init(*g);
  }

  template<class HGRID, class GE>
  template<class T>
  hier_grid_table<HGRID,GE>::hier_grid_table(typename hier_grid_table<HGRID,GE>::hier_grid_type const& gg,
					     T const& t)
    : base(&gg),  g(gg)
  {
    init(*g, t);
  }

  template<class HGRID, class GE>
  template<class T>
  hier_grid_table<HGRID,GE>::hier_grid_table(ref_ptr<typename hier_grid_table<HGRID,GE>::hier_grid_type const> gg,
					     T const& t)
    : base(& (*gg)),  g(gg)
  {
    init(*g, t);
  }


  template<class HGRID, class GE>
  void hier_grid_table<HGRID,GE>::set_grid(typename hier_grid_table<HGRID,GE>::hier_grid_type const& gg) 
  {
    if(g != 0)
      clear();
    g = ref_ptr<hier_grid_type const>(gg);
    base::set_grid(&gg);
    init(*g);
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
    g.clear();
  }



  template<class HGRID, class GE>
  void hier_grid_table<HGRID,GE>::update()
  {
    if(g->empty())
      entities.clear();
    else {
      if(entities.empty()) {
	add_root_level();
      } 
      while(coarsest_level() > g->coarsest_level())
	add_coarser_level();
      while(coarsest_level() < g->coarsest_level())
	remove_coarsest_level();
      while(finest_level()   < g->finest_level())
	add_finer_level();
      while(finest_level()   > g->finest_level())
	remove_finest_level();
    }
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
  template<class T>
  void hier_grid_table<HGRID,GE>::init(typename hier_grid_table<HGRID,GE>::hier_grid_type const& gg, T const& t)
  {
    entities.init_begin_index(g->table()->begin_index()); 
    for(level_handle lev = g->coarsest_level(); lev <= g->finest_level(); ++lev)
      // FIXME: do not copy large grid entities!
      entities.push_back(grid_entity_type(* g->FlatGrid(lev), t));
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
  typename  hier_grid_table<HGRID,GE>::level_handle
  hier_grid_table<HGRID,GE>::add_root_level()
  {
    REQUIRE_ALWAYS( empty(), "",1);
    entities.push_front(grid_entity_type(* g->FlatGrid(0)));
    return 0;
  }

  template<class HGRID, class GE>
  template<class T>
  typename  hier_grid_table<HGRID,GE>::level_handle
  hier_grid_table<HGRID,GE>::add_root_level(T const& initializer)
  {
    REQUIRE_ALWAYS( empty(), "",1);
    entities.push_front(grid_entity_type(* g->FlatGrid(0), initializer));
    return 0;
  }


  template<class HGRID, class GE>
  template<class T>
  typename  hier_grid_table<HGRID,GE>::level_handle
  hier_grid_table<HGRID,GE>::add_finer_level(T const& initializer)
  {
    REQUIRE_ALWAYS( (finest_level() < g->finest_level()), "",1);
    level_handle newlev = g->next_finer_level(finest_level());
    entities.push_back(grid_entity_type(* g->FlatGrid(newlev), initializer));
    return newlev;
  }

  template<class HGRID, class GE>
  template<class T>
  typename  hier_grid_table<HGRID,GE>::level_handle
  hier_grid_table<HGRID,GE>::add_coarser_level(T const& initializer)
  {
    REQUIRE_ALWAYS( (coarsest_level() > g->coarsest_level()), "",1);
    level_handle newlev = g->next_coarser_level(coarsest_level());
    entities.push_front(grid_entity_type(* g->FlatGrid(newlev), initializer));
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

  template<class HGRID, class GE>
  void hier_grid_table<HGRID,GE>::notifier_assigned  (hier_grid_table<HGRID,GE>::notifier_base const* n)
  {
    REQUIRE_ALWAYS((n == &(*g)), "", 1);
    entities.clear();
    update();
  }
  template<class HGRID, class GE>
  void hier_grid_table<HGRID,GE>::hgrid_level_added  (hier_grid_table<HGRID,GE>::notifier_type const* n, 
						      level_handle added)
  {
    REQUIRE_ALWAYS((n == &(*g)), "", 1);
    update();
  }
  template<class HGRID, class GE>
  void hier_grid_table<HGRID,GE>::hgrid_level_removed(hier_grid_table<HGRID,GE>::notifier_type const* n, 
						      level_handle removed)
  {
    REQUIRE_ALWAYS((n == &(*g)), "", 1);
    update();
  }


} // namespace hierarchical
} // namespace GrAL 


#endif
