#ifndef GRAL_GB_GRIDS_TRIANG3D_GRIDFUNCTION_H
#define GRAL_GB_GRIDS_TRIANG3D_GRIDFUNCTION_H


/* ------------------------------------------------------------

    Copyright (C) 2001 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include "Gral/Grids/Triang3D/triang3d.h"
#include "Gral/Grids/Triang3D/element-traits.h"

// generic grid functions
#include "Gral/Base/grid-function-vector.h"
#include "Gral/Base/grid-function-hash.h"

/*! \file
 */

namespace GrAL {

/*! \defgroup triang3dgf Total grid functions for Triang3D
    \ingroup triang3dmodule
 
    The specializations of grid_function in this group are
    models of $GrAL TotalGridFunction.
 */

/*! \brief Specialization of the grid_function primary template for Triang3D::Vertex
    \ingroup triang3dgf
 */template<class T>
class grid_function<Triang3D_Vertex, T> 
  : public grid_function_vector<Triang3D_Vertex, T> 
{
  typedef grid_function_vector<Triang3D_Vertex, T> base;
public:
  typedef typename base::grid_type grid_type;

  grid_function() {}
  grid_function(grid_type const& gg) : base(gg) {}
  grid_function(grid_type const& gg, T const& t) : base(gg,t) {}
};


/*! \brief Specialization of the grid_function primary template for Triang3D::Cell
    \ingroup triang3dgf
 */
template<class T>
class grid_function<Triang3D_Cell, T> 
  : public grid_function_vector<Triang3D_Cell, T> 
{
  typedef grid_function_vector<Triang3D_Cell, T> base;
public:
  typedef typename base::grid_type grid_type;

  grid_function() {}
  grid_function(grid_type const& gg) : base(gg) {}
  grid_function(grid_type const& gg, T const& t) : base(gg,t) {}
};



/*! \brief Specialization of the grid_function primary template for Triang3D::Edge

    \ingroup triang3dgf

    \note This class uses a hash-based implementation.
 */
template<class T>
class grid_function<Triang3D::Edge, T>
  : public grid_function_hash<Triang3D::Edge,T>
{
  typedef grid_function_hash<Triang3D::Edge,T> base;
 public:
  typedef typename base::grid_type grid_type;

  grid_function() {}
  grid_function(grid_type const& gg) : base(gg) {}
  grid_function(grid_type const& gg, T const& t) : base(gg,t) {}

};

/*! \brief Specialization of the grid_function primary template for Triang3D::Facet

    \ingroup triang3dgf

    \note This class uses a hash-based implementation.
 */
template<class T>
class grid_function<Triang3D::Facet, T>
  : public grid_function_hash<Triang3D::Facet,T>
{
  typedef grid_function_hash<Triang3D::Facet,T> base;
 public:
  typedef typename base::grid_type grid_type;

  grid_function() {}
  grid_function(grid_type const& gg) : base(gg) {}
  grid_function(grid_type const& gg, T const& t) : base(gg,t) {}

};

} // namespace GrAL 


#endif


