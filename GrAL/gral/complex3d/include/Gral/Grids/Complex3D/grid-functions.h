#ifndef GRAL_GB_COMPLEX3D_GRIDFUNCTIONS_H
#define GRAL_GB_COMPLEX3D_GRIDFUNCTIONS_H

// $LICENSE_NEC

#include "Gral/Grids/Complex3D/complex3d.h"

#include "Gral/Base/grid-function-vector.h"
#include "Gral/Base/grid-function-hash.h"

template<class T>
class grid_function<Vertex_Complex3D, T>
  : public grid_function_vector<Vertex_Complex3D,T>
{
  typedef grid_function_vector<Vertex_Complex3D,T> base;
 public:
  typedef typename base::grid_type grid_type;

  grid_function() {}
  grid_function(grid_type const& gg) : base(gg) {}
  grid_function(grid_type const& gg, T const& t) : base(gg,t) {}

};


template<class T>
class grid_function<Cell_Complex3D, T>
  : public grid_function_vector<Cell_Complex3D,T>
{
  typedef grid_function_vector<Cell_Complex3D,T> base;
 public:
  typedef typename base::grid_type grid_type;

  grid_function() {}
  grid_function(grid_type const& gg) : base(gg) {}
  grid_function(grid_type const& gg, T const& t) : base(gg,t) {}

};


template<class T>
class grid_function<Complex3D::Edge, T>
  : public grid_function_hash<Complex3D::Edge,T>
{
  typedef grid_function_hash<Complex3D::Edge,T> base;
 public:
  typedef typename base::grid_type grid_type;

  grid_function() {}
  grid_function(grid_type const& gg) : base(gg) {}
  grid_function(grid_type const& gg, T const& t) : base(gg,t) {}

};

template<class T>
class grid_function<Complex3D::Facet, T>
  : public grid_function_hash<Complex3D::Facet,T>
{
  typedef grid_function_hash<Complex3D::Facet,T> base;
 public:
  typedef typename base::grid_type grid_type;

  grid_function() {}
  grid_function(grid_type const& gg) : base(gg) {}
  grid_function(grid_type const& gg, T const& t) : base(gg,t) {}

};


#endif
