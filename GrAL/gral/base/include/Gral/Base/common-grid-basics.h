#ifndef NMWR_GB_COMMON_GRID_BASICS_H
#define NMWR_GB_COMMON_GRID_BASICS_H


//----------------------------------------------------------------
//   (c) Guntram Berti, 1997
//   Chair for Numerical Mathematics & Scientific Computing (NMWR)
//   TU Cottbus - Germany
//   http://math-s.math.tu-cottbus.de/NMWR
//   
//----------------------------------------------------------------


//----------------------------------------------------------------
//
// This bundles the basic template frames to be
// filled in where concrete grids are defined.
// This traits-related technique is very handy when used in 
// template functions or classes where the grid type is a parameter.
//
//----------------------------------------------------------------

// parameterized namespaces: access to grid related types.
#include "Grids/grid-types.h"

// obsolete: geoemtry is a template param. of its own
//template<class Grid>
//struct geometric_primitives {};


// empty frame for grid function template grid_function<E,T>
#include "Grids/grid-functions.h"

// empty frame for type tags tp<E>
#include "Grids/type-tags.h"

// empty frame for deducing types from elements
#include "Grids/element-traits.h"

// empty frame for boundary iteration
#include "Grids/boundary.h"

#endif
