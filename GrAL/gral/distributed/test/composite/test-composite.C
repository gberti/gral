
// $LICENSE

#include <fstream.h>

#include "Container/bijective-mapping.h"

#include "Gral/Distributed/construct-composite.h"
#include "Gral/Distributed/overlap-pattern.h"
#include "Gral/Distributed/composite-grid.h"
#include "Gral/Distributed/composite-grid-function.h"

#include "Gral/IO/istream-grid-complex2d-fmt.h"

#include "Gral/Grids/Complex2D/complex2d.h"
#include "Gral/Grids/Complex2D/boundary.h"
#include "Gral/Grids/Complex2D/stored-geometry.h"
#include "Gral/Grids/Complex2D/construct.h"
#include "Gral/Grids/Complex2D/enlarge.h"
#include "Gral/Grids/Complex2D/grid-functions.h"
#include "Gral/Grids/Complex2D/partial-grid-functions.h"


/*! \file 

  Tests:
    - 2x2p1: OK
    - 2x2  : OK
    - 4x1  : Wrong (First grid to small, nodes 2 <-> 3)
    - 4x1p1: Wrong (nodes 2 <-> 3)
 */


int main(int argc, char* argv[]) {

  typedef Complex2D                 fine_grid_type;
  typedef Complex2D                 coarse_grid_type;
  typedef stored_geometry_complex2D fine_geom_type;
  typedef grid_types<fine_grid_type> fgt;
  
  string gridfile       = "grid.complex2d";
  string partitionfile  = "grid.part";
  string pattern_string = "VC";

  //---------- construct master grid --------------

  fine_grid_type MasterG;
  fine_geom_type MasterGeom(MasterG);
  ifstream grd(gridfile.c_str());
  IstreamComplex2DFmt InG(grd);
  ConstructGrid(MasterG, MasterGeom, InG, InG);

  //----- create partitioning ---------

  partitioning<fine_grid_type> Prtng(MasterG);
  ifstream prt(partitionfile.c_str());
  Prtng.read_partition(prt);
  prt.close();


  //--- create composite grid -----

  typedef CompositeGrid<coarse_grid_type,fine_grid_type> comp_grid_type;
  typedef grid_types<coarse_grid_type>                   cgt;
  typedef cgt::Cell         CoarseCell;

  comp_grid_type  compG;
  typedef bijective_mapping<fgt::vertex_handle, fgt::vertex_handle> vertex_corr_map;
  typedef bijective_mapping<fgt::cell_handle,  fgt::cell_handle>   cell_corr_map;
  grid_function<CoarseCell, vertex_corr_map> master2part_v;
  grid_function<CoarseCell, cell_corr_map>   master2part_c;
  overlap_pattern ovlp_pattern(pattern_string);
 
  ConstructComposite(compG,
                     Prtng,MasterGeom,
                     ovlp_pattern,
                     master2part_v, master2part_c);


  grid_function<cgt::Cell,fine_geom_type> compGeom(compG.TheCoarseGrid());

  OstreamComplex2DFmt OG(cout);
  ConstructGrid0(OG,compG.TheCoarseGrid());
  cout << "master -> part (vertices):\n";
  for(cgt::CellIterator C(compG.TheCoarseGrid()); !C.IsDone(); ++C) {
    cout << "part " << C.handle() << '\n'; 
    for(fgt::VertexIterator v(MasterG); !v.IsDone(); ++v) {
      if(master2part_v(*C).defined(v.handle()))
        cout << v.handle() << " -> " << master2part_v(*C)(v.handle()) << '\n';
    }
  }
  cout << '\n';
  for(cgt::CellIterator C(compG.TheCoarseGrid()); !C.IsDone(); ++C) {
    fine_grid_type const& G_C = compG.Grid(*C);
    compGeom[*C].set_grid(G_C);
    for(fgt::VertexIterator v(MasterG); !v.IsDone(); ++v) {
      if(master2part_v(*C).defined(v.handle()))
        compGeom[*C].coord(G_C.vertex(master2part_v(*C)(v.handle())))
          = MasterGeom.coord(*v);
    }
    OstreamComplex2DFmt OG_C(cout);
    ConstructGrid(OG_C, compG.Grid(*C), compGeom(*C));
    cout << '\n';
 }

}
