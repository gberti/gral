#ifndef GRAL_GB_GEOMVIEW_FORMAT_OUTPUT_TT_C
#define GRAL_GB_GEOMVIEW_FORMAT_OUTPUT_TT_C


/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


/*! \file
 */

#include "Gral/IO/geomview-format-output.h"
#include "Geometry/point-traits.h"

namespace GrAL {

template<class GRID, class GEOM>
void ConstructGrid(OstreamOFF2DFmt& Out, 
                   GRID const& G, GEOM const& Geom)
{
  typedef grid_types<GRID>                  gt;
  typedef typename gt::Vertex               Vertex;
  typedef typename gt::Cell                 Cell;
  typedef typename gt::VertexIterator       VertexIterator;
  typedef typename gt::CellIterator         CellIterator;
  typedef typename gt::VertexOnCellIterator VertexOnCellIterator;

  std::ostream& out = Out.Out();
  out << "OFF\n";
  out << G.NumOfVertices() << " " 
      << G.NumOfCells()    << " "
      << "-1" << "\n\n"; // number of edges: not checked in OFF format.

  // if GRID is a subrange type, we cannot initialize grid-functions
  // directly with G, but must get hold of the underlying grid.
  // This solution is not optimal. There could be a c-t branch
  // with an additional grid-type parameter for grid-functions.
  typedef element_traits<Vertex> et;
  typedef typename et::grid_type base_grid_type; // possibly != GRID
  Vertex dummy(*(G.FirstVertex())); // FIXME: fails for empty grid
  base_grid_type const& baseG(dummy.TheGrid());

  grid_function<Vertex,int> VNum(baseG);
  int vnum = Out.Offset();
  for(VertexIterator v= G.FirstVertex(); ! v.IsDone(); ++v){
    //    out << Geo.coord(*v) << "\n"; 
    VNum[*v] = vnum;
    vnum++;
  }
  out << "\n";

  typedef typename GEOM::coord_type coord_type;
  typedef point_traits<coord_type>  pt;
  for(VertexIterator v= G.FirstVertex(); ! v.IsDone(); ++v){
    out << pt::x(Geom.coord(*v)) << ' '
        << pt::y(Geom.coord(*v)) << ' '
        << pt::z(Geom.coord(*v)) << ' '
        << '\n';

  }

  for(CellIterator c = G.FirstCell(); !c.IsDone(); ++c){
    out << (*c).NumOfVertices() << "   ";
    for(VertexOnCellIterator vc(*c); ! vc.IsDone(); ++vc)
      out << VNum[*vc] << " ";
    out << "\n";
  }


}

} // namespace GrAL 

#endif
