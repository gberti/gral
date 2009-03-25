#ifndef GRAL_GB_MEASUREMENT_COMPARE_GRIDS_H
#define GRAL_GB_MEASUREMENT_COMPARE_GRIDS_H


/* ------------------------------------------------------------

    Copyright (C) 2007 - 2009 Guntram Berti, 
    Contact: guntram AT users DOT berlios DOT de

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include "Gral/Base/common-grid-basics.h"
#include "Gral/Base/grid-morphism.h"

#include "Geometry/point-traits.h"
#include "Geometry/algebraic-primitives.h"

namespace GrAL {
  
  /*! \defgroup grid_comparison Comparison of grids and geometries

    
  */

  /*! \brief Result values for compare_grids

     \ingroup grid_comparison
  */
  struct grid_comparison_result {
    enum type { trivially_isomorphic = 0,
		unequal_number_of_elements = 1,
		not_trivially_isomorphic = 2,
		isomorphic = 3 };
  };

  /*! \brief Compare grids for equality

     \ingroup grid_comparison

    Equal means trivially isomorphic, that is, identifying
    vertices and cells by their position in the sequences
    of the corresponding iterators establishes a grid isomorphism.

   \todo The code does not check whether the archetypes of corresponding cells
    are equal. So, a single prism and a single octahedron would erronously be
    classified as trivially isomorphic.
  */
  template<class G1, class G2>
  grid_comparison_result::type
  compare_grids(G1 const& g1, G2 const& g2) 
  {
    typedef grid_types<G1> gt1;
    typedef grid_types<G2> gt2;
    if(g1.NumOfVertices() != g2.NumOfVertices()
       || g1.NumOfCells() != g1.NumOfCells())
      return grid_comparison_result::unequal_number_of_elements;
    vertex_morphism<G1,G2> vcorr(g1,g2);
    typename gt1::VertexIterator v1(g1);
    typename gt2::VertexIterator v2(g2);
    for(; !v1.IsDone(); ++v1,++v2)
      vcorr[*v1] = *v2;
    typename gt1::CellIterator c1(g1);
    typename gt2::CellIterator c2(g2);
    for(; !c1.IsDone(); ++c1, ++c2) {
      // check correspondance of vertex sequences.
      // This does not check whether a re-ordering of vertex sequences could yield an isomorphism
      // (e.g. a cyclic rotation of the vertex sequence in the polygonal case).
      if( (*c1).NumOfVertices() != (*c2).NumOfVertices())
	return grid_comparison_result::not_trivially_isomorphic;
      typename gt1::VertexOnCellIterator vc1(*c1);
      typename gt2::VertexOnCellIterator vc2(*c2);
      while(! vc1.IsDone()) {
	if(vcorr(*vc1) != *vc2)
	  return grid_comparison_result::not_trivially_isomorphic;
	++vc1; ++vc2;
      }
    }
    return grid_comparison_result::trivially_isomorphic;
  }


  /*! \brief Compute difference of grid geometries

  \ingroup grid_comparison

   \pre The underlying grids are trivially isomorphic
   \return Maximal distance of two corresponding vertices
  */
  template<class GEOM1, class GEOM2>
  typename GEOM1::coord_type
  max_vertex_distance(GEOM1 const& geom1, GEOM2 const& geom2)
  {
    typedef grid_types<typename GEOM1::grid_type> gt1;
    typedef grid_types<typename GEOM2::grid_type> gt2;
    typedef typename GEOM1::coord_type coord_type1;
    typedef typename GEOM2::coord_type coord_type2;
    typedef algebraic_primitives<coord_type1> ap;
    
    typename point_traits<coord_type1>::component_type max_dist(0);
    coord_type1 diff(max_dist);

    typename gt1::VertexIterator v1(geom1.TheGrid());
    typename gt2::VertexIterator v2(geom2.TheGrid());
    for(; !v1.IsDone(); ++v1, ++v2) {
      coord_type1 x1 = geom1.coord(*v1);
      coord_type1 x2 = convert_point<coord_type1>(geom2.coord(*v2));
      if(ap::distance(x1,x2) > max_dist) {
	max_dist = ap::distance(x1,x2);
	diff = x1 - x2;
      }
    }
    return diff;
  }

} // namespace GrAL

#endif
