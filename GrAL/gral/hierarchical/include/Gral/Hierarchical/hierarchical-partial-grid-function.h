#ifndef GRAL_GB_HIERARCHICAL_PARTIAL_GRID_FUNCTION_H
#define GRAL_GB_HIERARCHICAL_PARTIAL_GRID_FUNCTION_H

// $LICENSE_NEC

#include "Gral/Hierarchical/hierarchical-grid-function-base.h"

namespace hierarchical {

  /*! \brief A hierarchical partial grid function
      \author Guntram Berti

      \templateparams
      - \c E is an element type of an hierarchical grid
      - \c T is a plain value type

  */
  template<class E, class T>
  class hier_partial_grid_function : public hier_grid_function_base<E,T, partial_grid_function>
  {
    typedef hier_grid_function_base<E,T, partial_grid_function> base;
    typedef hier_partial_grid_function<E,T>                     self;
  public:
    hier_partial_grid_function() {}
    hier_partial_grid_function(typename base::hier_grid_type const& gg) : base(gg) {} 
    hier_partial_grid_function(typename base::hier_grid_type const& gg,
			       typename base::value_type     const& t ) : base(gg,t) {} 

    void set_default(typename base::value_type const& t) 
    {
      for(typename base::level_handle lev = TheGrid()->coarsest_level(); lev <= TheGrid()->finest_level(); ++lev)
	(*this)[lev].set_default(t);
    }
  };

} // namespace hierarchical

#endif
