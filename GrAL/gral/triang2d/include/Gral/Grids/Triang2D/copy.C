#ifndef NMWR_GB_GRIDS_TRIANG2D_COPY_CELLS_C
#define NMWR_GB_GRIDS_TRIANG2D_COPY_CELLS_C

//----------------------------------------------------------------
//   (c) Guntram Berti, 1999
//   Chair for Numerical Mathematics & Scientific Computing (NMWR)
//   TU Cottbus - Germany
//   http://math-s.math.tu-cottbus.de/NMWR
//   
//----------------------------------------------------------------

#include "Grids/Triang2D/generic/copy.h"

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

  int * cells = new int[3*(G_src.NumOfCells()+hc)];

  //  copy(G.cells_(),G.cells_() + 3*hc, cells + 3*hc);

  for(src_cell_it src_c = G_src.FirstCell(); ! src_c.IsDone(); ++src_c, ++hc){
    REQUIRE(((*src_c).NumOfVertices() == 3),
	    "Can handle only triangles, or  NumOfVertices() was "
	    << (*src_c).NumOfVertices() << "!\n",1);

    cell_corr[src_c.handle()] = hc;
    
    int lv = 0;
    src_vertex_on_cell_it src_lv((*src_c).FirstVertex());
    for(; ! src_lv.IsDone(); ++src_lv, ++lv) {
       cells[3*hc+lv] = vtx_corr(src_lv.handle());
    }
   
  }
  G.Steal(cells, G_src.NumOfCells(), G_src.NumOfVertices());
}


#endif
