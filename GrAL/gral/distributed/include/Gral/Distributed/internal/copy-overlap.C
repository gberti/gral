#ifndef NMWR_GB_GRAL_DISTRIBUTED_COPY_OVERLAP_C
#define NMWR_GB_GRAL_DISTRIBUTED_COPY_OVERLAP_C

// $LICENSE

#include "Gral/Distributed/copy-overlap.h"
#include "Gral/Distributed/copy-overlap-ranges.h"

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
		 FineCCorr         const&  src2dest_c)      // in
{
  // already done
  //  dest.set_coarse_grid(cg_dest);
  // dest.set_fine_grid(fg_dest);
  
  CopyOverlapRanges(dest.vertices(),src.vertices(),src2dest_v);
  CopyOverlapRanges(dest.cells(),   src.cells()   ,src2dest_c);
  
  // NOTE: dest.neighbour-iteration does not yet work!
  typedef typename Ovlp1::CoarseCell        destCoarseCell;
  typedef typename Ovlp2::CellNbIterator    srcCellNbIterator;
  typedef typename Ovlp2::VertexNbIterator  srcVertexNbIterator;

  for(srcCellNbIterator sCNb = src.FirstCellNeighbour(); ! sCNb.IsDone(); ++sCNb) {
    destCoarseCell dNb = cg_dest.cell(crs_src2dest_c(sCNb.handle()));
    dest.init_bilateral_cell_range(dNb);
    CopyOverlapRanges(dest.cells   (dNb),src.cells   (*sCNb),src2dest_c);
    
  }
  for(srcVertexNbIterator sVNb = src.FirstVertexNeighbour(); ! sVNb.IsDone(); ++sVNb) {
    destCoarseCell dNb = cg_dest.cell(crs_src2dest_c(sVNb.handle()));
    dest.init_bilateral_vertex_range(dNb);
    CopyOverlapRanges(dest.vertices(dNb),src.vertices(*sVNb),src2dest_v);
  }

  dest.calc_dependent_information();
}

} // namespace GrAL 

#endif
