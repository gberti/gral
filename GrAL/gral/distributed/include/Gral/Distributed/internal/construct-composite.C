#ifndef NMWR_GB_CONSTRUCT_COMPOSITE_C
#define NMWR_GB_CONSTRUCT_COMPOSITE_C



/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti, 
    Contact: guntram AT users DOT berlios DOT de

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include "Gral/Distributed/construct-composite.h"

#include "Container/bijective-mapping.h"

#include "Gral/Partitioning/coarse-grid-from-partitioning.h"

#include "Gral/Distributed/dyn-overlap.h"
#include "Gral/Distributed/construct-overlap.h"
#include "Gral/Distributed/construct-local-overlapping-grids.h"
#include "Gral/Distributed/internal/grid-self-map.h"

namespace GrAL {

template<class CompositeG, class Partition, class Geometry, 
         class OVPattern, class VCorr, class CCorr>
void ConstructComposite(CompositeG     & CG,
			Partition const& Prtng,
			Geometry  const& geom, 
			OVPattern const& ovlp_pattern,
			VCorr          & v_corr,
			CCorr          & c_corr)
{

  typedef typename  CompositeG::coarse_grid_type  coarse_grid_type;
  typedef grid_types<coarse_grid_type>            cgt;
  typedef typename cgt::Cell                      CoarseCell;
  typedef typename cgt::CellIterator              CoarseCellIterator;
  typedef typename cgt::vertex_handle             coarse_vertex_handle;
  typedef typename cgt::cell_handle               coarse_cell_handle;
  
  //  typedef typename Partition::grid_type           fine_grid_type;
  typedef typename CompositeG::fine_grid_type      fine_grid_type;
  typedef grid_types<fine_grid_type>               fgt;
  // typedef grid_types<fine_grid_type>::Vertex    Vertex;
  // typedef grid_types<fine_grid_type>::Facet     Facet;
  typedef typename fgt::Cell                      Cell;
  typedef typename fgt::vertex_handle             vertex_handle;

  //------------------ construct coarse grid and correspondencies -----------------

  bijective_mapping<coarse_cell_handle,   int>            CoarseCellCorr;
  bijective_mapping<coarse_vertex_handle, vertex_handle>  CoarseVertexCorr;

  ConstructCoarsePartitionGrid(CG.TheCoarseGrid(),Prtng, 			       
			       CoarseVertexCorr,
			       CoarseCellCorr);
  CG.coarse_grid_complete();

  v_corr.set_grid(CG.TheCoarseGrid());
  c_corr.set_grid(CG.TheCoarseGrid());

  coarse_grid_type& CrsG(CG.TheCoarseGrid());

  grid_function<CoarseCell,int> cell2part(CG.TheCoarseGrid());
  bijective_mapping<int, CoarseCell> part2cell;
  for(CoarseCellIterator P = CrsG.FirstCell(); ! P.IsDone(); ++P) {
    cell2part[*P] = CoarseCellCorr(CrsG.handle(*P));
    part2cell[cell2part(*P)] = *P;
  }


  //--------------------- construct overlap structures  -------------------------

  // overlap tied to the global grid
  typedef dyn_overlap<coarse_grid_type, fine_grid_type> overlap_type;
  grid_function<CoarseCell, overlap_type>               Ovlp(CrsG);
  for(CoarseCellIterator P0 = CrsG.FirstCell(); ! P0.IsDone(); ++P0) {
    Ovlp[*P0].init(CrsG,Prtng.TheGrid());
  }

  // identity mapping
  GridSelfMap<fine_grid_type> grid_map(Prtng.TheGrid());
  ConstructOverlap(Ovlp,CrsG,Prtng,ovlp_pattern, part2cell, cell2part, grid_map);


  //--------------------- construct local grids  ---------------------------------

  for(CoarseCellIterator P1 = CrsG.FirstCell(); ! P1.IsDone(); ++P1) {
    CG.OvrlpGrid(*P1).init(CG.TheCoarseGrid());
    ConstructLocalOverlappingGrid(CG.OvrlpGrid(*P1),
				  Prtng.Range(cell2part(*P1)), geom,
				  Ovlp[*P1], v_corr[*P1], c_corr[*P1]);
  }

  //---------------------------- do checkings   ------------------------------------

  check_composite_grid(CG);

}

} // namespace GrAL 

#endif
