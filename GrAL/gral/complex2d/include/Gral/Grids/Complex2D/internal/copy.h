#ifndef NMWR_GB_COMPLEX2D_INTERNAL_COPY_CELLS_H
#define NMWR_GB_COMPLEX2D_INTERNAL_COPY_CELLS_H


// $LICENSE

#include "Gral/Grids/Complex2D/complex2d.h"

//----------------------------------------------------------------
//  !! intended for use in other functions only, 
//  !! not directly by user! 
//  !! This leaves Complex2D G in an inconsistent state !!
//  
//  purpose: append the cells from G_src to G, translating the
//  vertices from G_src to G via vtx_corr : V_h(G_src) --> V_h(G).
//  * Preconditions:
//    -------------
//    The vertices in ran(vtx_corr) are already  created in G,
//    and vtx_corr is the correspondance map.
//  * Postconditions:
//    --------------
//    cells will be copied from G-src to G, 
//      with correct vertex references
//      with neighbour references set to <boundary>
//    cell_corr is the correspondance map C_h(G_src) --> C_h(G)
//----------------------------------------------------------------



#ifdef NMWR_INCLUDE_TEMPLATE_DEFS
#include "Gral/Grids/Complex2D/internal/copy.C"

#else

template<class Conn, class VtxCorr, class CellCorr>
void copy_cells(Complex2D& G, 
		const Conn& G_src,
		/* const */ VtxCorr&  vtx_corr,
		CellCorr&       cell_corr);


#endif

#endif
