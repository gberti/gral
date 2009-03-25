#ifndef GRAL_GB_GRAPH_BGL_PROPERTY_MAP_H
#define GRAL_GB_GRAPH_BGL_PROPERTY_MAP_H


/* ------------------------------------------------------------

    Copyright (C) 2005 - 2009 Guntram Berti, 
    Contact: guntram AT users DOT berlios DOT de

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include <boost/ref.hpp>

namespace boost {

  //--- make reference_wrapper<property_map> work ----

  template<class PM>
  typename PM::value_type 
  get(reference_wrapper<PM const> pm, typename PM::key_type k) { return get(pm.get(),k);}

  template<class PM>
  typename PM::value_type 
  get(reference_wrapper<PM      > pm, typename PM::key_type k) { return get(pm.get(),k);}

  template<class PM>
  void 
  put(reference_wrapper<PM      > pm, typename PM::key_type k, typename PM::value_type t) 
  { put(pm.get(),k,t);}


} // namespace boost 

#endif
