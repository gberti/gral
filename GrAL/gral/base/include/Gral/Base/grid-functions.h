#ifndef NMWR_GB_GRIDFUNCTIONS_H
#define NMWR_GB_GRIDFUNCTIONS_H


//----------------------------------------------------------------
//   (c) Guntram Berti, 1997
//   Chair for Numerical Mathematics & Scientific Computing (NMWR)
//   TU Cottbus - Germany
//   http://math-s.math.tu-cottbus.de/NMWR
//   
//----------------------------------------------------------------

#include "compiler-config.h"
#include "Grids/common-grid-basics.h"

// empty template frame, to be specialized for
// concrete types of E = grid-elements of a grid-type
// e.g. E = Complex2D::Vertex , E = RegGrid2D::Edge, ...
// T is a user-selected type
// grid_function<E,T> has the semantics of a mapping E -> T

template<class E, class T>
class grid_function;
  // Functionality:
  // grid_function(size_type n);  // allocate memory
  // T operator()(const E&) const; // get value
  // T& operator[](const E&);      // set value
  // maybe:
  // bool defined(const E& e) const; // is the mapping defined for e?



// sample usage:
// grid_function<Vertex,int> vnum(G.NumOfVertices());
// for(vit = G.FirstVertex(), i = 0; ! vit.IsDone(); ++i, ++vit)
//    vnum[*vit] = i;
// ....
//  
//  A(vnum(V),vnum(W)) = a(Phi(V),Phi(W)); 


template<class E, class T>
class partial_grid_function;



template<class E, class T>
inline bool operator==(const grid_function<E,T>&,const grid_function<E,T>&)
{ return false;}

template<class E, class T>
inline bool operator<(const grid_function<E,T>&,const grid_function<E,T>&)
{ return false;}

#endif
