#ifndef GRAL_BASE_GB_FACET_HANDLE_CORR_H
#define GRAL_BASE_GB_FACET_HANDLE_CORR_H

//----------------------------------------------------------------
//
//   (c) Guntram Berti, 1999
//   Chair for Numerical Mathematics & Scientific Computing (NMWR)
//   TU Cottbus - Germany
//   http://math-s.math.tu-cottbus.de/NMWR
//   
//----------------------------------------------------------------

#include "Gral/Base/common-grid-basics.h"

//----------------------------------------------------------------
//  facet_handle_corr - map facet handle of a source grid to that
//                      of a target grid.
//  It is assumed a facet handle is a pair (c,lf) of cell_handle
//  and local facet number, as is the case for the facet_handle<>
//  template ( Gral/Base/facet-handle.h )
//  This class is rather ad hoc in that it assumes that the 
//  corr. facet of the target has the same local number with
//  respect to the corresponding cell.
//
//----------------------------------------------------------------


template<class VCorr, class CCorr, class G>
class facet_handle_corr {
private:
  VCorr const* v_corr; // currently not used
  CCorr const* c_corr;
  // source_grid_type const& g_s;
  // target_grid_type const& g_t;
  // could be contained in the vertex/cell corr. mappings,
  // and be used to obtain a more stable mapping.

  typedef G                         target_grid_type;  
  typedef grid_types<G>             gt;
  typedef typename gt::facet_handle facet_handle;
public:
  facet_handle_corr(VCorr const& vc, CCorr const& cc)
    : v_corr(&vc), c_corr(&cc) {}

  // this is ad hoc and assumes that the target grid (for which the 
  // resulting handle is) is copied from the source.
  facet_handle operator()(const facet_handle& f) const 
    { return facet_handle((*c_corr)(f.c), f.local_facet());}
};

#endif
