

/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


/*! \file

    Test distributed I/O
 */

#include <iostream>
#include <fstream>

#include <mpi.h>

#include "IO/control-device.h"
#include "Container/bijective-mapping.h"    



#include "Gral/Grids/Complex2D/complex2d.h"
#include "Gral/Grids/Complex2D/stored-geometry.h"
#include "Gral/Grids/Complex2D/boundary.h"
#include "Gral/Grids/Complex2D/construct.h"
#include "Gral/Grids/Complex2D/enlarge.h"
#include "Gral/Grids/Complex2D/grid-functions.h"

#include "Gral/IO/complex2d-format.h"

#include "Gral/Distributed/mpi-distributed-grid.h"
#include "Gral/Distributed/mpi-distributed-grid-function.h"
#include "Gral/Distributed/mpi-distributed-grid-function-adapter.h"
#include "Gral/Distributed/mpi-distributed-grid-traits.h"
#include "Gral/Distributed/composite-grid.h"
#include "Gral/Distributed/construct-composite.h"
#include "Gral/Distributed/copy-composite-to-distributed.h"
#include "Gral/Distributed/overlap-pattern.h"      
#include "Gral/Distributed/overlapping-grid-function-adapter.h"

#include "Gral/Distributed/composite-grid-io.h"
#include "Gral/Distributed/overlapping-grid-io.h"
#include "Gral/Distributed/overlap-io.h"
#include "Gral/Distributed/overlap-range-io.h"


#include "Utility/as-string.h"




int main(int argc, char* argv[]) {
  using namespace GrAL;
  using std::string;
  using std::cerr;
  using std::ifstream;
  using std::ofstream;
  using std::endl;

  MPI_Init(&argc,&argv);
  MPI_Barrier(MPI_COMM_WORLD);
  cerr << "MPI Init finished\n";
  int size;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  cerr << "MPI: " << size << " processes!" << endl;

  typedef Complex2D                      coarse_grid_type;

  typedef Complex2D                      fine_grid_type;
  typedef stored_geometry_complex2D      fine_geom_type;
  typedef Complex2D                      master_grid_type;
  typedef stored_geometry_complex2D      master_geom_type;

  typedef grid_types<fine_grid_type>      fgt;
  typedef grid_types<master_grid_type>    mgt;
  typedef grid_types<coarse_grid_type>    cgt;
 
  string gridfile       = "grid.complex2d";
  string partitionfile  = "grid.part";
  string pattern_string = "VC";         
                                                          
  ControlDevice Ctrl(GetFileControlDevice("distributed.in",""));
  cerr << "Ctrl generated\n";
  Ctrl.update();
  cerr << "Ctrl updated\n";

   //---------- construct master grid --------------
 
  master_grid_type MasterG;
  master_geom_type MasterGeom(MasterG);
  ifstream grd(gridfile.c_str());
  IstreamComplex2DFmt InG(grd);
  // ConstructGrid0(MasterG, InG);
  ConstructGrid(MasterG, MasterGeom, InG, InG);                                                   


  //----- create partitioning ---------
 
  partitioning<master_grid_type> Prtng(MasterG);
  ifstream prt(partitionfile.c_str());
  Prtng.read_partition(prt);
  prt.close();                                                                                    

  //--- generate composite grid ---------------------

  typedef CompositeGrid<coarse_grid_type,fine_grid_type> comp_grid_type;
  typedef grid_types<coarse_grid_type>                   cgt;
  typedef cgt::Cell                                      CoarseCell;


  comp_grid_type  compG;
  typedef bijective_mapping<fgt::vertex_handle, fgt::vertex_handle> vertex_corr_map;
  typedef bijective_mapping<fgt::cell_handle,   fgt::cell_handle>   cell_corr_map;
  grid_function<CoarseCell, vertex_corr_map>    master2part_v;
  grid_function<CoarseCell, cell_corr_map>      master2part_c;
  overlap_pattern                               ovlp_pattern(pattern_string);
 
  ConstructComposite(compG,
                     Prtng,MasterGeom,
                     ovlp_pattern,
                     master2part_v, master2part_c);


  //---- generate distributed grid ------

  typedef MPIDistributedGrid<coarse_grid_type,fine_grid_type> distr_grid_type;
  distr_grid_type MpiG;
  
  cell_morphism<coarse_grid_type, coarse_grid_type> coarse_corr_c(compG.TheCoarseGrid(),
                                                                  MpiG .TheCoarseGrid());
  // it is not so simple here to use vertex/cell_morphism 
  // instead of  bijective_mapping<> ! (Initialization!)
  typedef fgt::vertex_handle fvh_comp;
  typedef fgt::vertex_handle fvh_mpi;
  typedef fgt::cell_handle   fch_comp;
  typedef fgt::cell_handle   fch_mpi;
  grid_function<CoarseCell, bijective_mapping<fvh_comp,fvh_mpi> > fine_corr_v(compG.TheCoarseGrid());
  grid_function<CoarseCell, bijective_mapping<fch_comp,fch_mpi> > fine_corr_c(compG.TheCoarseGrid());
  
  CopyComposite2Distributed(MpiG,compG, coarse_corr_c, fine_corr_v, fine_corr_c);


  
  typedef fine_geom_type   distr_geom_type;
  distr_geom_type MpiGeom(MpiG.TheGrid());

  CoarseCell C = coarse_corr_c.inverse()(MpiG.MyCell());
  for(fgt::VertexIterator v(compG.Grid(C)); ! v.IsDone(); ++v)
    MpiGeom       .coord(MpiG.TheGrid().vertex(fine_corr_v  (C)(v.handle())))  
      = MasterGeom.coord(MasterG       .vertex(master2part_v(C).inverse()(v.handle())));
  

  //------  output grid & geometry  ----------------

  {
    OstreamComplex2DFmt Out("mpigrid" + as_string(MpiG.MyRank()) + ".complex2d");
    ConstructGrid(Out,Out,MpiG.TheGrid(), MpiGeom);
  }

  //--------- test I/O to files    -----------------

  distr_grid_type MpiG2;
  
  {
    
    typedef IstreamComplex2DCombFmt coarse_grid_input_t;
    typedef IstreamComplex2DCombFmt fine_grid_input_t;
    typedef  overlap_input<
      coarse_grid_type, 
      fine_grid_input_t, 
      overlap_range_input>  overlap_input_t;
    typedef overlapping_grid_input<
      coarse_grid_type, 
      fine_grid_input_t,
      overlap_input_t> overlapping_grid_input_t;
    
    typedef composite_grid_input<
      coarse_grid_input_t,
      coarse_grid_type,
      overlapping_grid_input_t>  composite_input_t;
    
    composite_input_t compIn("composite-input/compgrid");
    comp_grid_type compG2;
    cell_morphism<composite_input_t::coarse_grid_type, coarse_grid_type> 
      coarse_corr_c_in(compIn.TheCoarseGrid(),
                       MpiG2.TheCoarseGrid());
    // it is not so simple here to use vertex/cell_morphism 
    // instead of  bijective_mapping<> ! (Initialization!)
    grid_function<CoarseCell, bijective_mapping<int,int> > fine_corr_v_in(compIn.TheCoarseGrid());
    grid_function<CoarseCell, bijective_mapping<int,int> > fine_corr_c_in(compIn.TheCoarseGrid());

    typedef fine_grid_input_t::vertex_handle fvh_comp;
    typedef fine_grid_input_t::cell_handle   fch_comp;
    typedef fgt::vertex_handle fvh_mpi;
    typedef fgt::cell_handle   fch_mpi;
    grid_function<CoarseCell, bijective_mapping<fvh_comp,fvh_mpi> > fine_corr_v(compG.TheCoarseGrid());
    grid_function<CoarseCell, bijective_mapping<fch_comp,fch_mpi> > fine_corr_c(compG.TheCoarseGrid());
  
    CopyComposite2Distributed(MpiG2,compIn,  coarse_corr_c, fine_corr_v, fine_corr_c);



    typedef OstreamComplex2DFmt coarse_grid_output_t;
    typedef OstreamComplex2DFmt fine_grid_output_t;
    typedef  overlap_output<
      coarse_grid_output_t, 
      fine_grid_output_t, 
      overlap_range_output> overlap_output_t;
    typedef overlapping_grid_output<
      coarse_grid_output_t, 
      fine_grid_output_t,
      overlap_output_t> overlapping_grid_output_t;
    

    coarse_grid_output_t CoarseOut("distributed-output/dg." + as_string(MpiG2.MyRank()) + ".coarse");
    cell_morphism  <coarse_grid_type, coarse_grid_output_t> cell_corr(MpiG2.TheCoarseGrid(),
								      CoarseOut);
    vertex_morphism<coarse_grid_type, coarse_grid_output_t> vtx_corr (MpiG2.TheCoarseGrid(),
								      CoarseOut);
    ConstructGrid0(CoarseOut,MpiG2.TheCoarseGrid(), vtx_corr, cell_corr);

    overlapping_grid_output_t OvlpOut(CoarseOut,
				      "distributed-output/dg." + as_string(MpiG2.MyRank()));
    CopyOverlappingGrid(OvlpOut,  MpiG2.TheOvrlpGrid(), cell_corr);
  }


  //------------------ finalize MPI ----------------------

  MPI_Barrier(MPI_COMM_WORLD);
  MPI_Finalize();

}
