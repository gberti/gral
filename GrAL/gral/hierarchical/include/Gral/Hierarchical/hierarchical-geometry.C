#ifndef GRAL_GB_HIERARCHICAL_GEOMETRY_C
#define GRAL_GB_HIERARCHICAL_GEOMETRY_C

// $LICENSE_NEC

#include "Gral/Hierarchical/hierarchical-geometry.h"

namespace hierarchical {

  template<class HGRID, class FLATGEOM>
  hier_geometry<HGRID, FLATGEOM>::hier_geometry() {}

  template<class HGRID, class FLATGEOM>
  hier_geometry<HGRID, FLATGEOM>::hier_geometry(typename hier_geometry<HGRID, FLATGEOM>::grid_type const& g)
    : geoms(g) 
  {
    connect(&g);
  }

 template<class HGRID, class FLATGEOM>
  hier_geometry<HGRID, FLATGEOM>::hier_geometry(ref_ptr<typename hier_geometry<HGRID, FLATGEOM>::grid_type const> g)
    : geoms(g) 
 {
   connect(&(*g));
 }

  template<class HGRID, class FLATGEOM>
  void hier_geometry<HGRID, FLATGEOM>::set_grid(typename hier_geometry<HGRID, FLATGEOM>::grid_type const& g)
  {
    disconnect();
    geoms.set_grid(g);
    connect(&g);
  }

  template<class HGRID, class FLATGEOM>
  void hier_geometry<HGRID, FLATGEOM>::set_grid(ref_ptr<typename hier_geometry<HGRID, FLATGEOM>::grid_type const> g)
  {
    disconnect();
    geoms.set_grid(g);
    connect(& (*g));
  }

  /*
  template<class HGRID, class FLATGEOM>
  void hier_geometry<HGRID, FLATGEOM>::init    (typename hier_geometry<HGRID, FLATGEOM>::grid_type const& g)
  {
    geoms.init(g);
  }
  */

  template<class HGRID, class FLATGEOM>
  void hier_geometry<HGRID, FLATGEOM>::clear() 
  { 
    geoms.clear();
    disconnect();
  }
  
  template<class HGRID, class FLATGEOM>
  void hier_geometry<HGRID, FLATGEOM>::update()
  { 
    geoms.update();
  }

  template<class HGRID, class FLATGEOM>
  typename hier_geometry<HGRID, FLATGEOM>::level_handle
  hier_geometry<HGRID, FLATGEOM>::add_coarser_level()
  {
    return geoms.add_coarser_level();
  }
  template<class HGRID, class FLATGEOM>
  typename hier_geometry<HGRID, FLATGEOM>::level_handle
  hier_geometry<HGRID, FLATGEOM>::add_finer_level()
  {
    return geoms.add_finer_level();
  }

  template<class HGRID, class FLATGEOM>
  void
  hier_geometry<HGRID, FLATGEOM>::remove_coarsest_level()
  {
    geoms.remove_coarsest_level();
  }

  template<class HGRID, class FLATGEOM>
  void
  hier_geometry<HGRID, FLATGEOM>::remove_finest_level()
  {
    geoms.remove_finest_level();
  }

  // work is done by table ... but hier_geometry<> could do more, eg interpolating / projecting values.

  template<class HGRID, class FLATGEOM>
  void hier_geometry<HGRID, FLATGEOM>::notifier_assigned  
  (hier_geometry<HGRID, FLATGEOM>::notifier_base const* n)
  {}

  template<class HGRID, class FLATGEOM>
  void hier_geometry<HGRID, FLATGEOM>::hgrid_level_added
  (hier_geometry<HGRID, FLATGEOM>::notifier_type const* n, level_handle added)
  {}

  template<class HGRID, class FLATGEOM>
  void hier_geometry<HGRID, FLATGEOM>::hgrid_level_removed
  (hier_geometry<HGRID, FLATGEOM>::notifier_type const* n, level_handle removed)
  {}
 

} // namespace hierarchical

#endif
