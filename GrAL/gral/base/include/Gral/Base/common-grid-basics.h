#ifndef GRAL_BASE_GB_COMMON_GRID_BASICS_H
#define GRAL_BASE_GB_COMMON_GRID_BASICS_H


//----------------------------------------------------------------
//   (c) Guntram Berti, 1997
//   Chair for Numerical Mathematics & Scientific Computing (NMWR)
//   TU Cottbus - Germany
//   http://math-s.math.tu-cottbus.de/NMWR
//   
//----------------------------------------------------------------


/*! \file
  \brief This file bundles the basic template frames to be
         filled in where concrete grids are defined.

   This traits- technique is very handy when used in 
   template functions or classes where the grid type is a parameter.
*/

// parameterized namespaces: access to grid related types.
#include "Gral/Base/grid-types.h"


// empty frame for grid function template grid_function<E,T>
#include "Gral/Base/grid-functions.h"

// empty frame for type tags tp<E>
#include "Gral/Base/type-tags.h"

// empty frame for deducing types from elements
#include "Gral/Base/element-traits.h"

// empty frame for boundary iteration
#include "Gral/Base/boundary.h"

#endif
