#ifndef GRAL_GB_GRIDS_TRIANG3D_GRID_TYPES_H
#define GRAL_GB_GRIDS_TRIANG3D_GRID_TYPES_H

// $LICENSE_NEC_2003

#include "Gral/Base/element-handle.h"
#include "Gral/Grids/Complex2D/complex2d.h"

/*! \file
 */

namespace GrAL {

class Triang3D;
class Triang3D_Vertex;
//class Triang3D_Edge;
class Triang3D_Cell;
class Triang3D_Cell_base;
class Triang3D_VertexOnCellIterator;
//class Triang3D_FacetOnCellIterator;
//class Triang3D_FacetIterator;

//class edge_handle_Triang3D;

struct grid_types_base_Triang3D {

  typedef Triang3D        grid_type;
  
  typedef Triang3D_Vertex Vertex;
  // typedef Triang3D_Edge   Edge;
  // typedef Edge            Facet;
  typedef Triang3D_Cell   Cell;

  typedef Triang3D_Cell_base cell_base_type;

  typedef Cell            CellIterator;
  typedef Vertex          VertexIterator;
  //typedef Triang3D_EdgeIterator EdgeIterator;
  //typedef EdgeIterator    FacetIterator;

  typedef Triang3D_VertexOnCellIterator VertexOnCellIterator;
  //typedef Triang3D_FacetOnCellIterator  FacetOnCellIterator;
  //typedef Triang3D_EdgeOnCellIterator   EdgeOnCellIterator;



  typedef vertex_handle_int<grid_type>  vertex_handle;
  typedef cell_handle_int  <grid_type>  cell_handle;
  //typedef edge_handle_Triang3D          edge_handle;
  // typedef edge_handle                   facet_handle;

  /*
  typedef Cell                 Face;
  typedef cell_handle          face_handle;
  typedef CellIterator         FaceIterator;
  typedef VertexOnCellIterator VertexOnFaceIterator;
  typedef EdgeOnCellIterator   EdgeOnFaceIterator;
  */

  typedef grid_dim_tag<3>       dimension_tag;

  typedef Complex2D             archetype_type;
  typedef archetype_type const* archetype_iterator;
  typedef int                   archetype_handle;
  typedef grid_types<archetype_type>  archgt;  
};


} // namespace GrAL 

#endif

