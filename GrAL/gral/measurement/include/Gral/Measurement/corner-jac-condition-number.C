#ifndef GRAL_GB_GRID_QUALITY_CORNER_JAC_CONDITION_NUMBER_C
#define GRAL_GB_GRID_QUALITY_CORNER_JAC_CONDITION_NUMBER_C

// $LICENSE_NEC

#include "Gral/Measurement/corner-jac-condition-number.h"
#include "Geometry/algebraic-primitives.h"

#include "Gral/Base/cell-archetype-map.h"
#include "Gral/Algorithms/calculate-vertex-stars.h"

template<class GRID, class GEOM>
void 
corner_jacobian<GRID,GEOM>::set_vertex_stars
(corner_jacobian<GRID,GEOM>::archetype_type const& A) 
{
  vertex_stars[&A] = vertex_star_map(A);
  calculate_vertex_stars(A, vertex_stars[&A]);
}

template<class GRID, class GEOM>
typename corner_jacobian<GRID,GEOM>::real
corner_jacobian<GRID,GEOM>
::condition(typename corner_jacobian<GRID,GEOM>::VertexOnCellIterator 
	    const& corner)
{
  archetype_type const& A(corner.TheCell().TheArchetype());
  if(! vertex_stars.defined(&A))
     set_vertex_stars(A);

  cell_archetype_map<grid_type>  cXa(corner.TheCell());
  Vertex v0 = *corner;
  ArchVertex v0_a(cXa(v0));
  flag<archetype_type>   star = vertex_stars(&A)(v0_a);
  
  // could use a star or interval iterator
  Vertex v1 = cXa(star.switched_vertex());
  star.switch_edge();
  Vertex v2 = cXa(star.switched_vertex());
  star.switch_face();
  star.switch_edge();
  Vertex v3 = cXa(star.switched_vertex()); 

  typedef algebraic_primitives<typename geom_type::coord_type> ap;
  // typename ap::Norm_frobenius N;
  double  q = ap::condition(TheGeom().coord(v0)- TheGeom().coord(v1),
			    TheGeom().coord(v0)- TheGeom().coord(v2),
			    TheGeom().coord(v0)- TheGeom().coord(v3),
			    typename ap::Norm_frobenius());
  return q;
}

#endif
