#ifndef NMWR_GB_GRAL_DISTRIBUTED_COPY_COMPOSITE_C
#define NMWR_GB_GRAL_DISTRIBUTED_COPY_COMPOSITE_C


// $LICENSE

#include "Gral/Distributed/copy-composite.h"
#include "Gral/Distributed/copy-overlapping-grid.h"

#include "Gral/Base/construct-grid.h"
#include "Gral/Base/grid-morphism.h"

namespace GrAL {

template
< class CG,          // composite grid
  class CG_SRC,      // composite grid (src)
  class CCORR_CRS,   //  grid morphism CG_SRC::coarse_grid -> CG::coarse_grid
  class VCORR,       //  vertex grid morphism CG_SRC::fine_grid -> CG::fine_grid
  class CCORR        //  cell   grid morphism CG_SRC::fine_grid -> CG::fine_grid
>
void CopyComposite(CG          & cg_dest,    // OUT
                   CG_SRC const& cg_src,     // IN
                   CCORR_CRS   & src2dest_coarse_c, // OUT: C(Coarse_src) -> C(Coarse)
                   VCORR       & src2dest_fine_v,   // OUT: C(Coarse_src) -> (V(FG_src) -> V(FG))
                   CCORR       & src2dest_fine_c    // OUT: C(Coarse_src) -> (C(FG_src) -> C(FG))
                   )
{

  // copy the coarse grid
  vertex_morphism<
    typename CG_SRC::coarse_grid_type,
    typename CG    ::coarse_grid_type> dummy_coarse_v(cg_src .TheCoarseGrid(),
                                                      cg_dest.TheCoarseGrid());
  ConstructGrid0(cg_dest.TheCoarseGrid(),
                 cg_src .TheCoarseGrid(), 
                 dummy_coarse_v, 
                 src2dest_coarse_c);
  cg_dest.coarse_grid_complete(); // should use notify mechanism !!  

  // copy the local overlapping grids
  typedef typename CG_SRC::CoarseCellIterator  srcCoarseCellIterator;
  typedef typename CG    ::CoarseCell          destCoarseCell;
  for(srcCoarseCellIterator srcC(cg_src.TheCoarseGrid()); ! srcC.IsDone(); ++srcC) {
    destCoarseCell destC = cg_dest.TheCoarseGrid().cell(src2dest_coarse_c(srcC.handle()));
    CopyOverlappingGrid(cg_dest.OvrlpGrid(destC),
                        cg_src .OvrlpGrid(*srcC),
                        src2dest_coarse_c,
                        src2dest_fine_v[*srcC],
                        src2dest_fine_c[*srcC]);

 
  }

  // cg_dest.calc_dependent_information();

}

} // namespace GrAL 

#endif
