#ifndef GRAL_BASE_GB_WRITE_STL_H
#define GRAL_BASE_GB_WRITE_STL_H


/* ------------------------------------------------------------

    Copyright (C) 2008 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include "Gral/Base/common-grid-basics.h"
#include "Geometry/algebraic-primitives.h"
#include "Geometry/point-traits.h"

#include <string>
#include <ostream>

namespace GrAL {

  /*! \brief Write grid to ASCII STL file

    STL is an inferior format for surface meshes used by a number of commercial tools 
    ("industry standard").
    It contains a simple list of triangles given by vertex coordinates and thus looses
    connectivity information. This information can only be recovered if no vertices have
    the same coordinates.

    All triangular cells of the grid \c G will be written to \c out.
    \return true if write was successful and no non-triangular cells occured. 
     \pre G is a 2D grid 
     \pre GeomG is a 3D geometry for G
     \post num_non_triangles contains the number of non-triangular cells.
  */
  template<class GRID, class GEOM>
  bool write_stl_ascii(std::ostream     & out,
		       GRID const& G, GEOM const& GeomG,
		       std::string const& geom_name,
		       int              & num_non_triangles)
  {
    typedef grid_types<GRID> gt;
    typedef typename GEOM::coord_type coord_type;
    typedef algebraic_primitives<coord_type> ap;
    typedef point_traits        <coord_type> pt;
    out << "solid <<" << geom_name << ">>\n";
    for(typename gt::CellIterator c(G); !c.IsDone(); ++c) {
      if((*c).NumOfVertices() == 3) {
	coord_type v[3];
	typename gt::VertexOnCellIterator vc(*c);
	v[0] = GeomG.coord(*vc);
	v[1] = GeomG.coord(*++vc);
	v[2] = GeomG.coord(*++vc);
	out << "  "   << "facet normal " 
	    << ap::normalization(ap::vectorproduct(v[1]-v[0],v[2]-v[0])) << "\n";
	out << "    " << "outer loop\n";
	for(int i = 0; i < 3; ++i)
	  out << "     " << "vertex " << pt::x(v[i]) << " " << pt::y(v[i]) << " " << pt::z(v[i]) << "\n";
	out << "    " << "endloop\n";
	out << "  " << "end facet\n";
    }
    else
      ++num_non_triangles;
    }
    out << "endsolid <<" << geom_name << ">>\n";
    return (num_non_triangles > 0 ? false : true);
  }

} // namespace GrAL


#endif

