#ifndef GRAL_GB_GRIDS_TRIANG2D_GRIDFUNCTION_H
#define GRAL_GB_GRIDS_TRIANG2D_GRIDFUNCTION_H


/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti, 
    Contact: guntram AT users DOT berlios DOT de

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


/*! \file
 */

#include "Gral/Grids/Triang2D/triang2d.h"
#include "Gral/Grids/Triang2D/element-traits.h"

// generic grid functions
#include "Gral/Base/grid-function-vector.h"
#include "Gral/Base/grid-function-hash.h"

#include "Utility/ref-ptr.h"

namespace GrAL {

/*! \defgroup triang2dgf Total grid functions for Triang2D
    \ingroup triang2dmodule
 
    The specializations of grid_function in this group are
    models of $GrAL TotalGridFunction.
 */

/*! \brief Specialization of the grid_function primary template for Triang2D::Vertex
    \ingroup triang2dgf
 */
template<class T>
class grid_function<Triang2D_Vertex, T> 
  : public grid_function_vector<Triang2D_Vertex, T> 
{
  typedef grid_function_vector<Triang2D_Vertex, T> base;
public:
  typedef typename base::grid_type grid_type;

  grid_function() {}
  grid_function(grid_type const& gg) : base(gg) {}
  grid_function(grid_type const& gg, T const& t) : base(gg,t) {}
  grid_function(ref_ptr<grid_type const> gg) : base(gg) {}
  grid_function(ref_ptr<grid_type const> gg, T const& t) : base(gg,t) {}
};


/*! \brief Specialization of the grid_function primary template for Triang2D::Cell
    \ingroup triang2dgf
 */
template<class T>
class grid_function<Triang2D_Cell, T> 
  : public grid_function_vector<Triang2D_Cell, T> 
{
  typedef grid_function_vector<Triang2D_Cell, T> base;
public:
  typedef typename base::grid_type grid_type;

  grid_function() {}
  grid_function(grid_type const& gg) : base(gg) {}
  grid_function(grid_type const& gg, T const& t) : base(gg,t) {}
  grid_function(ref_ptr<grid_type const> gg) : base(gg) {}
  grid_function(ref_ptr<grid_type const> gg, T const& t) : base(gg,t) {}
};



/*! \brief Specialization of the grid_function primary template for Triang2D::Edge

    \ingroup triang2dgf

    \note This class uses a hash-based implementation.
 */
template<class T>
class grid_function<Triang2D::Edge, T> 
  : public grid_function_hash<Triang2D::Edge, T> 
{
  typedef grid_function_hash<Triang2D::Edge, T> base;
public:
  typedef typename base::grid_type grid_type;

  grid_function() {}
  grid_function(grid_type const& gg) : base(gg) {}
  grid_function(grid_type const& gg, T const& t) : base(gg,t) {}

};


} // namespace GrAL 

#endif


