#ifndef NMWR_GB_GRIDS_TRIANG2D_GRID_TYPES_H
#define NMWR_GB_GRIDS_TRIANG2D_GRID_TYPES_H

//----------------------------------------------------------------
//   (c) Guntram Berti, 1999
//   Chair for Numerical Mathematics & Scientific Computing (NMWR)
//   TU Cottbus - Germany
//   http://math-s.math.tu-cottbus.de/NMWR
//   
//----------------------------------------------------------------

class Triang2D;
class Triang2D_Vertex;
class Triang2D_Edge;
class Triang2D_Cell;
class Triang2D_VertexOnCellIterator;
class Triang2D_FacetOnCellIterator;
class Triang2D_FacetIterator;

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

  typedef int vertex_handle;
  typedef int cell_handle;
};

#endif
