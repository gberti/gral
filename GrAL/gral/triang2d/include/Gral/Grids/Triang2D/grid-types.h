#ifndef GRAL_GB_GRIDS_TRIANG2D_GRID_TYPES_H
#define GRAL_GB_GRIDS_TRIANG2D_GRID_TYPES_H

// $LICENSE 

#include "Gral/Base/element-handle.h"
#include "Gral/Base/polygon.h"

class Triang2D;
class Triang2D_Vertex;
class Triang2D_Edge;
class Triang2D_Cell;
class Triang2D_VertexOnCellIterator;
class Triang2D_FacetOnCellIterator;
class Triang2D_FacetIterator;

class edge_handle_Triang2D;

struct grid_types_base_Triang2D {

  typedef Triang2D        grid_type;
  
  typedef Triang2D_Vertex Vertex;
  typedef Triang2D_Edge   Edge;
  typedef Edge            Facet;
  typedef Triang2D_Cell   Cell;

  typedef Cell            CellIterator;
  typedef Vertex          VertexIterator;
  typedef Triang2D_FacetIterator EdgeIterator;
  typedef EdgeIterator    FacetIterator;

  typedef Triang2D_VertexOnCellIterator VertexOnCellIterator;
  typedef Triang2D_FacetOnCellIterator  FacetOnCellIterator;
  typedef FacetOnCellIterator           EdgeOnCellIterator;


  typedef vertex_handle_int<grid_type>  vertex_handle;
  typedef cell_handle_int  <grid_type>  cell_handle;
  typedef edge_handle_Triang2D          edge_handle;
  typedef edge_handle                   facet_handle;

  typedef Cell                 Face;
  typedef cell_handle          face_handle;
  typedef CellIterator         FaceIterator;
  typedef VertexOnCellIterator VertexOnFaceIterator;
  typedef EdgeOnCellIterator   EdgeOnFaceIterator;

  typedef grid_dim_tag<2>               dimension_tag;

  typedef polygon1d::polygon                 archetype_type;
  typedef std::vector<archetype_type>        archetype_sequence;
  typedef archetype_sequence::const_iterator archetype_iterator;
  typedef int                                archetype_handle;
  typedef grid_types<archetype_type>         archgt;
};

#endif

