#ifndef NMWR_GB_GRAL_DISTRIBUTED_COMPOSITE_GRID_INPUT_H
#define NMWR_GB_GRAL_DISTRIBUTED_COMPOSITE_GRID_INPUT_H


/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */



#include <string>
#include "Utility/as-string.h"

namespace GrAL {

/*! \brief Input adapter for composite grids

    \ingroup compositegrids

    This class parallels CompositeGrid<CoarseG,FineG>
 */
template<class CoarseGIn, class CoarseG, class OGIn>
class composite_grid_input {
  typedef  composite_grid_input<CoarseGIn, CoarseG, OGIn> self;

public:
  typedef CoarseGIn                     coarse_grid_in_type;
  typedef CoarseG                       coarse_grid_type;
  typedef OGIn                          ovrlp_grid_type;
  typedef typename OGIn::fine_grid_type   fine_grid_type;                       
  typedef typename OGIn::overlap_type     overlap_type;                       

  typedef grid_types<coarse_grid_type> cgt;
  typedef typename cgt::Cell           CoarseCell;
  typedef typename cgt::CellIterator   CoarseCellIterator;

private:
  std::string basenm;

  coarse_grid_type                           the_coarse;
  grid_function<CoarseCell,ovrlp_grid_type>  local_grids;

  // FORBIDDEN (for the moment)
  composite_grid_input(self const& rhs);
  self& operator=(self const& rhs);

  std::string coarse_file_nm(std::string const& base)
  { return (base + ".coarse");}
  std::string ovlp_grid_base(std::string const& base, 
                             CoarseCell  const& C)
  { return (base + "." + as_string(number(*C))); }

  int number(CoarseCell const& C) const { return C.handle();}
public:

  composite_grid_input(std::string const& base);

  //  void coarse_grid_complete(); // update data dependent on coarse grid

  void calc_dependent_information() { /*  none */ }

  //-------------------- component access ----------------------------

  coarse_grid_type      & TheCoarseGrid()       { return the_coarse;}
  coarse_grid_type const& TheCoarseGrid() const { return the_coarse;}

  ovrlp_grid_type      &  OvrlpGrid(const CoarseCell& c)       { return local_grids[c];}
  ovrlp_grid_type const&  OvrlpGrid(const CoarseCell& c) const { return local_grids(c);}

  fine_grid_type       &  Grid(const CoarseCell& c)       { return local_grids[c].TheGrid();}
  fine_grid_type  const&  Grid(const CoarseCell& c) const { return local_grids[c].TheGrid();}

  overlap_type const& Overlap(const CoarseCell& c) const { return OvrlpGrid(c).TheOverlap();}
  overlap_type      & Overlap(const CoarseCell& c)       { return OvrlpGrid(c).TheOverlap();}

};

} // namespace GrAL 

#ifdef NMWR_INCLUDE_TEMPLATE_DEFS
#include "Gral/Distributed/internal/composite-grid-input.C"
#endif



#endif
