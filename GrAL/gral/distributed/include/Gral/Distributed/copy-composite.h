#ifndef NMWR_GB_GRAL_DISTRIBUTED_COPY_COMPOSITE_H
#define NMWR_GB_GRAL_DISTRIBUTED_COPY_COMPOSITE_H


// $LICENSE

namespace GrAL {

/*! \defgroup compositeio Composite Grid I/O and copying
    \ingroup compositegrids
 */

/*! \brief Construct a composite grid from another one.
    \ingroup compositeio

    This function allows  to construct a composite grid from another
    composite grid.
  
    - If CG_SRC is an input adapter like composite_grid_input,
      this function implements file input for composite grids;
    - If CG is an output adapter like composite_grid_output, CopyComposite 
      implements file output for composite grids.
*/
template
< class CG,          // composite grid
  class CG_SRC,      // composite grid (src)
  class CCORR_CRS,   //  grid morphism CG_SRC::coarse_grid -> CG::coarse_grid
  class VCORR,       //  vertex grid morphism CG_SRC::fine_grid -> CG::fine_grid
  class CCORR        //  cell   grid morphism CG_SRC::fine_grid -> CG::fine_grid
>
void CopyComposite(CG          & cg,         // OUT
                   CG_SRC const& cg_src,     // IN
                   CCORR_CRS   & src2dest_coarse_c, // OUT: C(Coarse_src) -> C(Coarse)
                   VCORR       & src2dest_fine_v,   // OUT: C(Coarse_src) -> (V(FG_src) -> V(FG))
                   CCORR       & src2dest_fine_c    // OUT: C(Coarse_src) -> (C(FG_src) -> C(FG))
                   );

} // namespace GrAL 

#ifdef NMWR_INCLUDE_TEMPLATE_DEFS
#include "Gral/Distributed/internal/copy-composite.C"
#endif

#endif
