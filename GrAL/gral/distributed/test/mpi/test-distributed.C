
// $LICENSE

/*! \file
 */


#include <iostream>
#include <fstream>
#include <vector>

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
//#include "Gral/Geometries/simple-geometry.h" 

#include "Gral/Distributed/mpi-distributed-grid.h"
#include "Gral/Distributed/mpi-distributed-grid-function.h"
#include "Gral/Distributed/mpi-distributed-grid-function-adapter.h"
#include "Gral/Distributed/mpi-distributed-grid-traits.h"
#include "Gral/Distributed/construct-mpi-distributed.h"
#include "Gral/Distributed/overlap-pattern.h"      
#include "Gral/Distributed/overlapping-grid-function-adapter.h"

#include "Utility/as-string.h"




int main(int argc, char* argv[]) {
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
  //  typedef simple_geometry<fine_grid_type,coordN<2> >  fine_geom_type;
  typedef Complex2D                      master_grid_type;
  typedef stored_geometry_complex2D      master_geom_type;
  //typedef simple_geometry<fine_grid_type,coordN<2> >  master_geom_type;


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

  //---- generate distributed grid ------
  typedef MPIDistributedGrid<coarse_grid_type,fine_grid_type> distr_grid_type;
  typedef fine_geom_type                                      distr_geom_type;
  distr_grid_type MpiG;
  distr_geom_type MpiGeom;
  bijective_mapping<fgt::vertex_handle, fgt::vertex_handle>  master2distr_v;
  bijective_mapping<fgt::cell_handle,   fgt::cell_handle>    master2distr_c;
  overlap_pattern ovlp_pattern(pattern_string);   
  ConstructMPIDistributedFromMaster(MpiG,
				    MpiGeom,
				    Prtng, // includes MasterG
				    MasterGeom,
				    ovlp_pattern,
				    master2distr_v,
				    master2distr_c);

  // create MpiGeom. FIXME: not done by above call, in spite of args.
  // inefficient, should loop only over v with master2distr_v.defined(v).
  for(mgt::VertexIterator v(MasterG); ! v.IsDone(); ++v)
    if(master2distr_v.defined(v.handle())) {
      fgt::Vertex lv(MpiG.TheGrid(), master2distr_v(v.handle()));
      MpiGeom.coord(lv) = MasterGeom.coord(*v);
    }

  //------ perform checks ----------------

  if(MpiG.isMaster()) {
    cerr << "Master: \n";
    ofstream gout("grid.master-local.complex2d");  
    write_complex2d(MpiG.TheGrid(), MpiGeom, gout);
    gout.close();
    cerr << "\n"
	 <<"Local Range: "
	 << MpiG.LocalRange().NumOfVertices() << " vertices, "
	 << MpiG.LocalRange().NumOfCells()    << " cells\n";
    cerr << "--------------\n";
  }

  typedef fgt::Cell   Cell;
  typedef fgt::Vertex Vertex;
  typedef distr_grid_type::range_type  d_range_type;
  typedef d_range_type::CellIterator   RgeCellIterator;
  typedef d_range_type::VertexIterator RgeVertexIterator;

  // test distributed grid functions
  distr_grid_function<Cell,  int,distr_grid_type> dgfc(MpiG,-77);
  distr_grid_function<Vertex,int,distr_grid_type> dgfv(MpiG,-77);

  if(MpiG.isMaster()) {
    cerr << " setting dgfc\n";
    for(RgeCellIterator C = MpiG.LocalRange().FirstCell(); ! C.IsDone(); ++C)
      cerr <<  (dgfc[*C] = C.handle()) << '\n';
    cerr << '\n';
  }
  else
    for(RgeCellIterator C = MpiG.LocalRange().FirstCell(); ! C.IsDone(); ++C)
      dgfc[*C] = (*C).handle();

  for(RgeVertexIterator V = MpiG.LocalRange().FirstVertex(); ! V.IsDone(); ++V)
    dgfv[*V] = (*V).handle();

  // check copy constructor, assigment, vector<dgf>
  distr_grid_function<Cell,  int,distr_grid_type> dgfc_copy(dgfc);
  distr_grid_function<Vertex,int,distr_grid_type> dgfv_copy(dgfv);
  for(RgeCellIterator   C = MpiG.LocalRange().FirstCell();   ! C.IsDone(); ++C) {
    REQUIRE_ALWAYS( (dgfc(*C) == dgfc_copy(*C)), "copy not identical!\n",1);
  }
  for(RgeVertexIterator V = MpiG.LocalRange().FirstVertex(); ! V.IsDone(); ++V) {
    REQUIRE_ALWAYS( (dgfv(*V) == dgfv_copy(*V)), "copy not identical!\n",1);
  }
  distr_grid_function<Cell,  int,distr_grid_type> dgfc_copy2;
  dgfc_copy2 = dgfc;
  distr_grid_function<Vertex,int,distr_grid_type> dgfv_copy2;
  dgfv_copy2 = dgfv;
  std::vector<distr_grid_function<Cell,  int,distr_grid_type> > dgfc_copy_vec(2);
  std::vector<distr_grid_function<Vertex,int,distr_grid_type> > dgfv_copy_vec(2);
  for(int i = 0; i < 2; ++i) {
    dgfc_copy_vec[i] = dgfc;
    dgfv_copy_vec[i] = dgfv;
  }

  for(RgeCellIterator   C = MpiG.LocalRange().FirstCell();   ! C.IsDone(); ++C) {
    REQUIRE_ALWAYS( (dgfc(*C) == dgfc_copy2      (*C)), "copy not identical!\n",1);
    REQUIRE_ALWAYS( (dgfc(*C) == dgfc_copy_vec[0](*C)), "copy not identical!\n",1);
    REQUIRE_ALWAYS( (dgfc(*C) == dgfc_copy_vec[1](*C)), "copy not identical!\n",1);
  }
  for(RgeVertexIterator V = MpiG.LocalRange().FirstVertex(); ! V.IsDone(); ++V) {
    REQUIRE_ALWAYS( (dgfv(*V) == dgfv_copy2      (*V)), "copy not identical!\n",1);
    REQUIRE_ALWAYS( (dgfv(*V) == dgfv_copy_vec[0](*V)), "copy not identical!\n",1);
    REQUIRE_ALWAYS( (dgfv(*V) == dgfv_copy_vec[1](*V)), "copy not identical!\n",1);
  }



  // put # adj local cells on dgfv2
  distr_grid_function<Vertex,int,distr_grid_type> dgfv2(MpiG,0);
  dgfv2.set_added_on_shared();




  for(RgeCellIterator C = MpiG.LocalRange().FirstCell(); ! C.IsDone(); ++C)
    for(fgt::VertexOnCellIterator VC(*C); ! VC.IsDone(); ++VC) {
      dgfv2[*VC]  += 1;
    }
  
  // after synchronization,  dgfv2 should equal the valence of v on each vertex v.

  dgfv2.synchronize();

  ofstream gfout(("dgf." + as_string(MpiG.MyRank())).c_str());
  for(fgt::VertexIterator v = MpiG.TheGrid().FirstVertex(); !v.IsDone(); ++v) {
    gfout << v.handle() << " -> " << dgfv2[*v] << '\n';
  }
  gfout.close();
  
  ofstream gridout(("grid." + as_string(MpiG.MyRank()) + ".out").c_str());
  OstreamComplex2DFmt GOut(gridout);
  ConstructGrid(GOut, GOut, MpiG.TheGrid(), MpiGeom);
  gridout.close();
  

  //------------------ finalize MPI ----------------------

  MPI_Barrier(MPI_COMM_WORLD);
  MPI_Finalize();

}
