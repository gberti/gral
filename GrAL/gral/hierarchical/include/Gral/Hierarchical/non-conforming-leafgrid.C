#ifndef GRAL_GB_NON_CONFORMING_LEAF_GRID_C
#define GRAL_GB_NON_CONFORMING_LEAF_GRID_C


// $LICENSE_NEC

#include "Gral/Hierarchical/non-conforming-leafgrid.h"

namespace GrAL {

namespace octree {

  template<class OCTREE>
  size_t non_conforming_leafgrid<OCTREE>::NumOfVertices() const
  {
    VertexIterator v(FirstVertex());
    size_t cnt = 0;
    while(! v.IsDone()) {
      ++cnt;
      ++v;
    }
    return cnt;
  }
} // namespace octree

} // namespace GrAL 

#endif
