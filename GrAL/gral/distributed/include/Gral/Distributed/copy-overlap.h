#ifndef NMWR_GB_GRAL_DISTRIBUTED_COPY_OVERLAP_H
#define NMWR_GB_GRAL_DISTRIBUTED_COPY_OVERLAP_H

// $LICENSE

namespace GrAL {

template
< class Ovlp1, 
  class Ovlp2,
  class CG1,        // Ovlp1::coarse_grid_type
  class CoarseCCorr, 
  class FG1,        // Ovlp1::fine_grid_type
  class FineVCorr, 
  class FineCCorr
>
void CopyOverlap(Ovlp1                  &  dest,            // out
		 Ovlp2             const&  src,             // in
		 CG1               const&  cg_dest,         // in
		 CoarseCCorr       const&  crs_src2dest_c,  // in 
		 FG1               const&  fg_dest,         // in
		 FineVCorr         const&  src2dest_v,      // in 
		 FineCCorr         const&  src2dest_c);     // in

} // namespace GrAL 

#ifdef NMWR_INCLUDE_TEMPLATE_DEFS
#include "Gral/Distributed/internal/copy-overlap.C"
#endif

#endif
