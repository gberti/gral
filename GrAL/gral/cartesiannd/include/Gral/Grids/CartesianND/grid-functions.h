#ifndef GRAL_GB_CARTESIANND_GRID_FUNCTIONS_H
#define GRAL_GB_CARTESIANND_GRID_FUNCTIONS_H

// $LICENSE_NEC

#include "Gral/Grids/CartesianND/cartesiannd.h"
#include "Gral/Base/grid-function-vector.h"

template<unsigned DIM, unsigned K, class T>
class grid_function<cartesiannd::sequence_iterator_t<cartesiannd::grid<DIM> ,K>, T >
  : public grid_function_vector<cartesiannd::sequence_iterator_t<cartesiannd::grid<DIM>,K>, T >
{
  typedef grid_function_vector<cartesiannd::sequence_iterator_t<cartesiannd::grid<DIM>,K>, T > base;
  typedef cartesiannd::grid<DIM>                                                               grid_type;
public:  
  grid_function() {}
  grid_function(grid_type const& g) : base(g) {}
  grid_function(grid_type const& g, T const& t) : base(g,t) {}
  grid_function(ref_ptr<grid_type const> g) : base(g) {}
  grid_function(ref_ptr<grid_type const> g, T const& t) : base(g,t) {}
};


#endif
