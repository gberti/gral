#ifndef GRAL_GB_GRIDS_TRIANG2D_CONSTRUCT_C
#define GRAL_GB_GRIDS_TRIANG2D_CONSTRUCT_C

// $LICENSE
 
/*! \file
 */

#include "Gral/Grids/Triang2D/copy.h"

#include "Container/partial-mapping.h"
#include "Container/dummy-mapping.h"

namespace GrAL {

//----------------------------------------------------------------
//
//  Copy-construct a Triang2D grid from a right-hand side (Connect)
//  that allows
//  - iteration over vertices ( VertexIterator | FirstVertex() )
//  - iteration over cells    ( CellIterator   | FirstCell() )
//  - iteration over cell-vertices (VertexOnCellIterator | Cell::FirstVertex() )
//  and a geometry (geom) that gives coordinates for each vertex.
//  The additional connectivity information is then calculated
//  (that is, neighbors of cells and adjacent cells of vertices)
//
//  This algorithm is essentially the same for reading a file from
//  disk (storing vertex coordinates and indices of vertices for each cell)
//  or, more general, for constructing the grid from a serialized representation
//  (e.g. in message passing).
//  Perhaps the best way to exploit this would be to write a grid mask
//  for these serial versions, using one single template method for
//  the copy-construction. This could then also be used for Enlarge(...)
//  and future similar functions.
//----------------------------------------------------------------


template<class Geom, class G2, class Geom2>
void ConstructGrid(Triang2D        & G, 
		   Geom            & destGeom,
		   G2         const& srcG,
		   Geom2      const& srcGeom )
{ 
  typedef grid_types<G2>                                source_gt;
  typedef typename source_gt::vertex_handle             src_vertex_handle;
  typedef typename grid_types<Triang2D>::vertex_handle  vertex_handle;

  // this should be replaced by an array if possible.
  // map also lacks an operator() const  |  operator[] const 
  partial_mapping<src_vertex_handle,vertex_handle> VertexCorr;
  ConstructGridV(G,destGeom,srcG,srcGeom,VertexCorr);
}

template<class G2> 
void ConstructGrid0(Triang2D        & G, 
		    G2         const& srcG)
{ 
  typedef grid_types<G2>                                source_gt;
  typedef typename source_gt::vertex_handle             src_vertex_handle;
  typedef typename source_gt::cell_handle               src_cell_handle;
  typedef typename grid_types<Triang2D>::vertex_handle  vertex_handle;
  typedef typename grid_types<Triang2D>::cell_handle    cell_handle;

  // this should be replaced by an array if possible.
  // map also lacks an operator() const  |  operator[] const 
  partial_mapping<src_vertex_handle,vertex_handle> VertexCorr;
  // here we are not interested in a cell correspondance
  dummy_mapping<src_cell_handle,cell_handle> CellCorr;
  ConstructGrid0(G,srcG,VertexCorr,CellCorr);
}


template<class Geom, class G2,  class Geom2, class VertexMap>
void ConstructGridV(Triang2D       & G, 
		    Geom           & destGeom,
		    G2        const& srcG,
		    Geom2     const& srcGeom,
		    // maps src::handles to grid2d_fvg::handles
		    VertexMap      &  VertexCorr) 
{

  typedef grid_types<G2>                               source_gt;
  typedef typename source_gt::cell_handle              src_cell_handle;
  typedef typename grid_types<Triang2D>::cell_handle   cell_handle;

  // here we are not interested in a cell correspondance
  dummy_mapping<src_cell_handle,cell_handle> CellCorr;

  ConstructGridVC(G,destGeom, srcG,srcGeom,VertexCorr,CellCorr);
}


template<class Geom, class G2, class Geom2, class VertexMap, class CellMap>
void ConstructGridVC(Triang2D       & G,
		     Geom           & destGeom,
		     G2        const&  srcG,
		     Geom2     const&  srcGeom,
		     VertexMap      &  VertexCorr, // maps src::handles to
		     CellMap        &  CellCorr)   // complex2d::handles
{
  // construct combinatorial grid
 ConstructGrid0(G,srcG,VertexCorr,CellCorr);


 // copy geometry
 destGeom.set_grid(G);
 typedef typename grid_types<G2>::VertexIterator   src_vertex_it;
 typedef typename grid_types<Triang2D>::Vertex     Vertex;
 for(src_vertex_it src_v = srcG.FirstVertex(); ! src_v.IsDone(); ++src_v) {
   Vertex V(G,VertexCorr(src_v.handle()));
   assign_point(destGeom.coord(V) , srcGeom.coord(*src_v));
 }
}

template<class G2, class VertexMap, class CellMap>
void ConstructGrid0(Triang2D       &  G, 
		    G2        const&  srcG,
		    VertexMap      &  VertexCorr, // maps src::handles to
		    CellMap        &  CellCorr)   // complex2d::handles

{
  typedef grid_types<G2> source_gt;

  //  typedef typename source_gt::vertex_handle   src_v_index;
  typedef typename source_gt::cell_handle          src_cell_handle;
  typedef typename source_gt::CellIterator         src_cell_it;
  typedef typename source_gt::VertexIterator       src_vertex_it;
  typedef typename source_gt::VertexOnCellIterator src_vertex_on_cell_it;

  typedef grid_types<Triang2D>       gt;
  typedef typename gt::Cell          Cell;
  typedef typename gt::cell_handle   cell_handle;
  typedef typename gt::vertex_handle vertex_handle;

  //---  construct vertices ---------------------------
  
  int v = 0;
  for(src_vertex_it src_v = srcG.FirstVertex(); ! src_v.IsDone(); ++src_v, ++v) {
    VertexCorr[src_v.handle()] = v;
  }
  

  //---- construct cells ------------------------------

  copy_cells(G,srcG,VertexCorr,CellCorr);
 

}

} // namespace GrAL 

#endif
