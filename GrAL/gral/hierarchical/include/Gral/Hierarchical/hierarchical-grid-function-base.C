#ifndef GRAL_GB_HIERARCHICAL_GRID_FUNCTION_BASE_C
#define GRAL_GB_HIERARCHICAL_GRID_FUNCTION_BASE_C

// $LICENSE_NEC

#include "Gral/Hierarchical/hierarchical-grid-function-base.h"

namespace GrAL {

namespace hierarchical {

  template<class HGRID, class GF> 
  void hier_grid_function_base<HGRID,GF>::
  init(typename hier_grid_function_base<HGRID,GF>::hier_grid_type const& gg,
       typename hier_grid_function_base<HGRID,GF>::value_type const& t)
  {
    set_grid (gg);
    set_value(t);
  }

  template<class HGRID, class GF> 
  void hier_grid_function_base<HGRID,GF>::clear() { gfs.clear();}

  template<class HGRID, class GF> 
  void hier_grid_function_base<HGRID,GF>::update() { 
    gfs.update();
    // FIXME: only for updated levels.
    if(default_val_set)
      set_value(default_val); 
  }

  template<class HGRID, class GF> 
  void hier_grid_function_base<HGRID,GF>::
  set_grid(typename hier_grid_function_base<HGRID,GF>::hier_grid_type const& gg)
  {
    clear();
    gfs.set_grid(gg);
    if(default_val_set)
      set_value(default_val);
  }

  template<class HGRID, class GF> 
  void hier_grid_function_base<HGRID,GF>::
  set_value(typename hier_grid_function_base<HGRID,GF>::value_type t)
  {
    default_val_set = true;
    default_val     = t;
    for(level_handle lev = TheGrid()->coarsest_level(); lev <= TheGrid()->finest_level(); ++lev)
      gfs[lev].set_value(t);
  }


  template<class HGRID, class GF> 
  typename hier_grid_function_base<HGRID,GF>::level_handle
  hier_grid_function_base<HGRID,GF>::add_finer_level() 
  {
    return gfs.add_finer_level();
  }


  template<class HGRID, class GF> 
  typename hier_grid_function_base<HGRID,GF>::level_handle
  hier_grid_function_base<HGRID,GF>::
  add_finer_level(typename hier_grid_function_base<HGRID,GF>::value_type const& t)
  {
    level_handle newlev = add_finer_level();
    gfs[newlev].set_value(t);
    return newlev;
  } 


  template<class HGRID, class GF> 
  typename hier_grid_function_base<HGRID,GF>::level_handle
  hier_grid_function_base<HGRID,GF>::add_coarser_level() 
  {
    return gfs.add_coarser_level();
  }


  template<class HGRID, class GF> 
  typename hier_grid_function_base<HGRID,GF>::level_handle
  hier_grid_function_base<HGRID,GF>::
  add_coarser_level(typename hier_grid_function_base<HGRID,GF>::value_type const& t)
  {
    level_handle newlev = add_coarser_level();
    gfs[newlev].set_value(t);
    return newlev;
  } 


  template<class HGRID, class GF> 
  void 
  hier_grid_function_base<HGRID,GF>::remove_coarsest_level()
  {
    return gfs.remove_coarsest_level();
  }


  template<class HGRID, class GF> 
  void 
  hier_grid_function_base<HGRID,GF>::remove_finest_level()
  {
    return gfs.remove_finest_level();
  }

  // Work done by hier_grid_table ...
  template<class HGRID, class GF> 
  void 
  hier_grid_function_base<HGRID,GF>::notifier_assigned(hier_grid_function_base<HGRID,GF>::notifier_base const* n)
  {
    // FIXME: only for updated levels...how to know about them?
    if(default_val_set)
      set_value(default_val); 
  }


  template<class HGRID, class GF> 
  void 
  hier_grid_function_base<HGRID,GF>::hgrid_level_added  
  (hier_grid_function_base<HGRID,GF>::notifier_type const* n, level_handle added) 
  {
    if(default_val_set)
      gfs[added].set_value(default_val);
  }

  template<class HGRID, class GF> 
  void 
  hier_grid_function_base<HGRID,GF>::hgrid_level_removed
  (hier_grid_function_base<HGRID,GF>::notifier_type const* n, level_handle added) 
  {}


} // namespace hierarchical
} // namespace GrAL 

#endif
