#ifndef NMWR_GB_GRAL_DISTRIBUTED_COMPOSITE_OUTPUT_H
#define NMWR_GB_GRAL_DISTRIBUTED_COMPOSITE_OUTPUT_H


/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti, 
    Contact: guntram AT users DOT berlios DOT de

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */



#include <string>

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
  { return (base + ".coarse");}
  std::string ovlp_grid_base(std::string const& base, 
                             CoarseCell  const& C)
  { return (base + "." + as_string(C.handle())); }
public:

  composite_grid_output(std::string const& base) 
    : basenm(base), the_coarse(coarse_file_nm(base)) {}

  void coarse_grid_changed() { }


  //-------------------- component access ----------------------------

  
  
};

} // namespace GrAL 

#ifdef NMWR_INCLUDE_TEMPLATE_DEFS
#include "Gral/Distributed/internal/composite-output.C"
#endif



#endif
