#ifndef GRAL_GB_GRID_QUALITY_CORNER_JAC_CONDITION_NUMBER_C
#define GRAL_GB_GRID_QUALITY_CORNER_JAC_CONDITION_NUMBER_C

// $LICENSE_NEC

#include "Gral/Measurement/corner-jac-condition-number.h"
#include "Geometry/algebraic-primitives.h"

#include "Gral/Base/cell-archetype-map.h"
#include "Gral/Algorithms/calculate-vertex-stars.h"

namespace GrAL {

namespace measurement {

template<class GRID, class GEOM>
void 
corner_jacobian<GRID,GEOM>::set_vertex_stars
(corner_jacobian<GRID,GEOM>::archetype_type const& A) 
{
  vertex_stars[&A] = vertex_star_map(A);
  calculate_vertex_stars(A, vertex_stars[&A]);
}

template<class GRID, class GEOM>
void 
corner_jacobian<GRID,GEOM>::set_ideal_corner
(typename corner_jacobian<GRID,GEOM>::VertexOnCellIterator const& ideal_corner_, 
 typename corner_jacobian<GRID,GEOM>::geom_type            const& ideal_geom_)
{
  ideal_geom = & ideal_geom_;
  matrix_type edges;
  get_edges(ideal_corner_, ideal_geom_, edges);
  inv_ideal_corner = ap::inverse(edges);
}

template<class GRID, class GEOM>
void 
corner_jacobian<GRID,GEOM>::get_edges
(typename corner_jacobian<GRID,GEOM>::VertexOnCellIterator const& corner_, 
 typename corner_jacobian<GRID,GEOM>::geom_type            const& geom_,
 typename corner_jacobian<GRID,GEOM>::matrix_type               & edges)
{
  archetype_type const& A(corner_.TheCell().TheArchetype());
  if(! vertex_stars.defined(&A))
     set_vertex_stars(A);

  cell_archetype_map<grid_type>  cXa(corner_.TheCell());
  Vertex v0 = *corner_;
  ArchVertex v0_a(cXa(v0));
  flag<archetype_type>   star = vertex_stars(&A)(v0_a);
  
  // could use a star or interval iterator
  Vertex v1 = cXa(star.switched_vertex());
  star.switch_edge();
  Vertex v2 = cXa(star.switched_vertex());
  star.switch_face();
  star.switch_edge();
  Vertex v3 = cXa(star.switched_vertex()); 

  edges = matrix_type(geom_.coord(v0) - geom_.coord(v1),
		      geom_.coord(v0) - geom_.coord(v2),
		      geom_.coord(v0) - geom_.coord(v3));
}



template<class GRID, class GEOM>
typename corner_jacobian<GRID,GEOM>::real
corner_jacobian<GRID,GEOM>
::condition(typename corner_jacobian<GRID,GEOM>::VertexOnCellIterator 
	    const& corner)
{
  matrix_type edges;
  get_edges(corner, *geom, edges);

  matrix_type weighted_edges = edges * inv_ideal_corner;
  double q = ap::condition(weighted_edges,
			   typename ap::Norm_frobenius());
  return q;
}

} // namespace measurement;

} // namespace GrAL 

#endif
