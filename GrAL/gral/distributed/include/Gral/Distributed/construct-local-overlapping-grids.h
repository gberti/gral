#ifndef NMWR_GB_CONSTRUCT_OVERLAPPING_GRIDS_H
#define NMWR_GB_CONSTRUCT_OVERLAPPING_GRIDS_H

//----------------------------------------------------------------
//   (c) Guntram Berti, 1998
//   Chair for Numerical Mathematics & Scientific Computing (NMWR)
//   TU Cottbus - Germany
//   http://math-s.math.tu-cottbus.de/NMWR
//   
//----------------------------------------------------------------

//----------------------------------------------------------------
/*! \brief Enlarge a local grid by overlap
    \ingroup overlappinggrids

  semantics:
  - ovlp_grid.TheGrid()    = Grid(Cells(owned_c \f$ \cup \f$ ovlp.copied()))
  - ovlp_grid.TheOverlap() = ovlp

  \templateparams
  - \c OvlpGrid: e.g. overlapping_grid<CoarseG,FineG> 
  - \c CellRange: $GrAL CellRange
  - \c Geometry: geometry on owned_c \f$\cup\f$ ovlp.copied()
  - \c Overlap: e.g dyn_overlap<CoarseG,FineG>
  - \c VertexCorr: mapping from V(owned_c \f$\cup\f$ ovlp.copied())
       to V(ovlp_grid)
  - \c CellCorr:from C(owned_c \f$\cup\f$ ovlp.copied())
       to C(ovlp_grid) 
  
  \param ovlp_grid: OUT, initially empty.
  \param owned_c: IN, range of owned cells to be copied
  \param geom: IN, geometry on owned & copied grid parts
  \param ovlp: IN/OUT, exposed/shared/copied ranges are ok,
    private ranges will be set.
  \param v_corr: OUT, will be set to map vertices of src grid parts to 
      ovlp_grid.TheGrid()
  \param c_corr: OUT, will be set to map cells of src grid parts 
      to ovlp_grid.TheGrid()

  \todo Fix geometry construction! There must be either a destination
  geometry on ovlp_grid.TheGrid(), or no geometries at all, handling
  geometry later on, using v_corr and c_corr.
*/
//----------------------------------------------------------------


template<class OvlpGrid, class CellRange, class Geometry,
         class Overlap, class VertexCorr, class CellCorr>
void ConstructLocalOverlappingGrid(OvlpGrid       & ovlp_grid, // OUT
				   CellRange const& owned_c,   // IN
				   Geometry  const& geom,      // IN
				   Overlap        & ovlp,      // INOUT
				   VertexCorr     & v_corr,    // OUT
				   CellCorr       & c_corr);   // OUT


#ifdef NMWR_INCLUDE_TEMPLATE_DEFS
#include "Gral/Distributed/internal/construct-local-overlapping-grids.C"
#endif

#endif
