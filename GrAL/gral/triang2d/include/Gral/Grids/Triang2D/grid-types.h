#ifndef GRAL_GB_GRIDS_TRIANG2D_GRID_TYPES_H
#define GRAL_GB_GRIDS_TRIANG2D_GRID_TYPES_H


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

#include "Gral/Base/element-handle.h"
#include "Gral/Base/polygon.h"
#include "Gral/Base/archetype-geometry.h"
#include "Gral/Iterators/closure-iterator.h"

namespace GrAL {

class Triang2D;
class Triang2D_Vertex;
class Triang2D_Edge;
class Triang2D_Cell;
class Triang2D_VertexOnCellIterator;
class Triang2D_FacetOnCellIterator;


class edge_handle_Triang2D;

struct grid_types_base_Triang2D : public grid_types_detail::grid_types_root {
  typedef grid_types_base_Triang2D self;
  typedef grid_dim_tag<2>               dimension_tag;
  
  typedef Triang2D        grid_type;
  
  typedef Triang2D_Vertex Vertex;
  typedef Triang2D_Edge   Edge;
  typedef Edge            Facet;
  typedef Triang2D_Cell   Cell;

  typedef Cell            CellIterator;
  typedef Vertex          VertexIterator;

  typedef Triang2D_VertexOnCellIterator VertexOnCellIterator;
  typedef Triang2D_FacetOnCellIterator  FacetOnCellIterator;
  typedef FacetOnCellIterator           EdgeOnCellIterator;

  typedef closure_iterator<CellIterator, EdgeOnCellIterator, self> EdgeIterator;
  typedef EdgeIterator    FacetIterator;

  typedef vertex_handle_int<grid_type,self>  vertex_handle;
  typedef cell_handle_int  <grid_type,self>  cell_handle;
  typedef edge_handle_Triang2D               edge_handle;
  typedef edge_handle                        facet_handle;

  typedef Cell                 Face;
  typedef cell_handle          face_handle;
  typedef CellIterator         FaceIterator;
  typedef VertexOnCellIterator VertexOnFaceIterator;
  typedef EdgeOnCellIterator   EdgeOnFaceIterator;


  typedef polygon1d::polygon                 archetype_type;
  typedef archetype_type const*              archetype_iterator;
  typedef size_type                          archetype_handle;
  typedef grid_types<archetype_type>         archgt;
  typedef archetype_geometry<grid_type, archetype_type> archetype_geom_type;
  typedef archetype_geom_type::coord_type    local_coord_type;
};

} // namespace GrAL 

#endif

