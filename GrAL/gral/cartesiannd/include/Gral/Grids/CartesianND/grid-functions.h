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


/*
// vertex
template<unsigned DIM, class T>
class grid_function<cartesiannd::sequence_iterator_t<cartesiannd::grid<DIM> ,0>, T >
  : public grid_function_vector<cartesiannd::sequence_iterator_t<cartesiannd::grid<DIM>,0>, T >
{
  typedef grid_function_vector<cartesiannd::sequence_iterator_t<cartesiannd::grid<DIM>,0>, T > base;
  typedef cartesiannd::grid<DIM>                                                               grid_type;
public:  
  grid_function() {}
  grid_function(grid_type const& g) : base(g) {}
  grid_function(grid_type const& g, T const& t) : base(g,t) {}
  grid_function(ref_ptr<grid_type const> g) : base(g) {}
  grid_function(ref_ptr<grid_type const> g, T const& t) : base(g,t) {}
};

// edge
template<unsigned DIM, class T>
class grid_function<cartesiannd::sequence_iterator_t<cartesiannd::grid<DIM> ,1>, T >
  : public grid_function_vector<cartesiannd::sequence_iterator_t<cartesiannd::grid<DIM>,1>, T >
{
  typedef grid_function_vector<cartesiannd::sequence_iterator_t<cartesiannd::grid<DIM>,1>, T > base;
  typedef cartesiannd::grid<DIM>                                                               grid_type;
public:  
  grid_function() {}
  grid_function(grid_type const& g) : base(g) {}
  grid_function(grid_type const& g, T const& t) : base(g,t) {}
  grid_function(ref_ptr<grid_type const> g) : base(g) {}
  grid_function(ref_ptr<grid_type const> g, T const& t) : base(g,t) {}
};

// facet
template<unsigned DIM, class T>
class grid_function<cartesiannd::sequence_iterator_t<cartesiannd::grid<DIM>,DIM-1>, T >
  : public grid_function_vector<cartesiannd::sequence_iterator_t<cartesiannd::grid<DIM>,DIM-1> , T >
{
  typedef grid_function_vector<cartesiannd::sequence_iterator_t<cartesiannd::grid<DIM>,DIM-1>, T > base;
  typedef cartesiannd::grid<DIM-1>                                                                 grid_type;
public:  
  grid_function() {}
  grid_function(grid_type const& g) : base(g) {}
  grid_function(grid_type const& g, T const& t) : base(g,t) {}
  grid_function(ref_ptr<grid_type const> g) : base(g) {}
  grid_function(ref_ptr<grid_type const> g, T const& t) : base(g,t) {}
};


// cell
template<unsigned DIM, class T>
class grid_function<cartesiannd::sequence_iterator_t<cartesiannd::grid<DIM>,DIM>, T >
  : public grid_function_vector<cartesiannd::sequence_iterator_t<cartesiannd::grid<DIM>,DIM> , T >
{
  typedef grid_function_vector<cartesiannd::sequence_iterator_t<cartesiannd::grid<DIM>,DIM>, T > base;
  typedef cartesiannd::grid<DIM>                                                                 grid_type;
public:  
  grid_function() {}
  grid_function(grid_type const& g) : base(g) {}
  grid_function(grid_type const& g, T const& t) : base(g,t) {}
  grid_function(ref_ptr<grid_type const> g) : base(g) {}
  grid_function(ref_ptr<grid_type const> g, T const& t) : base(g,t) {}
};


// disambiguate
// 0D: vertex <-> cell


// avoid conflict edge<->facet in 2D
template<class T>
class grid_function<cartesiannd::sequence_iterator_t<cartesiannd::grid<2> ,1>, T >
  : public grid_function_vector<cartesiannd::sequence_iterator_t<cartesiannd::grid<2>,1>, T >
{
  typedef grid_function_vector<cartesiannd::sequence_iterator_t<cartesiannd::grid<2>,1>, T > base;
  typedef cartesiannd::grid<2>                                                               grid_type;
public:  
  grid_function() {}
  grid_function(grid_type const& g) : base(g) {}
  grid_function(grid_type const& g, T const& t) : base(g,t) {}
  grid_function(ref_ptr<grid_type const> g) : base(g) {}
  grid_function(ref_ptr<grid_type const> g, T const& t) : base(g,t) {}
};
*/
#endif
