#ifndef GRAL_GB_CARTESIAN2D_TEST_CARTESIAN_GEOMETRY_2D_H
#define GRAL_GB_CARTESIAN2D_TEST_CARTESIAN_GEOMETRY_2D_H

// $LICENSE

/*! \file
 */

#include "Gral/Base/common-grid-basics.h"

/*! \brief Test a Cartesian geometry
    
    \ingroup cartesian2dgeom

    This test function assumes a 2-dimensional geometry, because facet normals
    are tested, which are defined only for 2D embeddings.
*/
template<class GEOM>
void test_cartesian2d_geometry(GEOM & Geo, std::ostream& out)
{
  typedef grid_types<typename GEOM::grid_type> gt;

  for(typename gt::VertexIterator v(Geo.TheGrid()); !v.IsDone(); ++v)
    out << "v=" << (*v).index() << " @ " << Geo.coord(*v) << "\n";
  for(typename gt::EdgeIterator e(Geo.TheGrid()); ! e.IsDone(); ++e)
    out << "e=[" << (*e).V1().index() << " -> " << (*e).V2().index() << "] "
	<< " length(e)=" << Geo.length(*e) 
	<< " center(e)=" << Geo.center(*e)
	<< " normal(e)=" << Geo.normal(*e)
	<< "\n";
  for(typename gt::CellIterator c(Geo.TheGrid()); !c.IsDone(); ++c) {
    out << "c=" << (*c).index() 
	<< " volume(c)=" << Geo.volume(*c) 
	<< " center(c)=" << Geo.center(*c) 
	<< "\n";
    for(typename gt::FacetOnCellIterator fc(*c); !fc.IsDone(); ++fc)
      out << "fc=[" << fc.V1().index() << " -> " << fc.V2().index() << "] "
	  << " outer_area_normal(fc)=" << Geo.outer_area_normal(fc) 
	  << " outer_normal(fc)="      << Geo.outer_normal(fc) 
	  << "\n";
  }

}


#endif
