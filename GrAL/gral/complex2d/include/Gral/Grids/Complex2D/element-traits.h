#ifndef NMWR_GB_COMPLEX2D_ELEMENT_TRAITS_H
#define NMWR_GB_COMPLEX2D_ELEMENT_TRAITS_H


//----------------------------------------------------------------
//   (c) Guntram Berti, 1997
//   Chair for Numerical Mathematics & Scientific Computing (NMWR)
//   TU Cottbus - Germany
//   http://math-s.math.tu-cottbus.de/NMWR
//   
//----------------------------------------------------------------


//----------------------------------------------------------------
//
//  traits classes for enabling uniform treatment
//  of different element types (vertex, edge etc)
//  in template class grid_function<E,T>.
//
//----------------------------------------------------------------

#include "Gral/Base/element-traits.h"
#include "Gral/Grids/Complex2D/complex2d.h"


template<class E> 
struct hasher_for_complex2d {};



struct hasher_for_complex2d<Vertex2D> 
  : public hash_vertex2d {};
struct hasher_for_complex2d<Edge2D> 
  : public hash_edge2d {};
struct hasher_for_complex2d<Cell2D> 
  : public hash_cell2d {};

template<class E>
struct element_traits_complex2d {};

struct element_traits_complex2d<Vertex2D> {
  typedef Complex2D                          grid_type;
  typedef grid_types<Complex2D>              gt;
  typedef  gt::Vertex                        element_type;
  typedef  gt::VertexIterator                ElementIterator;
  typedef  gt::vertex_handle                 handle_type;
  typedef hasher_for_complex2d<element_type> hasher_type;
  typedef vertex_type_tag                    element_type_tag;

  static ElementIterator FirstElement(const Complex2D& g) { return g.FirstVertex();}
  static element_type    handle2element(const Complex2D& g, handle_type h)
    { return g.vertex(h);}
  static handle_type handle(element_type const& e) { return e.handle();}
  static size_t  size(const Complex2D& g) { return g.NumOfVertices();}

};


struct element_traits_complex2d<Edge2D> {
  typedef Complex2D                          grid_type;
  typedef grid_types<Complex2D>              gt;
  typedef  gt::Edge                  element_type;
  typedef  gt::EdgeIterator          ElementIterator;
  typedef  gt::edge_handle           handle_type;
  typedef hasher_for_complex2d<element_type> hasher_type;
  typedef edge2d_type_tag                    element_type_tag;
  //  typedef facet1d_type_tag                     element_cotype_tag;
  

  static ElementIterator FirstElement(const Complex2D& g)  { return g.FirstEdge();}
  static element_type    handle2element(const Complex2D& g, const handle_type& h)
    { return g.edge(h);}
  static handle_type handle(element_type const& e) { return e.handle();}
  static size_t size(const Complex2D& g) { return g.NumOfEdges();}
};


struct element_traits_complex2d<Cell2D> {
  typedef Complex2D                          grid_type;
  typedef grid_types<Complex2D>              gt;
  typedef  gt::Cell                  element_type;
  typedef  gt::cell_handle           handle_type;
  typedef  gt::CellIterator          ElementIterator;
  typedef hasher_for_complex2d<element_type> hasher_type;
  typedef cell2d_type_tag                    element_type_tag;

  static ElementIterator FirstElement(const Complex2D& g)  { return g.FirstCell();}
  static element_type    handle2element(const Complex2D& g, handle_type h)
    { return g.cell(h);}
  static handle_type handle(element_type const& e) { return e.handle();}
  static size_t size(const Complex2D& g) { return g.NumOfCells();}
};

struct element_traits<Vertex2D> : public element_traits_complex2d<Vertex2D> {};
struct element_traits<Edge2D>   : public element_traits_complex2d<Edge2D>   {};
struct element_traits<Cell2D>   : public element_traits_complex2d<Cell2D>   {};

#endif
