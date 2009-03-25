#ifndef NMWR_GB_GRAL_DISTRIBUTED_COMPOSITE_OUTPUT_C
#define NMWR_GB_GRAL_DISTRIBUTED_COMPOSITE_OUTPUT_C


/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti, 
    Contact: guntram AT users DOT berlios DOT de

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include "Gral/Distributed/composite-grid-output.h"
#include "Utility/as-string.h"

namespace GrAL {

template<class CoarseG, class OG>
void composite_grid_output<CoarseG,OG>::coarse_grid_complete() {
  local_grids.set_grid(TheCoarseGrid());
  for(CoarseCellIterator C(TheCoarseGrid()); ! C.IsDone(); ++C) {
    local_grids[*C].init(TheCoarseGrid(), basenm + "." + as_string(number(*C)));
  }
}

} // namespace GrAL 

#endif
