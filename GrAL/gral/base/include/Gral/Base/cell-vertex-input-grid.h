#ifndef GRAL_GB_BASE_CELL_VERTEX_INPUT_GRID_H
#define GRAL_GB_BASE_CELL_VERTEX_INPUT_GRID_H


/* ------------------------------------------------------------

    Copyright (C) 2001 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include "Gral/Base/common-grid-basics.h"

namespace GrAL {

template<class GRID>
struct grid_types_cell_vertex_input_grid 
{
  typedef GRID grid_type;

  typedef typename GRID::Vertex Vertex;
  typedef typename GRID::Cell   Cell;
  typedef typename GRID::VertexIterator VertexIterator;
  typedef typename GRID::CellIterator   CellIterator;
  typedef typename GRID::vertex_handle  vertex_handle;
  typedef typename GRID::cell_handle    cell_handle;
  typedef typename GRID::VertexOnCellIterator VertexOnCellIterator;

};

} // namespace GrAL 

#endif
