/*! \file 

     Testing non conforming leaf grids of an octree.

*/

#include "Gral/Hierarchical/non-conforming-leafgrid.h"

#include "Gral/Grids/Cartesian2D/all.h"
#include "Gral/Grids/Cartesian3D/all.h"

namespace octree {
  typedef Octree<cartesian2d::CartesianGrid2D> octree2d_type;
  typedef Octree<cartesian3d::CartesianGrid3D> octree3d_type;

  template class non_conforming_leafgrid<octree2d_type>;
  // template class non_conforming_leafgrid<octree3d_type>;
 
  typedef non_conforming_leafgrid<octree2d_type>::element_base_type elem_base2d_type;
  // typedef non_conforming_leafgrid<octree3d_type>::element_base_type elem_base3d_type;

  
  template class hierarchical::h_vertex_t<elem_base2d_type>;
  // cannot instantiate this, because CellChildIterator is not defined.
  // template class hierarchical::h_cell_t  <elem_base2d_type>;
  template class hierarchical::h_vertex_on_cell_iterator_t  <elem_base2d_type>;
 
}

int main() {
   typedef Octree<cartesian2d::CartesianGrid2D> octree2d_type;
  
}
