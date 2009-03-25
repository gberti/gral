#ifndef GRAL_GB_NON_CONFORMING_LEAF_GRID_C
#define GRAL_GB_NON_CONFORMING_LEAF_GRID_C



/* ------------------------------------------------------------

    Copyright (C) 2001 - 2009 Guntram Berti, 
    Contact: guntram AT users DOT berlios DOT de

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


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
