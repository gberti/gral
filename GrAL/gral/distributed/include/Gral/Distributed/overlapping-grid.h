#ifndef NMWR_GB_OVERLAPPING_GRID_H
#define NMWR_GB_OVERLAPPING_GRID_H 



/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include "Gral/Distributed/overlap.h"

namespace GrAL {

/*! \defgroup overlappinggrids Overlapping Grids

    \brief Grids and grid functions augmented with %overlap ranges.

    \ingroup distributedgrids
*/


/*! \brief Sequential grid augmented with an %overlap.
    \ingroup overlappinggrids

 An overlapping grid augments a base grid with the notion of
 overlap ranges that capture its relationship to other parts
 of a (conceptual) global grid.

 However, an overlapping grid has no idea of the context it is
 contained in, which may be a really distributed environment with message
 passing or just a part of a composite grid.

 \partof overlappinggrids
*/


template<class CoarseGrid, class FineGrid>
class OverlappingGrid  {
public:
  typedef OverlappingGrid<CoarseGrid,FineGrid> self;
  typedef FineGrid    fine_grid_type;
  typedef CoarseGrid  coarse_grid_type;
  //typedef grid_types<coarse_grid_type> cgt;
  typedef grid_types<fine_grid_type> fgt;

  // typedef typename cgt::Cell           CoarseCell;
  typedef overlap<coarse_grid_type, fine_grid_type> overlap_type;
  //typedef overlap<fine_grid_type> overlap_type;
  typedef typename overlap_type::range_type_ref     range_type_ref;

private:

  //-------- owned DATA ---------------
  fine_grid_type     the_fine_grid;
  overlap_type       the_overlap;

  //CoarseCell         myCell; // position in coarse grid

  //-------- referenced DATA ----------
  
  const coarse_grid_type* the_coarse_grid;

public:
  //----------------------   construction   ----------------------
  
  OverlappingGrid() : the_coarse_grid(0) {} 

  void init(const coarse_grid_type& cg) { set_coarse_grid(cg);}

  void fine_grid_complete() { init_overlap(); }

  void calc_dependent_information() { /*  none */ }

private:
  void set_coarse_grid(const coarse_grid_type& cg)
    { the_coarse_grid = &cg;}

  void init_overlap() {
    the_overlap.init(TheCoarseGrid(), TheGrid());
  }

public:
  //---------------------- component access ----------------------

  coarse_grid_type const& TheCoarseGrid() const { return *the_coarse_grid;}

  fine_grid_type const& TheGrid() const { return the_fine_grid;}
  fine_grid_type      & TheGrid()       { return the_fine_grid;}

  overlap_type const& TheOverlap() const { return the_overlap;}
  overlap_type      & TheOverlap()       { return the_overlap;}

  range_type_ref   LocalRange() const { return the_overlap.local_range();}  
};

} // namespace GrAL 

#endif
