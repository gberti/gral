#ifndef GRAL_GB_HIERARCHICAL_GRID_FUNCTION_H
#define GRAL_GB_HIERARCHICAL_GRID_FUNCTION_H


/* ------------------------------------------------------------

    Copyright (C) 2001 - 2009 Guntram Berti, 
    Contact: guntram AT users DOT berlios DOT de

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include "Gral/Hierarchical/hierarchical-grid-function-base.h"

namespace GrAL {

namespace hierarchical {

  /*! \brief A hierarchical  grid function
      \author Guntram Berti

      \templateparams
      - \c E is an element type of an hierarchical grid
      - \c T is a plain value type

  */
  template<class E, class T>
  class hier_grid_function : public hier_grid_function_base<typename E::hier_grid_type, 
							    grid_function<typename E::flat_element_type,T> >
  {
    typedef hier_grid_function_base<typename E::hier_grid_type, 
				    grid_function<typename E::flat_element_type,T> > base;
    typedef hier_grid_function<E,T>                     self;
  public:
    hier_grid_function() {}
    hier_grid_function(typename base::hier_grid_type const& gg) : base(gg) {} 
    hier_grid_function(typename base::hier_grid_type const& gg,
			       typename base::value_type     const& t ) : base(gg,t) {} 
  };

} // namespace hierarchical

} // namespace GrAL {

#endif
