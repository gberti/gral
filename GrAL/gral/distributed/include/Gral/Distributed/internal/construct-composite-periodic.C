#ifndef NMWR_GB_CONSTRUCT_COMPOSITE_PERIODIC_C
#define NMWR_GB_CONSTRUCT_COMPOSITE_PERIODIC_C



/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */



#include "Gral/Distributed/construct-composite-periodic.h"

#include "Container/bijective-mapping.h"

#include "Gral/Geometries/transformed-geometry.h"
#include "Gral/Partitioning/coarse-grid-from-partitioning.h"
#include "Gral/Base/enlarge-grid.h"
#include "Gral/Base/construct-grid.h"

#include "Gral/Distributed/dyn-overlap.h"
#include "Gral/Distributed/construct-overlap.h"
#include "Gral/Distributed/construct-local-overlapping-grids.h"
#include "Gral/Distributed/internal/grid-self-map.h"

namespace GrAL {

// ad-hoc mapping for partition numbers
struct map_unique {
  typedef int argument_type;
  typedef int result_type;

  // this is self-inverse, and -1 -> -1
  int operator()(int i) const { return (-i-2);}
};


template<class CompositeG, class Partition, class Geometry, class VCorrLoc,
         class OVPattern, class VCorr, class CCorr, class Transform>
void ConstructComposite_per(CompositeG     & CG,
			    Partition      & Prtng,
			    Geometry  const& geom, 
			    OVPattern const& ovlp_pattern,
			    VCorrLoc  const& id_per_v1, // vertices identified due to periodicity
			    VCorrLoc  const& id_per_v2, // vertices identified due to periodicity
			    Transform const& T1, // V1 -> V2
			    Transform const& T2, // V2 -> V1 
			    VCorr          & v_corr,
			    CCorr          & c_corr)
{

  typedef typename  CompositeG::coarse_grid_type  coarse_grid_type;
  typedef grid_types<coarse_grid_type>            cgt;
  typedef typename cgt::Cell                      CoarseCell;
  typedef typename cgt::CellIterator              CoarseCellIterator;
  typedef typename cgt::vertex_handle             coarse_vertex_handle;
  typedef typename cgt::cell_handle               coarse_cell_handle;
  
  typedef typename CompositeG::fine_grid_type     fine_grid_type;
  typedef grid_types<fine_grid_type>              fgt;
  // typedef grid_types<fine_grid_type>::Vertex    Vertex;
  // typedef grid_types<fine_grid_type>::Facet     Facet;
  typedef typename fgt::Cell                      Cell;
  typedef typename fgt::vertex_handle             vertex_handle;
  typedef typename fgt::cell_handle               cell_handle;


  typedef typename Partition::grid_type           master_grid_type;
  typedef Geometry                                master_geom_type;

  //------------------ construct coarse grid and correspondencies -----------------

  bijective_mapping<coarse_cell_handle,   int>           CoarseCellCorr;
  bijective_mapping<coarse_vertex_handle, vertex_handle> CoarseVertexCorr;

  ConstructCoarsePartitionGrid(CG.TheCoarseGrid(),Prtng, 			       
			       CoarseVertexCorr,
			       CoarseCellCorr);
  CG.coarse_grid_complete();

  v_corr.set_grid(CG.TheCoarseGrid());
  c_corr.set_grid(CG.TheCoarseGrid());

  //-----------  enlarge grid by periodic overlap ----------------

  typedef bijective_mapping<cell_handle,cell_handle>     per_corr_c_type;
  typedef bijective_mapping<vertex_handle,vertex_handle> per_corr_v_type;
  typedef typename  per_corr_v_type::domain_type pv_domain;
  typedef typename  pv_domain::const_iterator    pviter;
  typedef typename  per_corr_c_type::domain_type pc_domain;
  typedef typename  pc_domain::const_iterator    pciter;

  master_grid_type & G(Prtng.TheGrid());
  // get the 2 overlapping subranges

  enumerated_vertex_range<fine_grid_type> shared_v_1(G);
  for(pviter v = id_per_v1.domain().begin(); v != id_per_v1.domain().end(); ++v)
    shared_v_1.push_back(*v);
  enumerated_vertex_range<fine_grid_type> shared_v_2(G);
  for(pviter vv = id_per_v2.domain().begin(); vv != id_per_v2.domain().end(); ++vv)
    shared_v_2.push_back(*vv);
  
  enumerated_cell_range<fine_grid_type> per_ovlp_cells_1(G);
  mark_layers(FacetSpanOfVertices(shared_v_1), //Vertices(id_per_v1.domain()),G ),
	      per_ovlp_cells_1, 
	      ovlp_pattern, IsCellInside(G));
  enumerated_cell_range<fine_grid_type> per_ovlp_cells_2(G);
  mark_layers(FacetSpanOfVertices(shared_v_2), //Vertices(id_per_v2.domain()),G), 
	      per_ovlp_cells_2, 
	      ovlp_pattern, IsCellInside(G));

  enumerated_subrange<fine_grid_type> per_ovlp1(G);
  ConstructSubrangeFromCells(per_ovlp1,per_ovlp_cells_1.FirstCell());

  enumerated_subrange<fine_grid_type> per_ovlp2(G);
  ConstructSubrangeFromCells(per_ovlp2,per_ovlp_cells_2.FirstCell());

  // enlarge grid by periodic geometric mapping of these subranges


  per_corr_c_type c_corr_per;
  per_corr_v_type v_corr_per;

  EnlargeGridVC(G,
		per_ovlp1,
		TransformGeom(geom,T1),
		id_per_v1,
		v_corr_per,
		c_corr_per);
  EnlargeGridVC(G,
		per_ovlp2,
		TransformGeom(geom,T2),
		id_per_v2,
		v_corr_per,
		c_corr_per);

  // should work automatically
  Prtng.update();

  /*
  ofstream oogl_output;
  oogl_output.open("construct-overlap.oogl");
  GraphicsDevice Dev = OOGLDevice(oogl_output);
  Dev << BeginGroup("m-mesh")       << ViewGrid(G,geom)                    << EndGroup
      << BeginGroup("m-vertex-hdl") << ViewGFAsString(vertex2handle(G), geom) << EndGroup
      << BeginGroup("m-cell-hdl")   << ViewGFAsString(cell2handle(G),   geom) << EndGroup;
  oogl_output.close();
  */

  // identify added ranges with their source
  GridSelfMap<fine_grid_type> grid_map(G);

  for(pviter v0 = v_corr_per.domain().begin(); v0  != v_corr_per.domain().end(); ++v0)
    grid_map.TheVertexMap()[v_corr_per(*v0)] = *v0;

  for(pciter c = c_corr_per.domain().begin(); c  != c_corr_per.domain().end(); ++c)
    grid_map.TheCellMap()[c_corr_per(*c)] = *c;


  // attach temporal partitions to added grid ranges -- partitions
  // are in principle inherited from src ranges, but are assigned a new
  // partition number to avoid conflicts if partitions are self-neighboured
  // under periodic wrapping.
  map_unique map_partition;
  typedef typename enumerated_cell_range<fine_grid_type>::ElementIterator RgeCellIterator;
  for(RgeCellIterator C = per_ovlp_cells_1.FirstCell(); ! C.IsDone(); ++C)
    Prtng.set_partition(G.cell(c_corr_per(C.handle())), map_partition(Prtng.partition(*C)));
  for(RgeCellIterator CC = per_ovlp_cells_2.FirstCell(); ! CC.IsDone(); ++CC)
    Prtng.set_partition(G.cell(c_corr_per(CC.handle())), map_partition(Prtng.partition(*CC)));



  coarse_grid_type& CrsG(CG.TheCoarseGrid());

  grid_function<CoarseCell,int> cell2part(CG.TheCoarseGrid());
  bijective_mapping<int, CoarseCell> part2cell;
  for(CoarseCellIterator P = CrsG.FirstCell(); ! P.IsDone(); ++P) {
    cell2part[*P] = CoarseCellCorr(P.handle());
    part2cell[cell2part(*P)] = *P;
    part2cell[map_partition(cell2part(*P))] = *P;  // "virtual" partition
  }


  //--------------------- construct overlap structures  -------------------------

  // overlap tied to the global grid
  typedef dyn_overlap<coarse_grid_type, fine_grid_type> overlap_type;
  grid_function<CoarseCell, overlap_type>               Ovlp(CrsG);
  for(CoarseCellIterator PP = CrsG.FirstCell(); ! PP.IsDone(); ++PP) {
    Ovlp[*PP].init(CrsG,Prtng.TheGrid());
  }

  ConstructOverlap(Ovlp,CrsG,Prtng,ovlp_pattern, part2cell, cell2part, grid_map);

  /*
  for(CoarseCellIterator P0 = CrsG.FirstCell(); ! P0.IsDone(); ++P0) {
    cerr << "Overlap " << CrsG.handle(*P0) << '\n'
	 << "------------------------------------------\n";
    write_ovlp(Ovlp[*P0],cerr);
    cerr << "------------------------------------------\n";
  }
  */

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
