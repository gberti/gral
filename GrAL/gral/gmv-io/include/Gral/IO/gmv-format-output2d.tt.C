#ifndef GRAL_GB_GMV_FMT_OUTPUT2D_TT
#define GRAL_GB_GMV_FMT_OUTPUT2D_TT

// $LICENSE_NEC

/*! \file
 */

#include "Gral/IO/gmv-format-output2d.h"
#include "Gral/Base/element-numbering.h"
#include "Geometry/point-traits.h"

namespace GrAL {

template<class GRID,class GEOM, class GF, class MOREGFS>
void ConstructGrid_GF(OstreamGMV2DFmt& Out, 
		      GRID const& G,
		      GEOM const& GEO,
		      heterogeneous_list::List<GF,MOREGFS> GFS);

template<class GRID,class GEOM>
void ConstructGrid(OstreamGMV2DFmt& Out, 
		   GRID const& G,
		   GEOM const& GEO)
{
  typedef heterogeneous_list::END END;
  heterogeneous_list::List<END,END> L;
  ConstructGrid_GF(Out,G,GEO,L);
}

template<class GRID,class GEOM, class GF, class MOREGFS>
void ConstructGrid_GF(OstreamGMV2DFmt& Out, 
		      GRID const& G,
		      GEOM const& GEO,
		      heterogeneous_list::List<GF,MOREGFS> GFS)
{
  typedef OstreamGMV2DFmt GMV3D;
  typedef grid_types<GRID> gt;
  typedef point_traits<typename GEOM::coord_type> pt;

  std::ostream& out = Out.Out();
  out << "gmvinput ascii\n";
  out << "nodev " << G.NumOfVertices() << '\n';

  for(typename gt::VertexIterator v = G.FirstVertex(); ! v.IsDone(); ++v)
    out << pt::x(GEO.coord(*v)) << ' '
	<< pt::y(GEO.coord(*v)) << ' '
	<< pt::z(GEO.coord(*v)) << ' '
	<< '\n';
  out << std::flush;

  // map G's vertices to GMV numbers, starting from 1.
  //  element_numbering<typename gt::Vertex> G2GMV(G,1);
  // FIXME! use element_numbering as above
  // (does not currently work for facet_grid, because
  // only typedefs instead of own element types)
  grid_function<typename gt::Vertex, int> G2GMV(G.FirstVertex().TheGrid(),-1);
  int cnt = 1;
  for(typename gt::VertexIterator v = G.FirstVertex(); !v.IsDone(); ++v)
    G2GMV[*v] = cnt++;

  // cells are 3D for GMV
  out << "cells 0\n";

  // map cells = surface polygons for GMV
  out << "surface " << G.NumOfCells() << "\n";
  for(typename gt::CellIterator c = G.FirstCell(); ! c.IsDone(); ++c) {
    out << (*c).NumOfVertices() << " ";
    for(typename gt::VertexOnCellIterator vc(*c); !vc.IsDone(); ++vc)
      out << G2GMV(*vc) << " ";
    out << '\n';
  }
  out << std::flush;

  Out.copy_grid_functions(GFS);
  out << "endgmv\n" << std::flush;
}

} // namespace GrAL 

#endif
