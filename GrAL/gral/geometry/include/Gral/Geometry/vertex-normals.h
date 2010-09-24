#ifndef GRAL_GB_GEOMETRY_VERTEX_NORMAL_H
#define GRAL_GB_GEOMETRY_VERTEX_NORMAL_H


/* ------------------------------------------------------------

    Copyright (C) 2004 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */

#include "Gral/Base/common-grid-basics.h"
#include "Gral/Geometry/enclosed-volume.h"

#include "Geometry/algebraic-primitives.h"


namespace GrAL {

  /*! \defgroup normals Computation of normal vectors
     
       \ingroup geometric_measures
  */

  /*! \brief Compute the normal of a surface cell

     \ingroup normals
     \pre G.dimension() + 1 == GeomG.spacedim() == 3

      This code does only work for 2D grids in 3D space.
     
  */
  template<class GRID, class GEOM, class CELL>
  typename GEOM::coord_type cell_normal(GRID const& G, GEOM const& GeomG, CELL const& c)
  {
    typedef grid_types<GRID>                 gt;
    typedef typename GEOM::coord_type        coord_type;
    typedef algebraic_primitives<coord_type> ap;

    typename gt::VertexOnCellIterator v = begin_x(c);
    coord_type v1  = GeomG.coord(*v);
    coord_type v2  = GeomG.coord(*++v);
    coord_type ctr = GeomG.barycenter(c);
    return ap::normalization(ap::vectorproduct(v1-ctr, v2-ctr));
  }


  /*! \brief Compute vertex-based normals of a grid

  \ingroup normals

  This function computes an approximation to the normal of each vertex
  of the input grid. As the surface given by a piecewise linear grid
  is in general not differentiable at vertices, there is no unambigous
  definition of the normal of a vertex.
  The algorithm used in this function simply averages the normals of the surrounding cells.

  \pre \c G is oriented
  \post vertex_normals maps each vertex to its approximated normals

   */
  template<class GRID, class GEOM, class GF>
  void compute_vertex_normals_oriented(GRID const& G, GEOM const& GeomG, GF & vertex_normals)
  {
    // REQUIRE_ALWAYS(is_oriented(G), "Input grid must be oriented!", 1);

    typedef grid_types<GRID>          gt;
    typedef typename GEOM::coord_type coord_type;
    typedef algebraic_primitives<coord_type> ap;
    for(typename gt::CellIterator c = begin_x(G); !c.IsDone(); ++c) {
      coord_type normal = cell_normal(G,GeomG,*c);
      for(typename gt::VertexOnCellIterator vc(*c); !vc.IsDone(); ++vc)
	vertex_normals[*vc] += normal;
    }
    for(typename gt::VertexIterator v = begin_x(G); !v.IsDone(); ++v)
      ap::normalize(vertex_normals[*v]);
  }

  /*! \brief Compute vertex normals

     \ingroup normals

     \pre  \c G is orientable
     \post \c vertex_normals maps each vertex to its approximated normals
     \post \c G is oriented iff result is true
     \return true if G could be oriented

  */
  template<class GRID, class GEOM, class GF>
  bool compute_vertex_normals(GRID & G, GEOM const& GeomG, GF & vertex_normals) 
  {
    bool res = fix_orientation(G);
    compute_vertex_normals_oriented(G,GeomG,vertex_normals);
    return res;
  }

  /*! \brief Compute outer vertex normals

     \ingroup normals

     \pre  \c G is orientable
     \pre  \c G is a closed manifold
     \post \c vertex_normals maps each vertex to its approximated normals pointing outward
     \post \c G is oriented positively (enclosed_volume(G,GeomG) > 0) iff result is true
     \return true if G could be oriented
  */
  template<class GRID, class GEOM, class GF>
  bool compute_outer_vertex_normals(GRID & G, GEOM const& GeomG, GF & vertex_normals) 
  {
    bool res = fix_geometric_orientation(G,GeomG);
    compute_vertex_normals_oriented(G,GeomG,vertex_normals);
    return res;
  }



}

#endif
