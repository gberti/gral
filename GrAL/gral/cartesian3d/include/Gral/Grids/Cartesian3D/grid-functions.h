#ifndef GRAL_GB_CARTESIAN3D_GRIDFUNCTIONS_H
#define GRAL_GB_CARTESIAN3D_GRIDFUNCTIONS_H

// $LICENSE

#include "Gral/Grids/Cartesian3D/cartesian3d.h"

// generic total grid function based on std::vector 
#include "Gral/Base/grid-function-vector.h"


// use generic version for total grid functions

template<class T>
class grid_function<cartesian3d::Vertex_Cartesian3D, T>
  : public grid_function_vector<cartesian3d::Vertex_Cartesian3D,T>
{
  typedef grid_function_vector<cartesian3d::Vertex_Cartesian3D,T> base;
 public:
  typedef typename base::grid_type grid_type;

  grid_function() {}
  grid_function(grid_type const& gg) : base(gg) {}
  grid_function(grid_type const& gg, T const& t) : base(gg,t) {}

};


template<class T>
class grid_function<cartesian3d::Edge_Cartesian3D, T>
  : public grid_function_vector<cartesian3d::Edge_Cartesian3D,T>
{
  typedef grid_function_vector<cartesian3d::Edge_Cartesian3D,T> base;
 public:
  typedef typename base::grid_type grid_type;

  grid_function() {}
  grid_function(grid_type const& gg) : base(gg) {}
  grid_function(grid_type const& gg, T const& t) : base(gg,t) {}

};

template<class T>
class grid_function<cartesian3d::Facet_Cartesian3D, T>
  : public grid_function_vector<cartesian3d::Facet_Cartesian3D,T>
{
  typedef grid_function_vector<cartesian3d::Facet_Cartesian3D,T> base;
 public:
  typedef typename base::grid_type grid_type;

  grid_function() {}
  grid_function(grid_type const& gg) : base(gg) {}
  grid_function(grid_type const& gg, T const& t) : base(gg,t) {}

};


template<class T>
class grid_function<cartesian3d::Cell_Cartesian3D, T>
  : public grid_function_vector<cartesian3d::Cell_Cartesian3D,T>
{
  typedef grid_function_vector<cartesian3d::Cell_Cartesian3D,T> base;
 public:
  typedef typename base::grid_type grid_type;

  grid_function() {}
  grid_function(grid_type const& gg) : base(gg) {}
  grid_function(grid_type const& gg, T const& t) : base(gg,t) {}

};



#endif
