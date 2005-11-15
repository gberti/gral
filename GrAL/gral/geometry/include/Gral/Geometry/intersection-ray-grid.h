#ifndef GRAL_GB_GRAL_GEOMETRY_INTERSECTION_RAY_GRID_H
#define GRAL_GB_GRAL_GEOMETRY_INTERSECTION_RAY_GRID_H

// $LICENSE_NEC

namespace GrAL {

  /*! \brief Calculate all intersections of a ray with a surface grid
    
  
  \todo Implement test cases, test rays passing through
  vertices and edges.
  \todo The functionality can be easily be extended to 
  point-inclusion test for the boundary of a 3D grid
  (odd number of intersections for some ray starting at query point).
    
  */
  template<class RAY, class GRID2D, class GEOM3D, class ARRAY>
  bool intersection_ray_grid(RAY     const& ray, 
			     GRID2D  const& G,
			     GEOM3D  const& Geom,
			     ARRAY        & intersections); // seq of pair<t,cell>

  /*! \brief Compute first intersection of ray with 2D cell
   */
  template<class RAY, class CELL2D, class GEOM, class REAL>
  bool intersection_ray_cell(RAY     const& ray, 
			     CELL2D  const& c,
			     GEOM    const& Geom,
			     REAL         & intersection);


} // namespace GrAL

#ifdef NMWR_INCLUDE_TEMPLATE_DEFS
#include "Gral/Geometry/intersection-ray-grid.C"
#endif

#endif
