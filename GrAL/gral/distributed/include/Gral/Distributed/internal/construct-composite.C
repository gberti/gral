#ifndef NMWR_GB_CONSTRUCT_COMPOSITE_C
#define NMWR_GB_CONSTRUCT_COMPOSITE_C

//----------------------------------------------------------------
//   (c) Guntram Berti, 1998
//   Chair for Numerical Mathematics & Scientific Computing (NMWR)
//   TU Cottbus - Germany
//   http://math-s.math.tu-cottbus.de/NMWR
//   
//----------------------------------------------------------------

#include "Grids/Distributed/construct-composite.h"

#include "Container/bijective-mapping.h"

#include "Grids/Algorithms/coarse-grid-from-partitioning.h"

#include "Grids/Distributed/dyn-overlap.h"
#include "Grids/Distributed/construct-overlap.h"
#include "Grids/Distributed/construct-local-overlapping-grids.h"
#include "Grids/Distributed/generic/grid-self-map.h"

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
  
  typedef typename Partition::grid_type           fine_grid_type;
  typedef grid_types<fine_grid_type>              fgt;
  // typedef grid_types<fine_grid_type>::Vertex    Vertex;
  // typedef grid_types<fine_grid_type>::Facet     Facet;
  typedef typename fgt::Cell                      Cell;
  typedef typename fgt::vertex_handle             vertex_handle;

  //------------------ construct coarse grid and correspondencies -----------------

  bijective_mapping<coarse_cell_handle,   int>           CoarseCellCorr;
  bijective_mapping<coarse_vertex_handle, vertex_handle> CoarseVertexCorr;

  ConstructCoarsePartitionGrid(CG.TheCoarseGrid(),Prtng, 			       
			       CoarseVertexCorr,
			       CoarseCellCorr);
  CG.update();

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
    // Ovlp[*P] = overlap_type(CrsG, Prtng.TheGrid());
    Ovlp[*P0].set_coarse_grid(CrsG);
    Ovlp[*P0].set_fine_grid  (Prtng.TheGrid());
  }

  // identity mapping
  GridSelfMap<fine_grid_type> grid_map(Prtng.TheGrid());
  ConstructOverlap(Ovlp,CrsG,Prtng,ovlp_pattern, part2cell, cell2part, grid_map);


  //--------------------- construct local grids  ---------------------------------

  for(CoarseCellIterator P1 = CrsG.FirstCell(); ! P1.IsDone(); ++P1) {
    CG.OvrlpGrid(*P1).set_coarse_grid(CG.TheCoarseGrid());
    ConstructLocalOverlappingGrid(CG.OvrlpGrid(*P1),
				  Prtng.Range(cell2part(*P1)), geom,
				  Ovlp[*P1], v_corr[*P1], c_corr[*P1]);
  }

  //---------------------------- do checkings   ------------------------------------

  check_composite_grid(CG);

}

#endif
