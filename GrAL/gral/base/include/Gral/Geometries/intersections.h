#ifndef GRAL_GB_GEOMETRIES_INTERSECTIONS_H
#define GRAL_GB_GEOMETRIES_INTERSECTIONS_H

// $LICENSE 

#include "Geometry/algebraic-primitives.h"
#include "Geometry/geometric-primitives.h"

/*! brief Test whether an edge and a facet do intersect.

    \ingroup geometricgridtools
  
    \templateparams
     - Geom: Model of $GrAL VertexGridGeometry

    \todo This works only for linear edges and
    facets with 3 or 4 vertices.
    In the 3-vertex case, a linear facet is assumed.
    In the 4-vertex case, the convex hull of the vertices
    (a tetrahedron) is checked for intersection with the edge. 

    \see Module geometricgridtools
  
 */
template<class GEOM, class GT = grid_types<typename GEOM::grid_type> >
class edge_facet_intersection
{
  typedef GEOM geom_type;
  typedef typename GT::grid_type grid_type;
  typedef typename geom_type::coord_type coord_type;
  typedef typename GT::Vertex Vertex;
  typedef typename GT::Edge   Edge;
  typedef typename GT::Facet  Facet;


  typedef segment <coord_type> segment_t;
  typedef triangle<coord_type> triangle_t;
  typedef intersection_segment_triangle<segment_t, triangle_t> sti_type;


  geom_type const* geom;

  coord_type is_point; // current intersection point
public:
  edge_facet_intersection(geom_type const& geo) : geom(&geo) {}

  // should check whether is_point is the correct one. 
  coord_type const& location() const { return is_point;}

  //! returns true if e and f intersect
  bool operator()(Edge const& e, Facet const& f);
private:
  // shorthand
  coord_type const& coo(Vertex const& v) const { return geom->coord(v);}
};


#ifdef NMWR_INCLUDE_TEMPLATE_DEFS
#include "Gral/Geometries/internal/intersections.C"
#endif


#endif
