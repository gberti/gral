#ifndef NMWR_GB_GRAL_DISTRIBUTED_COMPOSITE_OUTPUT_C
#define NMWR_GB_GRAL_DISTRIBUTED_COMPOSITE_OUTPUT_C

// $LICENSE

#include "Gral/Distributed/composite-grid-output.h"
#include "Utility/as-string.h"

template<class CoarseG, class OG>
void composite_grid_output<CoarseG,OG>::coarse_grid_complete() {
  local_grids.set_grid(TheCoarseGrid());
  for(CoarseCellIterator C(TheCoarseGrid()); ! C.IsDone(); ++C) {
    local_grids[*C].init(TheCoarseGrid(), basenm + "." + as_string(number(*C)));
  }
}


#endif
