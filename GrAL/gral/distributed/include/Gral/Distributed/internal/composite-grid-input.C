#ifndef NMWR_GB_GRAL_DISTRIBUTED_COMPOSITE_INPUT_C
#define NMWR_GB_GRAL_DISTRIBUTED_COMPOSITE_INPUT_C


/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti, 
    Contact: guntram AT users DOT berlios DOT de

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include "Gral/Distributed/composite-grid-input.h"
#include "Utility/as-string.h"

namespace GrAL {

template<class CoarseGIn, class CoarseG, class OGIn>
composite_grid_input<CoarseGIn,CoarseG,OGIn>
::composite_grid_input(std::string const& base)
  : basenm(base)
{
  CoarseGIn crs_in(coarse_file_nm(basenm));
  ConstructGrid0(the_coarse,crs_in);

  local_grids.set_grid(TheCoarseGrid());
  for(CoarseCellIterator C(TheCoarseGrid()); ! C.IsDone(); ++C) {
    local_grids[*C].init(TheCoarseGrid(), basenm + "." + as_string(number(*C)));
  }
  
}

} // namespace GrAL 

#endif
