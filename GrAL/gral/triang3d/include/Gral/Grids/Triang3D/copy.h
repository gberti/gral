#ifndef GRAL_GB_GRIDS_TRIANG3D_COPY_CELLS_H
#define GRAL_GB_GRIDS_TRIANG3D_COPY_CELLS_H


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

#include "Gral/Grids/Triang3D/triang3d.h"

namespace GrAL {

//----------------------------------------------------------------
//  !! DIRTY operation.
//  !! intended for use in other functions only, 
//  !! not directly by user! 
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



template<class Conn, class VtxCorr, class CellCorr>
void copy_cells(Triang3D       & G, 
		Conn      const& G_src,
		VtxCorr   const& vtx_corr,
		CellCorr       & cell_corr);


} // namespace GrAL 

#ifdef NMWR_INCLUDE_TEMPLATE_DEFS
#include "Gral/Grids/Triang3D/copy.C"
#endif

#endif
