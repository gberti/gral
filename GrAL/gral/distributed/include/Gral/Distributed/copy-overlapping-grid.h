#ifndef NMWR_GB_GRAL_DISTRIBUTED_COPY_OVERLAPPING_GRID_H
#define NMWR_GB_GRAL_DISTRIBUTED_COPY_OVERLAPPING_GRID_H

// $LICENSE

#include "Container/dummy-mapping.h"

#include "Gral/Base/common-grid-basics.h"
#include "Gral/Base/grid-morphism.h"

template
< class OG,          //  overlapping grid
  class OG_SRC,      //  overlapping grid (src)
  class CCORR_CRS,   //  grid morphism CG_SRC::coarse_grid -> CG::coarse_grid
  class VCORR,       //  vertex grid morphism OG_SRC::local_grid -> OG::local_grid
  class CCORR        //  cell   grid morphism OG_SRC::local_grid -> OG::local_grid
>
void CopyOverlappingGrid(OG          & og,
                         OG_SRC const& og_src,
                         CCORR_CRS   const& src2dest_coarse_c, // IN: C(Coarse_src) -> C(Coarse)
                         VCORR       & src2dest_v,
                         CCORR       & src2dest_c);

template
< class OG,          //  overlapping grid
  class OG_SRC,      //  overlapping grid (src)
  class CCORR_CRS    //  grid morphism CG_SRC::coarse_grid -> CG::coarse_grid
>
inline
void CopyOverlappingGrid(OG             & og,
                         OG_SRC    const& og_src,
                         CCORR_CRS const& src2dest_coarse_c) // IN: C(Coarse_src) -> C(Coarse)
{
    typedef grid_types<typename OG_SRC::fine_grid_type>   src_fgt;
    typedef grid_types<typename OG    ::fine_grid_type>   dest_fgt;
    dummy_mapping<typename src_fgt::vertex_handle, typename dest_fgt::vertex_handle>
	dummy_src2dest_v;
    dummy_mapping<typename src_fgt::cell_handle,   typename dest_fgt::cell_handle>
	dummy_src2dest_c;
    // FIXME: does not work if dummy_src2dest_v is really needed in construction of grid!
    // Should somehow query OG::fine_grid_type for a dummy vertex map!
    CopyOverlappingGrid(og,
			og_src,
			src2dest_coarse_c,
			dummy_src2dest_v,
			dummy_src2dest_c);
}

#ifdef NMWR_INCLUDE_TEMPLATE_DEFS
#include "Gral/Distributed/internal/copy-overlapping-grid.C"
#endif




#endif
