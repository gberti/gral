#ifndef NMWR_GB_GRIDS_TRIANG2D_ELEMENT_TRAITS_H
#define NMWR_GB_GRIDS_TRIANG2D_ELEMENT_TRAITS_H

//----------------------------------------------------------------
//   (c) Guntram Berti, 1999
//   Chair for Numerical Mathematics & Scientific Computing (NMWR)
//   TU Cottbus - Germany
//   http://math-s.math.tu-cottbus.de/NMWR
//   
//----------------------------------------------------------------


#include "Grids/Triang2D/triang2d.h"

template<class T> struct hash;

struct hash<Triang2D_Vertex> {
  size_t operator()(Triang2D_Vertex const& v) const { return v.handle();}
};

struct hash<Triang2D_Cell> {
  size_t operator()(Triang2D_Cell const& c) const { return c.handle();}
};

struct hash<Triang2D::Edge> : public grid_types_base_Triang2D {
  enum { p= 37};
  size_t operator()(Triang2D::Edge const& e) const { 
    vertex_handle v1 = e.V1().handle();
    vertex_handle v2 = e.V2().handle();
    return (v1 > v2 ? p*v1 + (v2%p) : p*v2 + (v1%p));
  }
};

struct element_traits<Triang2D_Vertex> : public grid_types_base_Triang2D {
  typedef Triang2D_Vertex  element_type;
  typedef vertex_type_tag  element_type_tag;
  typedef vertex_handle    handle_type;
  typedef VertexIterator   ElementIterator;
  typedef hash<Triang2D_Vertex> hasher_type;

  static handle_type handle(VertexIterator const& v) { return v.handle();}
  static size_t size(grid_type const& g) { return g.NumOfVertices();}
  static ElementIterator FirstElement(grid_type const& g) { return ElementIterator(g);}

};

struct element_traits<Triang2D_Cell> : public grid_types_base_Triang2D {

  typedef Triang2D_Cell    element_type;
  typedef cell2d_type_tag  element_type_tag;
  typedef cell_handle      handle_type;
  typedef CellIterator ElementIterator;
  typedef hash<Triang2D_Cell> hasher_type;

  static  handle_type     handle(CellIterator const& v)    { return v.handle();}
  static  size_t          size(grid_type const& g)         { return g.NumOfCells();}
  static  ElementIterator FirstElement(grid_type const& g) { return ElementIterator(g);}
};

struct element_traits<Triang2D::Edge> : public grid_types_base_Triang2D {
  typedef hash<Triang2D::Edge> hasher_type;
  typedef Triang2D             grid_type;
  typedef Triang2D_Edge        element_type;
  typedef edge_type_tag        element_type_tag;
};

#endif
