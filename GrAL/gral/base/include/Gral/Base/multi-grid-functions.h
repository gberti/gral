#ifndef GRAL_BASE_GB_MULTI_GRIDFUNCTIONS_H
#define GRAL_BASE_GB_MULTI_GRIDFUNCTIONS_H


//----------------------------------------------------------------
//   (c) Guntram Berti, 1998
//   Chair for Numerical Mathematics & Scientific Computing (NMWR)
//   TU Cottbus - Germany
//   http://math-s.math.tu-cottbus.de/NMWR
//   
//----------------------------------------------------------------

#include "compiler-config.h"
#include "Gral/Base/common-grid-basics.h"

//----------------------------------------------------------------
//
// template<class Grid, class T> class  multi_grid_function;
// template<class Grid, class T> class  partial_multi_grid_function;
//
// These are grid functions defined on all element types of <Grid>.
// T is a user-selected type.
// multi_grid_function<Grid,T> defines a mapping
// V(G) \cup E(G) \cup C(G) -> T ( in the 2d case)
//
// These classes are fully generic and do not need any further
// specialization, because they build on grid_function<Elt,T>.
//
//----------------------------------------------------------------

//----------------------------------------------------------------
//                   multi_grid_function
//----------------------------------------------------------------


template<class Grid, class T>
class multi_grid_function {
  // Functionality:
  // grid_function(size_type n);  // allocate memory
  // Elt \in {Vertex,Edge,Cell}       (2D)
  // Elt \in {Vertex,Edge,Facet,Cell} (3D)
  // T operator()(const Elt&) const; // get value
  // T& operator[](const Elt&);      // set value
  // maybe:
  // bool defined(const Elt& e) const; // is the mapping defined for e?

  typedef grid_types<Grid>            gt;
  typedef Grid                        grid_type;
  typedef typename gt::Vertex         Vertex;
  typedef typename gt::Edge           Edge;
  typedef typename gt::Cell           Cell;
  typedef typename gt::VertexIterator VertexIterator;
  typedef typename gt::EdgeIterator   EdgeIterator;
  typedef typename gt::CellIterator   CellIterator;

  typedef T                           value_type;


private:
  grid_function<Vertex,T> vgf;
  grid_function<Edge  ,T> egf;
  grid_function<Cell  ,T> cgf;

public:
  multi_grid_function() {}
  multi_grid_function(const grid_type& g) : vgf(g), egf(g), cgf(g) {}
  
  //--------------------- data access  ---------------------------

  T& operator[](const Vertex& V) { return vgf[V];}
  T& operator[](const Edge&   E) { return egf[E];}
  T& operator[](const Cell&   C) { return cgf[C];}
  const T& operator()(const Vertex& V) const { return vgf(V);}
  const T& operator()(const Edge&   E) const { return egf(E);}
  const T& operator()(const Cell&   C) const { return cgf(C);}

  const grid_type& TheGrid() const { return vgf.TheGrid();}

  const grid_function<Vertex,T>& TheVertexFunction() const { return vgf;}
  const grid_function<Edge,T>  & TheEdgeFunction()   const { return egf;}
  const grid_function<Cell,T>  & TheCellFunction()   const { return cgf;}
};



//----------------------------------------------------------------
//                partial_multi_grid_function
//----------------------------------------------------------------

template<class Grid, class T>
class partial_multi_grid_function {
  typedef grid_types<Grid>            gt;
  typedef Grid                        grid_type;
  typedef typename gt::Vertex         Vertex;
  typedef typename gt::Edge           Edge;
  typedef typename gt::Cell           Cell;
  typedef typename gt::VertexIterator VertexIterator;
  typedef typename gt::EdgeIterator   EdgeIterator;
  typedef typename gt::CellIterator   CellIterator;

  typedef T                           value_type;

private:
  partial_grid_function<Vertex,T> vgf;
  partial_grid_function<Edge  ,T> egf;
  partial_grid_function<Cell  ,T> cgf;

public:
  partial_multi_grid_function() {}
  partial_multi_grid_function(const grid_type& g) : vgf(g), egf(g), cgf(g) {}
  partial_multi_grid_function(const grid_type& g, const T& t) 
    : vgf(g,t), egf(g,t), cgf(g,t) {}
  
  //--------------------- data access  ---------------------------

  T& operator[](const Vertex& V) { return vgf[V];}
  T& operator[](const Edge&   E) { return egf[E];}
  T& operator[](const Cell&   C) { return cgf[C];}
  const T& operator()(const Vertex& V) const { return vgf(V);}
  const T& operator()(const Edge&   E) const { return egf(E);}
  const T& operator()(const Cell&   C) const { return cgf(C);}

  const grid_type& TheGrid() const { return vgf.TheGrid();}

  const partial_grid_function<Vertex,T>& TheVertexFunction() const { return vgf;}
  const partial_grid_function<Edge,T>  & TheEdgeFunction()   const { return egf;}
  const partial_grid_function<Cell,T>  & TheCellFunction()   const { return cgf;}

};


/*
template<class E, class T>
inline bool operator==(const grid_function<E,T>&,const grid_function<E,T>&)
{ return false;}

template<class E, class T>
inline bool operator<(const grid_function<E,T>&,const grid_function<E,T>&)
{ return false;}
*/

#endif
