#ifndef GRAL_GB_HIERARCHICAL_GRID_FUNCTION_BASE_C
#define GRAL_GB_HIERARCHICAL_GRID_FUNCTION_BASE_C

// $LICENSE_NEC

#include "Gral/Hierarchical/hierarchical-grid-function-base.h"


namespace hierarchical {

 template<class E, class T, template<class EE, class TT> class GF>
 void hier_grid_function_base<E,T,GF>::
 init(typename hier_grid_function_base<E,T,GF>::hier_grid_type const& gg,
      typename hier_grid_function_base<E,T,GF>::value_type const& t)
 {
   set_grid (gg);
   set_value(t);
 }

 template<class E, class T, template<class EE, class TT> class GF>
 void hier_grid_function_base<E,T,GF>::clear() { gfs.clear();}

 template<class E, class T, template<class EE, class TT> class GF>
 void hier_grid_function_base<E,T,GF>::update() { gfs.update();}

 template<class E, class T, template<class EE, class TT> class GF>
 void hier_grid_function_base<E,T,GF>::
 set_grid(typename hier_grid_function_base<E,T,GF>::hier_grid_type const& gg)
 {
   clear();
   gfs.set_grid(gg);
 }

 template<class E, class T, template<class EE, class TT> class GF>
 void hier_grid_function_base<E,T,GF>::
 set_value(typename hier_grid_function_base<E,T,GF>::value_type t)
 {
   for(level_handle lev = TheGrid()->coarsest_level(); lev <= TheGrid()->finest_level(); ++lev)
     gfs[lev].set_value(t);
 }

 template<class E, class T, template<class EE, class TT> class GF>
 typename hier_grid_function_base<E,T,GF>::level_handle
 hier_grid_function_base<E,T,GF>::add_finer_level() 
 {
   return gfs.add_finer_level();
 }

 template<class E, class T, template<class EE, class TT> class GF>
 typename hier_grid_function_base<E,T,GF>::level_handle
 hier_grid_function_base<E,T,GF>::
 add_finer_level(typename hier_grid_function_base<E,T,GF>::value_type const& t)
 {
   level_handle newlev = add_finer_level();
   gfs[newlev].set_value(t);
   return newlev;
 } 

 template<class E, class T, template<class EE, class TT> class GF>
 typename hier_grid_function_base<E,T,GF>::level_handle
 hier_grid_function_base<E,T,GF>::add_coarser_level() 
 {
   return gfs.add_coarser_level();
 }

 template<class E, class T, template<class EE, class TT> class GF>
 typename hier_grid_function_base<E,T,GF>::level_handle
 hier_grid_function_base<E,T,GF>::
 add_coarser_level(typename hier_grid_function_base<E,T,GF>::value_type const& t)
 {
   level_handle newlev = add_coarser_level();
   gfs[newlev].set_value(t);
   return newlev;
 } 

 template<class E, class T, template<class EE, class TT> class GF>
 void 
 hier_grid_function_base<E,T,GF>::remove_coarsest_level()
 {
   return gfs.remove_coarsest_level();
 }

 template<class E, class T, template<class EE, class TT> class GF>
 void 
 hier_grid_function_base<E,T,GF>::remove_finest_level()
 {
   return gfs.remove_finest_level();
 }

  // Work done by hier_grid_table ...
 template<class E, class T, template<class EE, class TT> class GF>
 void 
 hier_grid_function_base<E,T,GF>::notifier_assigned(hier_grid_function_base<E,T,GF>::notifier_base const* n)
 {}


 template<class E, class T, template<class EE, class TT> class GF>
 void 
 hier_grid_function_base<E,T,GF>::hgrid_level_added  
 (hier_grid_function_base<E,T,GF>::notifier_type const* n, level_handle added) 
 {}

 template<class E, class T, template<class EE, class TT> class GF>
 void 
 hier_grid_function_base<E,T,GF>::hgrid_level_removed
 (hier_grid_function_base<E,T,GF>::notifier_type const* n, level_handle added) 
 {}


} //  namespace hierarchical

#endif
