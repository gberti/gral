#ifndef GRAL_GB_HIERARCHICAL_PARTIAL_GRID_FUNCTION_H
#define GRAL_GB_HIERARCHICAL_PARTIAL_GRID_FUNCTION_H


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

  /*! \brief A hierarchical partial grid function
      \author Guntram Berti

      \templateparams
      - \c E is an element type of an hierarchical grid
      - \c T is a plain value type

  */
  template<class E, class T>
  class hier_partial_grid_function : public hier_grid_function_base<typename E::hier_grid_type, 
								    partial_grid_function<typename E::flat_element_type,T> >
  {
    typedef hier_grid_function_base<typename E::hier_grid_type, 
				    partial_grid_function<typename E::flat_element_type,T> > base;
    typedef hier_partial_grid_function<E,T>                     self;
  public:
    using base::TheGrid;

    hier_partial_grid_function() {}
    hier_partial_grid_function(typename base::hier_grid_type const& gg) : base(gg) {} 
    hier_partial_grid_function(typename base::hier_grid_type const& gg,
			       typename base::value_type     const& t ) : base(gg,t) {} 

    void set_default(typename base::value_type const& t) 
    {
      for(typename base::level_handle lev = TheGrid()->coarsest_level(); lev <= TheGrid()->finest_level(); ++lev)
	(*this)[lev].set_default(t);
    }

    template<class ELEM>
    void undefine(ELEM const& e) { base::gfs[e.level()].undefine(e.Flat());}
    template<class ELEM>
    bool defined (ELEM const& e) const { return base::gfs(e.level()).defined(e.Flat());}
  };

} // namespace hierarchical
} // namespace GrAL

#endif
