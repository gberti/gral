#ifndef GRAL_GB_GRAL_GEOMETRY_INTERSECTION_RAY_GRID_C
#define GRAL_GB_GRAL_GEOMETRY_INTERSECTION_RAY_GRID_C


/* ------------------------------------------------------------

    Copyright (C) 2001 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include "Gral/Geometry/intersection-ray-grid.h"
#include "Gral/Base/common-grid-basics.h"

#include "Geometry/geometric-primitives.h"
#include "Container/tuple.h"

#include <algorithm> // sort

namespace GrAL {

struct compare_second {
  template<class PAIR>
  bool operator()(PAIR const& x,
		  PAIR const& y) const 
    { return (x.second < y.second);}
};

template<class RAY, class GRID2D, class GEOM3D, class ARRAY>
bool intersection_ray_grid(RAY     const& ray, 
			   GRID2D  const& G,
			   GEOM3D  const& Geom,
			   ARRAY        & intersections) // seq of pair<t,cell>
{
  // static check: GRID is a 2D grid
  typedef grid_types<GRID2D> gt;
  typedef point_traits<typename GEOM3D::coord_type> pt;
  //typedef typename pt::real_type               real;
  typedef typename pt::component_type               real;
  typedef typename gt::CellIterator CellIterator;
  typedef typename ARRAY::value_type  intersection_pair;

  int n_is = 0; // number of intersections
  for(CellIterator c(G.FirstCell()); ! c.IsDone(); ++c) {
    real t;
    if (intersection_ray_cell(ray,*c,Geom,t)) {
      ++n_is;
      intersections.push_back(intersection_pair(t,*c));
    }
  }
  std::sort(intersections.begin(),
	    intersections.end(),
	    compare_second());
  return (n_is > 0);
}

  template<class RAY, class CELL2D, class GEOM, class REAL>
  bool intersection_ray_cell(RAY     const& ray, 
			     CELL2D  const& c,
			     GEOM    const& geom,
			     REAL          & intersection) 
{
  typedef typename GEOM::coord_type                  coord_type;
  typedef triangle<coord_type>                       triangle_t;
  typedef intersection_ray_triangle<RAY, triangle_t> rti_type;
  typedef element_traits<CELL2D> et;
  typedef typename et::grid_type grid_type;
  typedef grid_types<grid_type>  gt;
  typedef typename gt::Vertex    Vertex;

  bool res = false;
  if(c.NumOfVertices() == 3) {
    tuple<Vertex,3> v(c.FirstVertex(),c.EndVertex());
    triangle_t tr(geom.coord(v[0]),
		  geom.coord(v[1]),
		  geom.coord(v[2]));
    rti_type rti(ray, // segment_t (coo(e.v1()), coo(e.v2())),
		 tr);
    if(rti.ray_intersects_triangle()) {
      res =  true;
      intersection = rti.ray_intersection(); // single point
    }
  }
  else if(c.NumOfVertices() == 4) {
    // should use bilinear geometry ...
    tuple<Vertex,4> v(c.FirstVertex(),c.EndVertex());
    for(int i = 0; i < 4; ++i) {
      int k = (i+1) % 4;
      int j = (i+2) % 4;
      triangle_t t(geom.coord(v[i]), geom.coord(v[j]), geom.coord(v[k]));

      rti_type rti(ray,t);
      bool found = rti.ray_intersects_triangle();
      if(found) {
	intersection  = rti.ray_intersection();
      }
      res |= found;
    }
  }
  return res;
}

} // namespace GrAL

#endif
