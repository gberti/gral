#ifndef GRAL_GB_BASE_CALCULATE_VERTEX_STARS_H
#define GRAL_GB_BASE_CALCULATE_VERTEX_STARS_H

// $LICENSE_NEC

/*! Get a germ flag for the star of each vertex of G

   The routine calculates a flag for each vertex, that is,
   a tuple containing the vertex, an edge, up to a cell,
   each mutually incident. This flag can serve as starting point
   for obtaining \e all incident elements of the vertex,
   requiring cell-cell adjacency information.

   The algorithm uses only downward incidence iterators.
   It can be used e.g. for implementing upward incidence iterators
   on vertices with minimal storage requirements.  

   \todo Works only for 2D meshes.
 */
template<class GRID, class GERMS>
void calculate_vertex_stars
(GRID const& G,      // 2D, e.g. archetype of 3D cell
 GERMS &     germs); // e.g gf: Vertex -> flag<GRID>

/*!  Get a germ flag for the star of each vertex of G

    This is similar to calculate_vertex_stars, but it
    checks that the sequence of incident edges of  each 
    vertex has positive orientation (with respect to \c jacc).

 */
template<class GRID, class GERMS, class DET>
void calculate_positive_vertex_stars
(GRID const& G,      // 2D, e.g. archetype of 3D cell
 GERMS     & germs,  // e.g gf: Vertex -> flag<GRID>
 DET  const& det);    // corner jac. of archetype

#ifdef NMWR_INCLUDE_TEMPLATE_DEFS
#include "Gral/Algorithms/calculate-vertex-stars.C"
#endif

#endif
