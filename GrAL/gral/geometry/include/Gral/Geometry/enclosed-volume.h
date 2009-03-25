#ifndef GRAL_GB_GEOMETRY_ENCLOSED_VOLUME
#define GRAL_GB_GEOMETRY_ENCLOSED_VOLUME


/* ------------------------------------------------------------

    Copyright (C) 2008 - 2009 Guntram Berti, 
    Contact: guntram AT users DOT berlios DOT de

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include "Gral/Base/common-grid-basics.h"
#include "Gral/Algorithms/orientation.h"
#include "Gral/Algorithms/manifold-check.h"
#include "Geometry/algebraic-primitives.h"



namespace GrAL {

 
 //FIXME: move to grid-types.h
  /*

  template<class GEOM>
  struct real_type { 
    typedef typename point_traits<typename GEOM::coord_type>::real_type real_type;
  };


  // FIXME: restrict with boost::enable_if
  template<class CELL, class GEOM>
  inline hyper_plane<typename GEOM::coord_type>
  tangent_plane(CELL const& c, GEOM const& geom)
  {
    // 2D case.
    typedef grid_types<typename element_traits<CELL>::grid_type> gt;
    typename gt::VertexOnCellIterator vc = begin_x(c);
    coord_type p0 = geom.coord(*vc);
    ++vc;
    coord_type p1 = geom.coord(*vc);
    ++vc;
    coord_type p2 = geom.coord(*vc);
    return hyper_plane<coord_type>(p1, p2-p1,p0-p1); 
  }

  template<class P, class CELL, class GEOM>
  typename real_type<GEOM>::type
  oriented_height(P const& p, CELL const& c, GEOM const& geom)
  {
    hyper_plane<coord_type> = tangent_plane(c, geom);
    return oriented_distance(p, hp);
  } 


  template<class CELL, class GEOM>
  typename real_type<GEOM>::type
  oriented_volume(CELL const& c, GEOM const& geom)
  { return geom.oriented_volume(c); }
  */


  /*! \brief Find volume enclosed by surface mesh

    \ingroup geometricmeasures

    \pre G is a manifold mesh (without boundary)
    \pre G is oriented
    \pre dim(G) = spacedim(GeomG) - 1
    \result The volume of the region enclosed by G.

   \note Could also pass orientation as a third parameter (GF with values in {+1,-1})
  */
  template<class GRID, class GEOM>
  double
 // typename point_traits<typename GEOM::coord_type>::scalar_type
  //  typename real_type<GEOM>::type
  enclosed_volume_oriented(GRID const& G, GEOM const& GeomG)
  {
    //  REQUIRE_ALWAYS(is_manifold(G), "Grid must be a manifold without boundary", 1);
    //  REQUIRE_ALWAYS(is_oriented(G), "Grid must be oriented!", 1);

    typedef grid_types<GRID>          gt;
    typedef typename GEOM::coord_type coord_type;
    typedef algebraic_primitives<coord_type> ap;
    coord_type p0 = point_traits<coord_type>::Origin(GeomG.space_dimension());
    //typename real_type<coord_type>::type res = 0;
    double res = 0;
    for(typename gt::CellIterator c = begin_x(G); !c.IsDone(); ++c) {
      typename gt::VertexOnCellIterator vc = begin_x(*c);
      coord_type v0 = GeomG.coord(*vc);
      ++vc;
      coord_type vi = GeomG.coord(*vc);
      ++vc;
      coord_type vi1;
      while(! vc.IsDone()) {
	vi1 = GeomG.coord(*vc);
	// oriented volume of tet (p0, v0,vi,vi1)
	res += (1/6.0) * ap::det3(v0-p0,vi-p0,vi1-p0);

	vi = vi1;
	++vc;
      };
    }
    return res;
  }

  /*! \brief Find volume enclosed by surface mesh

    \ingroup geometricmeasures

    \pre G is a manifold mesh (without boundary)
    \pre dim(G) = spacedim(GeomG) - 1

    \post G is oriented
    \result The volume of the region enclosed by G.
  */
  template<class GRID, class GEOM>
  double 
  //  typename point_traits<typename GEOM::coord_type>::real_type
  //  typename real_type<GEOM>::type
  enclosed_volume(GRID & G, GEOM const& GeomG) 
  {
    // if(! is_oriented(G))
    fix_orientation(G);
    return enclosed_volume_oriented(G, GeomG);
  }



} // namespace GrAL

#endif
