#ifndef GRAL_GB_CARTESIAN3D_GRIDFUNCTIONS_H
#define GRAL_GB_CARTESIAN3D_GRIDFUNCTIONS_H

// $LICENSE

#include "Gral/Grids/Cartesian3D/cartesian3d.h"
#include "Gral/Base/grid-function-vector.h"

/*! \defgroup cartesian3dgf Total grid functions for CartesianGrid3D
    \ingroup cartesian3dmodule

     \see $GrAL TotalGridFunction
     \see Test in \ref test-cart-gf.C
 */


// use generic version for total grid functions

/*! \brief Specialization of the grid_function primary template for CartesianGrid3D::Vertex
    
    \ingroup cartesian3dgf
 */
template<class T>
class grid_function<cartesian3d::Vertex_Cartesian3D, T>
  : public grid_function_vector<cartesian3d::Vertex_Cartesian3D,T>
{
  typedef grid_function_vector<cartesian3d::Vertex_Cartesian3D,T> base;
 public:
  typedef typename base::grid_type grid_type;

  grid_function() {}
  grid_function(grid_type const& gg) : base(gg) {}
  grid_function(grid_type const& gg, T const& t) : base(gg,t) {}

};


/*! \brief Specialization of the grid_function primary template for CartesianGrid3D::Edge
    
    \ingroup cartesian3dgf
 */
template<class T>
class grid_function<cartesian3d::Edge_Cartesian3D, T>
  : public grid_function_vector<cartesian3d::Edge_Cartesian3D,T>
{
  typedef grid_function_vector<cartesian3d::Edge_Cartesian3D,T> base;
 public:
  typedef typename base::grid_type grid_type;

  grid_function() {}
  grid_function(grid_type const& gg) : base(gg) {}
  grid_function(grid_type const& gg, T const& t) : base(gg,t) {}

};

/*! \brief Specialization of the grid_function primary template for CartesianGrid3D::Facet
    
    \ingroup cartesian3dgf
 */
template<class T>
class grid_function<cartesian3d::Facet_Cartesian3D, T>
  : public grid_function_vector<cartesian3d::Facet_Cartesian3D,T>
{
  typedef grid_function_vector<cartesian3d::Facet_Cartesian3D,T> base;
 public:
  typedef typename base::grid_type grid_type;

  grid_function() {}
  grid_function(grid_type const& gg) : base(gg) {}
  grid_function(grid_type const& gg, T const& t) : base(gg,t) {}

};


/*! \brief Specialization of the grid_function primary template for CartesianGrid3D::Cell
    
    \ingroup cartesian3dgf
 */
template<class T>
class grid_function<cartesian3d::Cell_Cartesian3D, T>
  : public grid_function_vector<cartesian3d::Cell_Cartesian3D,T>
{
  typedef grid_function_vector<cartesian3d::Cell_Cartesian3D,T> base;
 public:
  typedef typename base::grid_type grid_type;

  grid_function() {}
  grid_function(grid_type const& gg) : base(gg) {}
  grid_function(grid_type const& gg, T const& t) : base(gg,t) {}

};



#endif
