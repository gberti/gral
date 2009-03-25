#ifndef NMWR_GB_GRAL_DISTRIBUTED_COMPOSITE_GRID_OUTPUT_H
#define NMWR_GB_GRAL_DISTRIBUTED_COMPOSITE_GRID_OUTPUT_H


/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti, 
    Contact: guntram AT users DOT berlios DOT de

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */



#include <string>
#include "Utility/as-string.h"

namespace GrAL {

/*! \brief Output adapter for composite grids

    \ingroup compositegrids

    This class parallels CompositeGrid<CoarseG,FineG>
 */
template<class CoarseG, class OG>
class composite_grid_output {
  typedef  composite_grid_output<CoarseG, OG> self;

public:
  typedef CoarseG                     coarse_grid_type;
  typedef OG                          ovrlp_grid_type;
  typedef typename OG::fine_grid_type fine_grid_type;                       
  typedef typename OG::overlap_type   overlap_type;                       

  typedef grid_types<coarse_grid_type> cgt;
  typedef typename cgt::Cell           CoarseCell;
  typedef typename cgt::CellIterator   CoarseCellIterator;

private:
  std::string basenm;

  coarse_grid_type                           the_coarse;
  grid_function<CoarseCell,ovrlp_grid_type>  local_grids;

  // FORBIDDEN (for the moment)
  composite_grid_output(self const& rhs);
  self& operator=(self const& rhs);

  std::string coarse_file_nm(std::string const& base)
  { return (basenm + ".coarse");}
  std::string ovlp_grid_base(std::string const& base, 
                             CoarseCell  const& C)
  { return (basenm + "." + as_string(number(*C))); }

  int number(CoarseCell const& C) const { return C.handle();}
public:

  composite_grid_output(std::string const& base) 
    : basenm(base), the_coarse(coarse_file_nm(basenm)) {}

  void coarse_grid_complete(); // update data dependent on coarse grid

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
#include "Gral/Distributed/internal/composite-grid-output.C"
#endif



#endif
