#ifndef GRAL_BASE_GB_CONSTRUCT_GRID_H
#define GRAL_BASE_GB_CONSTRUCT_GRID_H

//----------------------------------------------------------------
//   (c) Guntram Berti, 1998
//   Chair for Numerical Mathematics & Scientific Computing (NMWR)
//   TU Cottbus - Germany
//   http://math-s.math.tu-cottbus.de/NMWR
//   
//----------------------------------------------------------------


//----------------------------------------------------------------
// copy-construct a grid destG from another 2D grid srcG
//
// CONSTRAINTS on template parameters:
// -----------
// * G1: category grid. These template functions are specialized 
//    for concrete types of G1.
// * G2: category grid, must support
//   - grid_types<>, 
//   - CellIterator, VertexIterator, VertexOnCellIterator,
//   - vertex_handle
// * Geom2 : category grid geometry
//   - coord(Vertex2)
// * VertexMap: category map V(G2) -> V(G1)
//   - vertex_handle1 operator()(vertex_handle2) (read)
//   - vertex_handle1 operator[](vertex_handle2) (write)
// * CellMap: category map C(G2) -> C(G1)
//   - cell_handle1 operator()(cell_handle2) (read)
//   - cell_handle1 operator[](cell_handle2) (write)
//
//----------------------------------------------------------------
/*
template<class G1, class G2, class Geom2>
extern void 
ConstructGrid(G1         & destG,
	      G2    const& srcG, 
	      Geom2 const& srcGeom);

template<class G1, class G2, class Geom2, class VertexMap>
extern void 
ConstructGridV(G1            & destG, 
	       G2       const& srcG,
	       Geom2    const& srcGeom,
	       VertexMap     & VCorrG2_G1); 
// VertexCorr  maps G2::vertex_handles to  G1::vertex_handles
// for transfer of grid-related information, e.g. grid-functions.

template<class G1, class G2, class Geom2, class VertexMap, class CellMap>
extern void 
ConstructGridVC(G1            & destG, 
		G2       const& srcG,
		Geom2    const& srcGeom,
		VertexMap     & VCorrG2_G1,
		CellMap       & CCorrG2_G1); 
// CellCorr  maps G2::cell_handles to  G1::vertex_handles
// for transfer of grid-related information, e.g. grid-functions.


// construct without geometry
template<class G1, class G2, class VertexMap, class CellMap>
extern void 
ConstructGrid0(G1            & destG, 
	       G2       const& srcG,
	       VertexMap     & VCorrG2_G1,
	       CellMap       & CCorrG2_G1); 

*/
#include "Container/dummy-mapping.h"
#include "Container/partial-mapping.h"

template<class G1, class G2>
inline void 
ConstructGrid0(G1            & destG, 
	       G2       const& srcG)
{
  typedef grid_types<G2>                         gt2;
  typedef typename gt2::cell_handle              cell_handle_2;
  typedef typename gt2::vertex_handle            vertex_handle_2;
  typedef typename grid_types<G1>::cell_handle   cell_handle_1;
  typedef typename grid_types<G1>::vertex_handle vertex_handle_1;

  // here we are not interested in a cell correspondance
  dummy_mapping<cell_handle_2,cell_handle_1>       CellCorr;
  // VertexCorr must be a functional type, because it is used.
  partial_mapping<vertex_handle_2,vertex_handle_1> VertexCorr;
  ConstructGrid0(destG,srcG,VertexCorr,CellCorr);
}


#endif
