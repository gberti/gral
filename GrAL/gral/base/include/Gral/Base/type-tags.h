#ifndef NMWR_GB_TYPE_TAGS_H
#define NMWR_GB_TYPE_TAGS_H


//----------------------------------------------------------------
//   (c) Guntram Berti, 1997
//   Chair for Numerical Mathematics & Scientific Computing (NMWR)
//   TU Cottbus - Germany
//   http://math-s.math.tu-cottbus.de/NMWR
//   
//----------------------------------------------------------------


//----------------------------------------------------------------
//
//  template<class T> class tp {} is a workaround for missing explicit
//  specialization of function template arguments.
//  This empty frame is to be specialized for every element type.
//  Its purpose is to create a unique tagging type for that element
//  types in order to select appropriate functions by the type of
//  the element.
//
//  E.g.
// template<class Elt> class client {
//  ...
//   the_range = server.get_range(tp<Elt>);
//  ...
// };
//
// class server {
//
//  vertex_range get_range(tp<Vertex>);
//  edge_range   get_range(tp<Edge>);
//  // etc.
//  // without tp<>, we would need something like this:
//  // template<class E> get_range();  // empty frame
//  // vertex_range get_range<Vertex>(); // specialization
//  // edge_range   get_range<Edge>();   // is this possible at all??
//  };
//
// This approach allows in class client to write code independent
// of the type of Elt, be it Vertex, Edge or Cell.
//----------------------------------------------------------------

template<class T> 
class tp {};

enum grid_element_tag { vertex_tag, edge_tag, facet_tag, cell_tag };

class vertex_type_tag {};
class edge_type_tag   {};
class facet_type_tag  {};
class cell_type_tag   {};

class edge2d_type_tag  : public edge_type_tag {};
class facet1d_type_tag : public facet_type_tag {};
class facet2d_type_tag : public facet_type_tag {};
class cell2d_type_tag  : public cell_type_tag {};
class cell3d_type_tag  : public cell_type_tag {};


#endif
