#ifndef GRAL_GB_HIERARCHICAL_GRID_FUNCTION_C
#define GRAL_GB_HIERARCHICAL_GRID_FUNCTION_C

/* ------------------------------------------------------------

    Copyright (C) 2001 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include "Gral/Hierarchical/hierarchical-grid-function.h"
#include <algorithm>

namespace GrAL {

namespace hierarchical {

 template<class E, class T>
 void hier_grid_function<E,T>::init(typename hier_grid_function<E,T>::value_type const& t)
 {
   for(level_handle lev = TheGrid()->coarsest_level(); lev <= TheGrid()->finest_level(); ++lev)
     std::fill(gfs[lev].begin(), gfs[lev].end(),t);
 }
 template<class E, class T>
 void hier_grid_function<E,T>::clear() { gfs.clear();}

 template<class E, class T>
 void hier_grid_function<E,T>::set_grid(typename hier_grid_function<E,T>::hier_grid_type const& gg)
 {
   clear();
   gfs.set_grid(gg);
 }


 template<class E, class T>
 typename hier_grid_function<E,T>::level_handle
 hier_grid_function<E,T>::add_finer_level() 
 {
   return gfs.add_finer_level();
 }

 template<class E, class T>
 typename hier_grid_function<E,T>::level_handle
 hier_grid_function<E,T>::add_finer_level(typename hier_grid_function<E,T>::value_type const& t)
 {
   level_handle newlev = add_finer_level();
   std::fill(gfs[newlev].begin(), gfs[newlev].end(), t);
   return newlev;
 } 

 template<class E, class T>
 typename hier_grid_function<E,T>::level_handle
 hier_grid_function<E,T>::add_coarser_level() 
 {
   return gfs.add_coarser_level();
 }

 template<class E, class T>
 typename hier_grid_function<E,T>::level_handle
 hier_grid_function<E,T>::add_coarser_level(typename hier_grid_function<E,T>::value_type const& t)
 {
   level_handle newlev = add_coarser_level();
   std::fill(gfs[newlev].begin(), gfs[newlev].end(), t);
   return newlev;
 } 

 template<class E, class T>
 void 
 hier_grid_function<E,T>::remove_coarsest_level()
 {
   return gfs.remove_coarsest_level();
 }

 template<class E, class T>
 void 
 hier_grid_function<E,T>::remove_finest_level()
 {
   return gfs.remove_finest_level();
 }



} //  namespace hierarchical

} // namespace GrAL 

#endif
