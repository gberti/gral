#ifndef NMWR_GB_REG2D_GRIDFUNCTIONS_H
#define NMWR_GB_REG2D_GRIDFUNCTIONS_H


//----------------------------------------------------------------
//   (c) Guntram Berti, 1997
//   Chair for Numerical Mathematics & Scientific Computing (NMWR)
//   TU Cottbus - Germany
//   http://math-s.math.tu-cottbus.de/NMWR
//   
//----------------------------------------------------------------

#include "Grids/Reg2D/element-traits.h" 
#include "Grids/grid-function-vector.h"

template<class T>
class grid_function<RegGrid2D::Vertex,T> 
  : public grid_function_vector<RegGrid2D::Vertex,T>
{
  typedef grid_function<RegGrid2D::Vertex,T>        self;
  typedef grid_function_vector<RegGrid2D::Vertex,T> base;
public:
  grid_function() {}
  grid_function(grid_type const& gg) : base(gg) {}
  grid_function(grid_type const& gg, T const& t) : base(gg,t) {}
};


template<class T>
class grid_function<RegGrid2D::Edge,T> 
  : public grid_function_vector<RegGrid2D::Edge,T>
{
  typedef grid_function<RegGrid2D::Edge,T>        self;
  typedef grid_function_vector<RegGrid2D::Edge,T> base;
public:
  grid_function() {}
  grid_function(grid_type const& gg) : base(gg) {}
  grid_function(grid_type const& gg, T const& t) : base(gg,t) {}
};


template<class T>
class grid_function<RegGrid2D::Cell,T> 
  : public grid_function_vector<RegGrid2D::Cell,T>
{
  typedef grid_function<RegGrid2D::Cell,T>        self;
  typedef grid_function_vector<RegGrid2D::Cell,T> base;
public:
  grid_function() {}
  grid_function(grid_type const& gg) : base(gg) {}
  grid_function(grid_type const& gg, T const& t) : base(gg,t) {}
};



#define SPECIALIZATION_GRIDFUNCTION_REGGRID2D(E,T) 


#endif
