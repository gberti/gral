#ifndef NMWR_GB_OVERLAPPING_GRID_INPUT_H
#define NMWR_GB_OVERLAPPING_GRID_INPUT_H

// $LICENSE

/*! \brief Input adapter for an overlapping grid
    \ingroup overlappinggrids

    It is almost identical to OverlappingGrid<COARSEG,FINEG>.
 */
#include "Gral/Distributed/overlap-input.h"

namespace GrAL {

template<class COARSEG, class FINEG, 
         class OVLP = overlap_input<COARSEG,FINEG, overlap_range_input> >
class overlapping_grid_input 
{
  typedef overlapping_grid_input<COARSEG, FINEG, OVLP> self;

public:
  typedef COARSEG coarse_grid_type;
  typedef FINEG   fine_grid_type;
  typedef OVLP    overlap_type;

private:

  //-------- owned data ---------------
  fine_grid_type     the_fine_grid;
  overlap_type       the_overlap;

  std::string        basenm;

  //-------- referenced data ----------
  
  const coarse_grid_type* the_coarse_grid;

public:
  //----------------------   construction   ----------------------
  
  overlapping_grid_input() : the_coarse_grid(0) {} 

  void init(coarse_grid_type const& cg, 
            std::string      const& basenm_);

  // void fine_grid_complete() { init_overlap(); }

  void calc_dependent_information() { /*  none */ }

private:

  void init_overlap() {
    the_overlap.init(TheCoarseGrid(), TheGrid(), basenm);
  }

public:
  //---------------------- component access ----------------------

  coarse_grid_type const& TheCoarseGrid() const { return *the_coarse_grid;}

  fine_grid_type const& TheGrid() const { return the_fine_grid;}
  fine_grid_type      & TheGrid()       { return the_fine_grid;}

  overlap_type const& TheOverlap() const { return the_overlap;}
  overlap_type      & TheOverlap()       { return the_overlap;}

};

} // namespace GrAL 

#ifdef NMWR_INCLUDE_TEMPLATE_DEFS
#include "Gral/Distributed/internal/overlapping-grid-input.C"
#endif



#endif
