#ifndef GRAL_GB_COMPLEX3D_GRIDFUNCTIONS_H
#define GRAL_GB_COMPLEX3D_GRIDFUNCTIONS_H

// $LICENSE_NEC

#include "Gral/Grids/Complex3D/complex3d.h"

#include "Gral/Base/grid-function-vector.h"
#include "Gral/Base/grid-function-hash.h"

namespace GrAL {

/*! \defgroup complex3dgf Total grid functions for Complex3D
 
    \ingroup complex3dmodule

    The specializations in this module are models of $GrAL TotalGridFunction.

*/

/*! \brief Specialization of the grid_function primary template
    \ingroup complex3dgf
    
     Model of $GrAL TotalGridFunction

     The implementation uses vectors.
 */
template<class T>
class grid_function<Vertex_Complex3D, T>
  : public grid_function_vector<Vertex_Complex3D,T>
{
  typedef grid_function_vector<Vertex_Complex3D,T> base;
 public:
  typedef typename base::grid_type grid_type;

  grid_function() {}
  grid_function(grid_type const& gg) : base(gg) {}
  grid_function(grid_type const& gg, T const& t) : base(gg,t) {}

};


/*! \brief Specialization of the grid_function primary template
    \ingroup complex3dgf
    
     Model of $GrAL TotalGridFunction

     The implementation uses vectors.
 */
template<class T>
class grid_function<Cell_Complex3D, T>
  : public grid_function_vector<Cell_Complex3D,T>
{
  typedef grid_function_vector<Cell_Complex3D,T> base;
 public:
  typedef typename base::grid_type grid_type;

  grid_function() {}
  grid_function(grid_type const& gg) : base(gg) {}
  grid_function(grid_type const& gg, T const& t) : base(gg,t) {}

};


/*! \brief Specialization of the grid_function primary template
    \ingroup complex3dgf
    
     Model of $GrAL TotalGridFunction

     The implementation uses hash tables.
 */
template<class T>
class grid_function<Complex3D::Edge, T>
  : public grid_function_hash<Complex3D::Edge,T>
{
  typedef grid_function_hash<Complex3D::Edge,T> base;
 public:
  typedef typename base::grid_type grid_type;

  grid_function() {}
  grid_function(grid_type const& gg) : base(gg) {}
  grid_function(grid_type const& gg, T const& t) : base(gg,t) {}

};

/*! \brief Specialization of the grid_function primary template
    \ingroup complex3dgf
    
     Model of $GrAL TotalGridFunction.

     The implementation uses hash tables.
 */
template<class T>
class grid_function<Complex3D::Facet, T>
  : public grid_function_hash<Complex3D::Facet,T>
{
  typedef grid_function_hash<Complex3D::Facet,T> base;
 public:
  typedef typename base::grid_type grid_type;

  grid_function() {}
  grid_function(grid_type const& gg) : base(gg) {}
  grid_function(grid_type const& gg, T const& t) : base(gg,t) {}

};

} // namespace GrAL 

#endif
