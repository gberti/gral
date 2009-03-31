#ifndef NMWR_GB_CONSTRUCT_MPI_DISTRIBUTED_C
#define NMWR_GB_CONSTRUCT_MPI_DISTRIBUTED_C



/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */



#include <vector>



#include "Gral/Algorithms/grid-copy.h"
#include "Gral/Base/type-tags.h"
#include "Gral/Partitioning/partitioning.h"

#include "Gral/Distributed/construct-composite-periodic.h"

#include "Gral/Distributed/composite-grid.h"
#include "Gral/Distributed/composite-grid-function.h"
#include "Gral/Distributed/overlap-pattern.h"

namespace GrAL {

template<class CG, class FG, class FGEOM, class MG, class GEOM, 
         class VCORRLOC, class TRAFO, class VCORR, class CCORR>
void
ConstructMPIDistributedFromMaster
(
 MPIDistributedGrid<CG,FG>          & DistrG,          // OUT: to be constructed
 FGEOM                              & DistrGeom,       // [OUT] to date, implicitely constructed
 partitioning<MG>                   & Prtng,           // IN:  contains partitioning & master
 GEOM                         const & MasterGeom,      // IN:  geometry for master grid
 overlap_pattern              const & ovlp_pattern,    // IN:  logical dscr. of overlap
 VCORRLOC                           & per_v_1,         // IN:  mapping of periodic bd 
                                                       //  (optional, may be empty)
 VCORRLOC                           & per_v_2,         //  inverse of per_v_1
 TRAFO                        const & T1,              // IN: geometric transf. corr. to per_v_1
 TRAFO                        const & T2,              //  inverse of T1
 VCORR                              & master2distr_v,  //  map vertices of master to those of DistrG
 CCORR                              & master2distr_c   //  same for cells
)
{
  typedef grid_types<CG>                    cgt;
  typedef typename cgt::Cell                CoarseCell;
  typedef typename cgt::CellIterator        CoarseCellIterator;

  typedef grid_types<FG>                    fgt;
  typedef typename fgt::cell_handle         cell_handle;
  typedef typename fgt::vertex_handle       vertex_handle;

  typedef FGEOM                             geometry_type;

  //----------- (1) construct composite grid for ALL partitions -------------

  // it would be enough to construct the local part & neighbor parts
  // of the composite grid!
  CompositeGrid<CG,FG>     compG;
  typedef bijective_mapping<vertex_handle,vertex_handle> vertex_corr_map;
  typedef bijective_mapping<cell_handle,  cell_handle>   cell_corr_map;

  grid_function<CoarseCell, vertex_corr_map> master2part_v;
  grid_function<CoarseCell, cell_corr_map>   master2part_c;
  ConstructComposite_per(compG,Prtng,MasterGeom,ovlp_pattern,
			 per_v_1,per_v_2, T1, T2,
			 master2part_v, master2part_c);

  // composite geometry -- should be constructed in  ConstructComposite_per above.
  grid_function<CoarseCell,geometry_type> compGeom(compG.TheCoarseGrid());
  for(CoarseCellIterator CC = compG.TheCoarseGrid().FirstCell(); ! CC.IsDone(); ++CC) 
    compGeom[*CC] = geometry_type(compG.Grid(*CC));

  //---------- (2) copy the appropriate local part to the distributed grid --------

  DistrG.set_coarse_grid(compG.TheCoarseGrid());

  cell_corr_map distr2coarse;
  ConstructCellCorrespondence(DistrG.TheCoarseGrid(),
			      compG.TheCoarseGrid(),
			      distr2coarse);
  
  CoarseCell mMyC = DistrG.MyCell();
  CoarseCell cMyC = compG.TheCoarseGrid().cell(distr2coarse(mMyC.handle()));

  cell_corr_map   part2distr_c;
  vertex_corr_map part2distr_v;

  // this should be done outside,
  // using the master2distr mappings.
  DistrGeom.set_grid(DistrG.TheGrid());
  
  // Use CopyOverlappingGrid for the stuff below??
  ConstructGridVC(DistrG.TheGrid(),  DistrGeom,
		   compG.Grid(cMyC), compGeom(cMyC),
		   part2distr_v, part2distr_c);

  DistrG.TheOvrlpGrid().fine_grid_complete();

  inverse_mapping<cell_handle,   cell_handle> coarse2distr(distr2coarse.inverse());
  CopyOverlap(DistrG.TheOverlap(),  compG.Overlap(cMyC),
	      DistrG.TheCoarseGrid(), coarse2distr,
	      DistrG.TheGrid(), 		   
	      part2distr_v, part2distr_c);

  // chain correspondance maps
  // master2distr_v = part2distr_v o master2part_v
  //  mapping_assign(master2distr_v, compose_map(make_unary_fct_ref(part2distr_v),
  //				     make_unary_fct_ref(master2part_v)));
  typedef typename vertex_corr_map::domain_type dom_type_v;
  typedef typename dom_type_v::const_iterator dom_iter_v;
  for(dom_iter_v v = master2part_v[cMyC].domain().begin(); v != master2part_v[cMyC].domain().end(); ++v)
    master2distr_v[*v] = part2distr_v(master2part_v[cMyC](*v));

  //  mapping_assign(master2distr_c, compose_map(make_unary_fct_ref(part2distr_c),
  //					     make_unary_fct_ref(master2part_c)));
  typedef typename cell_corr_map::domain_type dom_type_c;
  typedef typename dom_type_c::const_iterator dom_iter_c;
  for(dom_iter_c c = master2part_c[cMyC].domain().begin(); c != master2part_c[cMyC].domain().end(); ++c)
    master2distr_c[*c] = part2distr_c(master2part_c[cMyC](*c));


}

} // namespace GrAL 

#endif
