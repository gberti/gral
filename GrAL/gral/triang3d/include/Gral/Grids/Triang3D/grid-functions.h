#ifndef GRAL_GB_GRIDS_TRIANG3D_GRIDFUNCTION_H
#define GRAL_GB_GRIDS_TRIANG3D_GRIDFUNCTION_H

// $LICENSE_NEC_2003

#include "Gral/Grids/Triang3D/triang3d.h"
#include "Gral/Grids/Triang3D/element-traits.h"

// generic grid functions
#include "Gral/Base/grid-function-vector.h"
#include "Gral/Base/grid-function-hash.h"



template<class T>
class grid_function<Triang3D_Vertex, T> 
  : public grid_function_vector<Triang3D_Vertex, T> 
{
  typedef grid_function_vector<Triang3D_Vertex, T> base;
public:
  typedef typename base::grid_type grid_type;

  grid_function() {}
  grid_function(grid_type const& gg) : base(gg) {}
  grid_function(grid_type const& gg, T const& t) : base(gg,t) {}

  T&       operator[](Triang3D_Vertex     const& v)       { return table[v.handle()];}
  T const& operator()(Triang3D_Vertex     const& v) const { return table[v.handle()];}
  T& operator[](Triang3D_VertexOnCellIterator const& v) { return table[v.handle()];}
};


template<class T>
class grid_function<Triang3D_Cell, T> 
  : public grid_function_vector<Triang3D_Cell, T> 
{
  typedef grid_function_vector<Triang3D_Cell, T> base;
public:
  typedef typename base::grid_type grid_type;

  grid_function() {}
  grid_function(grid_type const& gg) : base(gg) {}
  grid_function(grid_type const& gg, T const& t) : base(gg,t) {}

  T      & operator[](Triang3D_Cell const& c)       { return table[c.handle()];}
  T const& operator()(Triang3D_Cell const& c) const { return table[c.handle()];}
};



template<class T>
class grid_function<Triang3D::Edge, T>
  : public grid_function_hash<Triang3D::Edge,T>
{
  typedef grid_function_hash<Triang3D::Edge,T> base;
 public:
  typedef typename base::grid_type grid_type;

  grid_function() {}
  grid_function(grid_type const& gg) : base(gg) {}
  grid_function(grid_type const& gg, T const& t) : base(gg,t) {}

};

template<class T>
class grid_function<Triang3D::Facet, T>
  : public grid_function_hash<Triang3D::Facet,T>
{
  typedef grid_function_hash<Triang3D::Facet,T> base;
 public:
  typedef typename base::grid_type grid_type;

  grid_function() {}
  grid_function(grid_type const& gg) : base(gg) {}
  grid_function(grid_type const& gg, T const& t) : base(gg,t) {}

};


#endif


