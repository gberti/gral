#ifndef GRAL_GB_CARTESIAN3D_GRIDFUNCTIONS_H
#define GRAL_GB_CARTESIAN3D_GRIDFUNCTIONS_H

// $LICENSE

#include "Gral/Grids/Cartesian3D/cartesian3d.h"

// generic total grid function based on std::vector 
#include "Gral/Base/grid-function-vector.h"


// use generic version for total grid functions

template<class T>
class grid_function<Vertex_Cartesian3D, T>
  : public grid_function_vector<Vertex_Cartesian3D,T>
{
  typedef grid_function_vector<Vertex_Cartesian3D,T> base;
 public:
  grid_function() {}
  grid_function(grid_type const& gg) : base(gg) {}
  grid_function(grid_type const& gg, T const& t) : base(gg,t) {}

};


template<class T>
class grid_function<Cell_Cartesian3D, T>
  : public grid_function_vector<Cell_Cartesian3D,T>
{
  typedef grid_function_vector<Cell_Cartesian3D,T> base;
 public:
  grid_function() {}
  grid_function(grid_type const& gg) : base(gg) {}
  grid_function(grid_type const& gg, T const& t) : base(gg,t) {}

};


#endif
