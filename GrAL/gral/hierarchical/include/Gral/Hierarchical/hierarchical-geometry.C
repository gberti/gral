#ifndef GRAL_GB_HIERARCHICAL_GEOMETRY_C
#define GRAL_GB_HIERARCHICAL_GEOMETRY_C

// $LICENSE_NEC

#include "Gral/Hierarchical/hierarchical-geometry.h"

namespace GrAL {
namespace hierarchical {

  template<class HGRID, class FLATGEOM>
  hier_geometry<HGRID, FLATGEOM>::hier_geometry() {}

  template<class HGRID, class FLATGEOM>
  hier_geometry<HGRID, FLATGEOM>::hier_geometry(typename hier_geometry<HGRID, FLATGEOM>::grid_type const& g)
    : base(g) 
  {
    base::connect(&g);
  }



 template<class HGRID, class FLATGEOM>
  hier_geometry<HGRID, FLATGEOM>::hier_geometry(ref_ptr<typename hier_geometry<HGRID, FLATGEOM>::grid_type const> g)
    : base(g) 
 {
   base::connect(&(*g));
 }

  template<class HGRID, class FLATGEOM>
  hier_geometry<HGRID, FLATGEOM>::hier_geometry
  (typename hier_geometry<HGRID, FLATGEOM>::grid_type          const& g,
   typename hier_geometry<HGRID, FLATGEOM>::flat_geometry_type const& root_geo)
  : base(g, root_geo),
    root_geom(root_geo)
  {
    base::connect(&g);
  }


  template<class HGRID, class FLATGEOM>
  hier_geometry<HGRID, FLATGEOM>::hier_geometry
  (ref_ptr<typename hier_geometry<HGRID, FLATGEOM>::grid_type const>          g,
   ref_ptr<typename hier_geometry<HGRID, FLATGEOM>::flat_geometry_type const> root_geo)
  : base(g, * root_geo),
    root_geom(root_geo)
  {
    base::connect(& *g);
  }


  template<class HGRID, class FLATGEOM>
  void hier_geometry<HGRID, FLATGEOM>::set_grid(typename hier_geometry<HGRID, FLATGEOM>::grid_type const& g)
  {
    base::disconnect();
    base::set_grid(g);
    base::connect(&g);
  }

  template<class HGRID, class FLATGEOM>
  void hier_geometry<HGRID, FLATGEOM>::set_grid(ref_ptr<typename hier_geometry<HGRID, FLATGEOM>::grid_type const> g)
  {
    base::disconnect();
    base::set_grid(g);
    base::connect(& (*g));
  }

  /*
  template<class HGRID, class FLATGEOM>
  void hier_geometry<HGRID, FLATGEOM>::init    (typename hier_geometry<HGRID, FLATGEOM>::grid_type const& g)
  {
    base.init(g);
  }
  */

  template<class HGRID, class FLATGEOM>
  void hier_geometry<HGRID, FLATGEOM>::clear() 
  { 
    base::clear();
    base::disconnect();
  }
  
  template<class HGRID, class FLATGEOM>
  void hier_geometry<HGRID, FLATGEOM>::update()
  { 
    base::update();
  }

  template<class HGRID, class FLATGEOM>
  typename hier_geometry<HGRID, FLATGEOM>::level_handle
  hier_geometry<HGRID, FLATGEOM>::add_coarser_level()
  {
    return(root_geom != 0 ?
	   base::add_coarser_level(*root_geom) :
	   base::add_coarser_level());
  }

  template<class HGRID, class FLATGEOM>
  typename hier_geometry<HGRID, FLATGEOM>::level_handle
  hier_geometry<HGRID, FLATGEOM>::add_finer_level()
  {
    return( root_geom != 0 ?
	    base::add_finer_level(*root_geom) :
	    base::add_finer_level());
  }

  template<class HGRID, class FLATGEOM>
  typename hier_geometry<HGRID, FLATGEOM>::level_handle
  hier_geometry<HGRID, FLATGEOM>::add_root_level()
  {
    if(root_geom == 0)
      return base::add_root_level();
    else {
      update_root_geom();
      return base::add_root_level(*root_geom);
    }
  }

  template<class HGRID, class FLATGEOM>
  void hier_geometry<HGRID, FLATGEOM>::update_root_geom()
  {
    root_geom.make_own(new flat_geometry_type(* TheGrid()->FlatGrid(0), *root_geom));
		       
					  
  }

  template<class HGRID, class FLATGEOM>
  void
  hier_geometry<HGRID, FLATGEOM>::remove_coarsest_level()
  {
    base::remove_coarsest_level();
  }

  template<class HGRID, class FLATGEOM>
  void
  hier_geometry<HGRID, FLATGEOM>::remove_finest_level()
  {
    base::remove_finest_level();
  }


} // namespace hierarchical

} // namespace GrAL 

#endif
