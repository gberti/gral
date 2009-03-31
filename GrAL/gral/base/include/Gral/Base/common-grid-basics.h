#ifndef GRAL_BASE_GB_COMMON_GRID_BASICS_H
#define GRAL_BASE_GB_COMMON_GRID_BASICS_H




/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */



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

/*! \brief Enclosing namespace for all components of GrAL.
 */
namespace GrAL { }

#endif
