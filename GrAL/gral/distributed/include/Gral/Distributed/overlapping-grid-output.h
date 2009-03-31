#ifndef NMWR_GB_OVERLAPPING_GRID_OUTPUT_H
#define NMWR_GB_OVERLAPPING_GRID_OUTPUT_H


/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


/*! \brief Output adapter for an overlapping grid
    \ingroup overlappinggrids

    It is almost identical to OverlappingGrid<COARSEG,FINEG>.
 */
#include "Gral/Distributed/overlap-output.h"
#include "Gral/Distributed/overlap-range-output.h"

namespace GrAL {

template<class COARSEG, class FINEG, 
         class OVLP = overlap_output<COARSEG,FINEG, overlap_range_output> >
class overlapping_grid_output 
{
  typedef overlapping_grid_output<COARSEG, FINEG, OVLP> self;

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
  
  overlapping_grid_output() : the_coarse_grid(0) {} 
  overlapping_grid_output(coarse_grid_type const& cg, 
			  std::string      const& basenm_)
  { init(cg,basenm_);}

  void init(coarse_grid_type const& cg, 
            std::string      const& basenm_) 
  {
    the_coarse_grid = &cg;
    basenm = basenm_;
    the_fine_grid.init(basenm + ".grid");
  }

  void fine_grid_complete() { init_overlap(); }

  void calc_dependent_information() { /*  none */ }

private:

  void init_overlap() {
    the_overlap.init(TheCoarseGrid(), TheGrid(), basenm);
    /*
      the_overlap.set_coarse_grid(*the_coarse_grid);
      the_overlap.set_fine_grid  (the_fine_grid);
      the_overlap.init(basenm);
    */
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

#endif
