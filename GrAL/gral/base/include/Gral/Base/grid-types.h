#ifndef NMWR_GB_GRID_TYPES_H
#define NMWR_GB_GRID_TYPES_H

//----------------------------------------------------------------
//   (c) Guntram Berti, 1997
//   Chair for Numerical Mathematics & Scientific Computing (NMWR)
//   TU Cottbus - Germany
//   http://math-s.math.tu-cottbus.de/NMWR
//   
//----------------------------------------------------------------

//----------------------------------------------------------------
//
// template<class Grid> class grid_types;
//
// This is the empty frame for parameterized namespaces associated 
// with a grid.
// The specializations for a concrete grid type provide access to
// grid related types like
//   * Vertex, Edge, ....
//   * VertexIterator, ....
//   * vertex_handle, ...
//
// sample usage:
// ------------
// typedef grid_types<Complex2D> gt;
// for(typename gt::VertexIterator v = G.FirstVertex(); ...)
//    do_something(*v);
//
//----------------------------------------------------------------


template<class Grid>
struct grid_types {};


#endif
