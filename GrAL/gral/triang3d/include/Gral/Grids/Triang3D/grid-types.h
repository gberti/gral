#ifndef GRAL_GB_GRIDS_TRIANG3D_GRID_TYPES_H
#define GRAL_GB_GRIDS_TRIANG3D_GRID_TYPES_H


/* ------------------------------------------------------------

    Copyright (C) 2001 - 2009 Guntram Berti, 
    Contact: guntram AT users DOT berlios DOT de

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */
_2003

#include "Gral/Base/element-handle.h"
#include "Gral/Grids/Complex2D/complex2d.h"

/*! \file
 */

namespace GrAL {

class Triang3D;
class Triang3D_Vertex;
class Triang3D_Cell;
class Triang3D_Cell_base;
class Triang3D_VertexOnCellIterator;

class  Triang3D_grid_base;

struct grid_types_base_Triang3D : public grid_types_detail::grid_types_root {
  typedef grid_types_base_Triang3D self;

  typedef Triang3D        grid_type;
  typedef grid_dim_tag<3>       dimension_tag;
  
  typedef Triang3D_Vertex Vertex;
  typedef Triang3D_Cell   Cell;

  typedef Triang3D_Cell_base cell_base_type;
  typedef Triang3D_grid_base grid_base_type;

  typedef Cell            CellIterator;
  typedef Vertex          VertexIterator;

  typedef Triang3D_VertexOnCellIterator VertexOnCellIterator;

  typedef vertex_handle_int<grid_type,self>  vertex_handle;
  typedef cell_handle_int  <grid_type,self>  cell_handle;


  typedef Complex2D             archetype_type;
  typedef archetype_type const* archetype_iterator;
  typedef int                   archetype_handle;
  typedef grid_types<archetype_type>  archgt;  
};


} // namespace GrAL 

#endif

