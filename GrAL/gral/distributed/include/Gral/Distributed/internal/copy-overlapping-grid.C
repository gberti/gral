#ifndef NMWR_GB_GRAL_DISTRIBUTED_COPY_OVERLAPPING_GRID_C
#define NMWR_GB_GRAL_DISTRIBUTED_COPY_OVERLAPPING_GRID_C

// $LICENSE

#include "Gral/Distributed/copy-overlapping-grid.h"
#include "Gral/Distributed/copy-overlap.h"

namespace GrAL {

template
< class OG,          //  overlapping grid
  class OG_SRC,      //  overlapping grid (src)
  class CCORR_CRS,   //  grid morphism CG_SRC::coarse_grid -> CG::coarse_grid
  class VCORR,       //  vertex grid morphism OG_SRC::local_grid -> OG::local_grid
  class CCORR        //  cell   grid morphism OG_SRC::local_grid -> OG::local_grid
>
void CopyOverlappingGrid(OG             & og_dest,
                         OG_SRC    const& og_src,
                         CCORR_CRS const& src2dest_coarse_c, // IN: C(Coarse_src) -> C(Coarse)
                         VCORR          & src2dest_v,
                         CCORR          & src2dest_c)
{
  ConstructGrid0(og_dest.TheGrid(),
                 og_src .TheGrid(),
                 src2dest_v,
                 src2dest_c);

  og_dest.fine_grid_complete();

  CopyOverlap(og_dest.TheOverlap(),
              og_src .TheOverlap(),
              og_dest.TheCoarseGrid(),
              src2dest_coarse_c,
              og_dest.TheGrid(),
              src2dest_v,
              src2dest_c);

  // og_dest.calc_dependent_information()
}

} // namespace GrAL 

#endif
