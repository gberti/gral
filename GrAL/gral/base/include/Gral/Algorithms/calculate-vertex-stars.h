#ifndef GRAL_GB_BASE_CALCULATE_VERTEX_STARS_H
#define GRAL_GB_BASE_CALCULATE_VERTEX_STARS_H

// $LICENSE_NEC

/*! \defgroup calc_vertex_stars Vertex star determination
    \ingroup combinatoric_grid_algorithms

    \brief Algorithms for determining the star of grid vertices,
    possibly oriented

    A vertex star is the set of higher-dimensional elements incident
    to the vertex.
 */

/*! \brief Get a germ flag for the star of each vertex of G
   \ingroup calc_vertex_stars
   \partof  calc_vertex_stars

   The routine calculates a flag for each vertex, that is,
   a tuple containing the vertex, an edge, up to a cell,
   each mutually incident. This flag can serve as starting point
   for obtaining \e all incident elements of the vertex,
   requiring cell-cell adjacency information.

   The algorithm uses only downward incidence iterators.
   It can be used e.g. for implementing upward incidence iterators
   on vertices with minimal storage requirements.  

   \templateparams
   - GRID: a 2D grid type, having types
      \c VertexOnFacetIterator and \c FacetOnCellIterator
   - GERMS: mapping of vertices to flags, for example
      \c grid_function<GRID::Vertex, flag<GRID> >

   \todo Works only for 2D meshes.
 */
template<class GRID, class GERMS>
void calculate_vertex_stars
(GRID const& G,      // 2D, e.g. archetype of 3D cell
 GERMS &     germs); // e.g gf: Vertex -> flag<GRID>

/*! \brief  Get a germ flag for the star of each vertex of G
    \ingroup calc_vertex_stars
    \partof  calc_vertex_stars

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
