#ifndef NMWR_GB_COMPLEX2D_INTERNAL_COPY_CELLS_C
#define NMWR_GB_COMPLEX2D_INTERNAL_COPY_CELLS_C


// $LICENSE

#include "Gral/Grids/Complex2D/internal/copy.h"

template<class Conn, class VtxCorr, class CellCorr>
void copy_cells(Complex2D& G, 
		const Conn& G_src,
		/* const */ VtxCorr&  vtx_corr,
		CellCorr&       cell_corr)
{
  typedef grid_types<Conn>                      src_gt;
  typedef typename src_gt::CellIterator         src_cell_it;
  typedef typename src_gt::VertexOnCellIterator src_vertex_on_cell_it;

  typedef grid_types<Complex2D>     gt;
  typedef typename  gt::Cell        Cell;
  typedef typename  gt::cell_handle cell_handle;
  //typedef typename gt::BoundaryFacetIterator BdFacetIterator;    

  friend_for_input cc(G);

  cell_handle boundary(cc.outer_cell_handle()); // invalid reference to a cell
                                                // (used to mark unknown neighbours)
  for(src_cell_it src_c = G_src.FirstCell(); ! src_c.IsDone(); ++src_c){
    cell_handle pc = cc._new_cell((*src_c).NumOfVertices());
    cell_corr[src_c.handle()] = pc;

    typedef cell2d_connectivity c2d;
    typedef c2d::cell_list   cell_list;
    typedef c2d::vertex_list vertex_list;
    typename cell_list::iterator   ln(cc._cell_neighbours(pc).begin()); 
    typename vertex_list::iterator lv(cc._cell_vertices(pc).begin());
    src_vertex_on_cell_it src_lv((*src_c).FirstVertex());
    for(; ! src_lv.IsDone(); ++src_lv) {
      (int&)(*lv) = vtx_corr(src_lv.handle());
      (int&)(*ln) = boundary; // correct neighbour is calculated later
      ++lv;
      ++ln;
    }

  }

}


#endif
