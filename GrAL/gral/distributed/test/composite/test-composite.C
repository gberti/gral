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




int main(int argc, char* argv[]) {

  typedef Complex2D                 fine_grid_type;
  typedef Complex2D                 coarse_grid_type;
  typedef stored_geometry_complex2D fine_geom_type;
  typedef grid_types<fine_grid_type> fgt;
  
  string gridfile       = "grid.complex2d";
  string partitionfile  = "grid.part.2";
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
}
