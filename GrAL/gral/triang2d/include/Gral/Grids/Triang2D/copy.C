#ifndef GRAL_GB_GRIDS_TRIANG2D_COPY_CELLS_C
#define GRAL_GB_GRIDS_TRIANG2D_COPY_CELLS_C


/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti, 
    Contact: guntram AT users DOT berlios DOT de

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


/*! \file
 */

#include "Gral/Grids/Triang2D/copy.h"

namespace GrAL {

template<class Conn, class VtxCorr, class CellCorr>
void copy_cells(Triang2D       & G, 
		Conn      const&  G_src,
		VtxCorr        &  vtx_corr,
		CellCorr       &  cell_corr)
{
  typedef grid_types<Conn>                      src_gt;
  typedef typename src_gt::CellIterator         src_cell_it;
  typedef typename src_gt::VertexOnCellIterator src_vertex_on_cell_it;

  typedef grid_types<Triang2D>      gt;
  typedef typename  gt::Cell        Cell;
  typedef typename  gt::cell_handle cell_handle;

  // first unused cell handle, = 0 if new grid
  cell_handle hc = G.NumOfCells();

  gt::size_type * cells = new gt::size_type[3*(G_src.NumOfCells()+hc)];

  //  copy(G.cells_(),G.cells_() + 3*hc, cells + 3*hc);

  //  for(src_cell_it src_c = begin_x(G_src); ! src_c.IsDone(); ++src_c, ++hc){
  for(src_cell_it src_c = G_src.FirstCell(); ! src_c.IsDone(); ++src_c, ++hc){
    REQUIRE((size_d<0>(*src_c) == 3),
	    "Can handle only triangles, but  NumOfVertices() was "
	    << size_d<0>(*src_c) << "!\n",1);

    cell_corr[src_c.handle()] = hc;
    
    int lv = 0;
    src_vertex_on_cell_it src_lv = begin_x(*src_c);
    for(; ! src_lv.IsDone(); ++src_lv, ++lv) {
      cells[3*hc+lv] = vtx_corr(src_lv.handle());
    }
   
  }
  G.Steal(cells, G_src.NumOfCells(), G_src.NumOfVertices());
}

} // namespace GrAL 

#endif
