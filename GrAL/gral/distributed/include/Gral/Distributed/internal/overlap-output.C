#ifndef NMWR_GB_OVERLAP_OUTPUT_C
#define NMWR_GB_OVERLAP_OUTPUT_C

#include "Gral/Distributed/overlap-output.h"
#include <fstream>

namespace GrAL {

template<
  class CoarseGrid, 
  class FineGrid,
  template<class U> class OVLP_RANGES 
  >
void overlap_output<CoarseGrid, FineGrid, OVLP_RANGES>::set_neighbours()
{
  neighbours.set_grid(TheCoarseGrid());
  std::ofstream nbout((basenm + ".neighbors").c_str());
  for(VertexNbIterator VNb = FirstVertexNeighbour(); ! VNb.IsDone(); ++VNb) {
    neighbours.push_back(*VNb);
    nbout << VNb.handle() <<  ' ';
  }
  
  // add neighbours from cell ranges not already contained in neighbours
  for(CellNbIterator CNb = FirstCellNeighbour(); ! CNb.IsDone(); ++CNb) {
    bool found = false;
    for (NbIterator N = neighbours.FirstCell() ; ! N.IsDone(); ++N)
      if(*N == *CNb) {
        found = true;
        break;
      }
    if ( ! found) {
      neighbours.push_back(*CNb);
      nbout << CNb.handle() << ' ';
    }
  }
  nbout << std::endl;
  nbout.close();
}

template<
  class CoarseGrid, 
  class FineGrid,
  template<class U> class OVLP_RANGES 
  >
void overlap_output<CoarseGrid, FineGrid, OVLP_RANGES>
::init(typename overlap_output<CoarseGrid, FineGrid, OVLP_RANGES>::coarse_grid_type const& cg,
       typename overlap_output<CoarseGrid, FineGrid, OVLP_RANGES>::  fine_grid_type const& fg,
       std::string const& basenm_)
{
  basenm = basenm_;
  set_coarse_grid(cg);
  set_fine_grid  (fg);

  total_ranges_v.init(basenm + ".total.vertices");
  total_ranges_c.init(basenm + ".total.cells");
}

} // namespace GrAL 

#endif
