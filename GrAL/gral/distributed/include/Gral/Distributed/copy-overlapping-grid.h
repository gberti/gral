#ifndef NMWR_GB_GRAL_DISTRIBUTED_COPY_OVERLAPPING_GRID_H
#define NMWR_GB_GRAL_DISTRIBUTED_COPY_OVERLAPPING_GRID_H

// $LICENSE

template
< class OG,          //  overlapping grid
  class OG_SRC,      //  overlapping grid (src)
  class CCORR_CRS,   //  grid morphism CG_SRC::coarse_grid -> CG::coarse_grid
  class VCORR,       //  vertex grid morphism OG_SRC::local_grid -> OG::local_grid
  class CCORR        //  cell   grid morphism OG_SRC::local_grid -> OG::local_grid
>
void CopyOverlappingGrid(OG          & og,
                         OG_SRC const& og_src,
                         CCORR_CRS   & src2dest_coarse_c, // IN: C(Coarse_src) -> C(Coarse)
                         VCORR       & src2dest_v,
                         CCORR       & src2dest_c);


#ifdef NMWR_INCLUDE_TEMPLATE_DEFS
#include "Gral/Distributed/internal/copy-overlapping-grid.C"
#endif




#endif
