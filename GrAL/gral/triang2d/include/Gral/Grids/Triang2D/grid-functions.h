#ifndef GRAL_GB_GRIDS_TRIANG2D_GRIDFUNCTION_H
#define GRAL_GB_GRIDS_TRIANG2D_GRIDFUNCTION_H

// $LICENSE

#include "Gral/Grids/Triang2D/triang2d.h"
#include "Gral/Grids/Triang2D/element-traits.h"

// generic grid functions
#include "Gral/Base/grid-function-vector.h"
#include "Gral/Base/grid-function-hash.h"



template<class T>
class grid_function<Triang2D_Vertex, T> 
  : public grid_function_vector<Triang2D_Vertex, T> 
{
  typedef grid_function_vector<Triang2D_Vertex, T> base;
public:
  typedef typename base::grid_type grid_type;

  grid_function() {}
  grid_function(grid_type const& gg) : base(gg) {}
  grid_function(grid_type const& gg, T const& t) : base(gg,t) {}

  T&       operator[](Triang2D_Vertex     const& v)       { return table[v.handle()];}
  T const& operator()(Triang2D_Vertex     const& v) const { return table[v.handle()];}
  T& operator[](Triang2D_VertexOnCellIterator const& v) { return table[v.handle()];}
};


template<class T>
class grid_function<Triang2D_Cell, T> 
  : public grid_function_vector<Triang2D_Cell, T> 
{
  typedef grid_function_vector<Triang2D_Cell, T> base;
public:
  typedef typename base::grid_type grid_type;

  grid_function() {}
  grid_function(grid_type const& gg) : base(gg) {}
  grid_function(grid_type const& gg, T const& t) : base(gg,t) {}

  T      & operator[](Triang2D_Cell const& c)       { return table[c.handle()];}
  T const& operator()(Triang2D_Cell const& c) const { return table[c.handle()];}
};



template<class T>
class grid_function<Triang2D::Edge, T> 
  : public grid_function_hash<Triang2D::Edge, T> 
{
  typedef grid_function_hash<Triang2D::Edge, T> base;
public:
  typedef typename base::grid_type grid_type;

  grid_function() {}
  grid_function(grid_type const& gg) : base(gg) {}
  grid_function(grid_type const& gg, T const& t) : base(gg,t) {}

};


#endif


