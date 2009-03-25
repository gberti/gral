#ifndef GRAL_GB_BASE_CALCULATE_VERTEX_STARS_C
#define GRAL_GB_BASE_CALCULATE_VERTEX_STARS_C


/* ------------------------------------------------------------

    Copyright (C) 2001 - 2009 Guntram Berti, 
    Contact: guntram AT users DOT berlios DOT de

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


/*! \file
 */

#include "Gral/Base/common-grid-basics.h"
#include "Gral/Algorithms/calculate-vertex-stars.h"

namespace GrAL {

// GERMS e.g. grid_function<Vertex, flag>
template<class GRID, class GERMS>
void calculate_vertex_stars(GRID const& G, // 2D, e.g. archetype of 3D cell
			    GERMS &     germs) // e.g gf: Vertex -> flag<GRID>
{
  typedef grid_types<GRID> gt;
  typedef typename GERMS::value_type flag_type;
  for(typename gt::CellIterator c(G); ! c.IsDone(); ++c)
    for(typename gt::FacetOnCellIterator fc(*c); !fc.IsDone(); ++fc)
      for(typename gt::VertexOnFacetIterator vfc(*fc); !vfc.IsDone(); ++vfc)
	germs[*vfc] = flag_type(*vfc,*fc,*c);
}

template<class GRID, class GERMS, class DET>
void calculate_positive_vertex_stars(GRID const& G, // 2D, e.g. archetype of 3D cell
				     GERMS     & germs,
				     DET  const& det) // corner jac. of archetype
{
  calculate_vertex_stars(G,germs);

  // make germs (= stars) positive oriented 
  typedef grid_types<GRID>           gt;
  typedef typename GERMS::value_type flag_type;
  for(typename gt::VertexIterator v(G); ! v.IsDone(); ++v) {
    typedef typename gt::Vertex Vertex;
    typedef typename GERMS::value_type flag_type;
    flag_type flag = germs[*v];
    Vertex v1 = flag.switched_vertex();
    flag.switch_edge();
    Vertex v2 = flag.switched_vertex();
    flag.switch_face();
    flag.switch_edge();
    Vertex v3 = flag.switched_vertex();
    // assume only 3 edges starting from v

    if(det(*v,v1,v2,v3) < 0) // wrong orientation?
      germs[*v].switch_edge();   // now above iteration changes direction
  }
  
}

} // namespace GrAL 

#endif
