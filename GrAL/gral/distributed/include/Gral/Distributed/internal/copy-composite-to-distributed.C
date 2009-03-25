#ifndef NMWR_GB_GRAL_DISTRIBUTED_COPY_COMPOSITE_TO_DISTRIBUTED_C
#define NMWR_GB_GRAL_DISTRIBUTED_COPY_COMPOSITE_TO_DISTRIBUTED_C



/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti, 
    Contact: guntram AT users DOT berlios DOT de

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include "Gral/Distributed/copy-composite-to-distributed.h"
#include "Gral/Distributed/copy-overlapping-grid.h"

#include "Gral/Base/construct-grid.h"
#include "Gral/Base/grid-morphism.h"

namespace GrAL {

template
< class DG,          // distributed grid
  class CG_SRC,      // composite grid (src)
  class CCORR_CRS,   //  grid morphism CG_SRC::coarse_grid -> CG::coarse_grid
  class VCORR,       //  vertex grid morphism CG_SRC::fine_grid -> DG::the_grid
  class CCORR        //  cell   grid morphism CG_SRC::fine_grid -> DG::the_grid
>
void CopyComposite2Distributed
(
 DG               & dg_dest,    // OUT
 CG_SRC      const& cg_src,     // IN
 CCORR_CRS        & src2dest_coarse_c, // OUT: C(Coarse_src) -> C(Coarse)
 VCORR            & src2dest_fine_v,   // OUT: C(Coarse_src) -> (V(FG_src) -> V(FG))
 CCORR            & src2dest_fine_c    // OUT: C(Coarse_src) -> (C(FG_src) -> C(FG))
 )
{

  // copy the coarse grid
  vertex_morphism<
    typename CG_SRC::coarse_grid_type,
    typename DG    ::coarse_grid_type> dummy_coarse_v(cg_src .TheCoarseGrid(),
                                                      dg_dest.TheCoarseGrid());
  ConstructGrid0(dg_dest.TheCoarseGrid(),
                 cg_src .TheCoarseGrid(), 
                 dummy_coarse_v, 
                 src2dest_coarse_c);
  dg_dest.coarse_grid_complete(); // should use notify mechanism !!  

  // copy the local overlapping grids
  typename CG_SRC::CoarseCell C = src2dest_coarse_c.inverse()(dg_dest.MyCell());
  CopyOverlappingGrid(dg_dest.TheOvrlpGrid(),
                      cg_src .OvrlpGrid(C),
                      src2dest_coarse_c,
                      src2dest_fine_v[C],
                      src2dest_fine_c[C]);
  

  // dg_dest.calc_dependent_information();

}

} // namespace GrAL 

#endif
