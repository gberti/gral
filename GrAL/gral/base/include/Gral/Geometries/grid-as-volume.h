#ifndef GRAL_GB_BASE_GRID_AS_VOLUME_H
#define GRAL_GB_BASE_GRID_AS_VOLUME_H

// $LICENSE_NEC

#include "Gral/Base/common-grid-basics.h"
#include "Gral/Base/boundary.h"
#include "Gral/Geometries/geometric-types-nd.h"

#include "Geometry/geometric-shapes.h"
#include "Geometry/geometric-primitives.h"
#include "Geometry/intersections.h"
#include "Utility/ref-ptr.h"



/*! \brief View to a grid as spatial volume
    \ingroup geometricgridtools
     \see \ref geometricgridtools module
 
    This view allows point-in-grid queries, as well as intersection tests with 
    arbitrary (convex) polytopes. 
    It operates under the assumption that the elements of the grid are mapped to
    convex polytopes by the geometry.

    \note This does work only if the space dimension matches the grid dimension.
*/

template<class GEOM, class GRID = typename GEOM::grid_type, class GT = grid_types<GRID> >
class grid_as_volume : public geometric_shapes::geom_base {
public:
  typedef GRID grid_type;
  typedef GEOM geometry_type;
  typedef GEOM geom1_type;
  typedef GT   gt;
  typedef typename gt::Vertex Vertex;
  typedef typename gt::Cell   Cell;
  typedef typename geom1_type::coord_type coord_type;
private:
  ref_ptr<geom1_type const> geom;

public:
  grid_as_volume() {}
  grid_as_volume(geom1_type const&         geo) : geom(geo) {}
  grid_as_volume(ref_ptr<geom1_type const> geo) : geom(geo) {}

  ref_ptr<geom1_type const> TheGeometry() const { return geom;}
  ref_ptr<grid_type const>  TheGrid()     const { return ref_ptr<grid_type const>(geom->TheGrid());}

  bool is_inside(coord_type const& X) const
  {
    typedef space_point<coord_type> point_type;
    typedef polytope<GEOM, Cell>    polytope_type;

    // brute force approach: test for each cell if it contains pX
    // OK for small grids.
    // Alternatives:
    // 1) use the boundary of the grid to view it as a single polyhedron and use ray shooting
    //    - works for mwb grids, complexity: O(|Bd|) ~ O(|G|^{1 - 1/d})
    //    - problems with non-generic cases: ray passing through edges, vertices 
    // 2) Use octrees etc. 
    point_type pX(X);
    bool res = false;
    for(typename gt::CellIterator c(* TheGrid()); ! c.IsDone(); ++c) {
      polytope_type poly(TheGeometry(), *c);
      res = res || intersection_by_separating_axes<point_type, polytope_type>::intersects(pX, poly);
    }
    return res;
  }

  template<class POLY>
  intersection_result intersection_check(POLY const& p) const 
  {
    typedef polytope<GEOM, typename gt::Facet>  facet_polytope;
    BoundaryRange<grid_type> Bd(*TheGrid());
    typedef grid_types<BoundaryRange<grid_type> > bgt;
    bool intersects = false;
    for(typename bgt::FacetIterator bf = Bd.FirstFacet(); ! bf.IsDone(); ++bf) {
      facet_polytope bdfacet(TheGeometry(), *bf);
      intersects = intersects || intersection_by_separating_axes<POLY, facet_polytope>::intersects(p,bdfacet);
      if(intersects) 
	break;
    }

    intersection_result res;
    if(intersects)
      	res = geometric_shapes::geom_base::intersection;
    else {
      if(is_inside(p(*p.FirstVertex())))
	res = geometric_shapes::geom_base::inside;
      else
	res = geometric_shapes::geom_base::outside;
    }
    return res;

  }
  
  template<class ELEM2, class GEOM2> // class GT2 = grid_types<typename GEOM2::grid_type> >
  intersection_result intersection_check(ELEM2 const& e2, GEOM2 const& geom2) const 
  {
    polytope<GEOM2, ELEM2> p(geom2, e2);
    return intersection_check(p);
  }
    
};

#endif
