/*! \file

   \todo Mixing full refinement and trivial refinement does not yet work.
*/

/* ------------------------------------------------------------

    Copyright (C) 2005 - 2009 Guntram Berti,
    Contact: guntram AT users DOT berlios DOT de

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include "Gral/Refinement/barycentric-subdivision-transformation-pattern.h"
#include "Gral/Transformation/transformed-grid-view.h"

#include "Gral/Grids/Triang2D/all.h"
#include "Gral/IO/gmv-format-output2d.h"
#include "Gral/IO/complex2d-format.h"

#include "Utility/as-string.h"

#include <cmath>
#include <vector>

namespace GrAL {

  typedef Triang2D                 grid_type;
  typedef Triang2D                 pattern_grid_type;
  typedef stored_geometry_triang2d pattern_geom_type;
  typedef transformation_pattern_config<grid_type, pattern_grid_type, pattern_geom_type> pcfg;

  template class barycentric_subdivision_transformation_pattern<pcfg>;

}

int main() 
{
  using namespace GrAL;
  using namespace std;

  typedef Triang2D                  grid_type;
  typedef stored_geometry_triang2d  geom_type;
  typedef grid_types<grid_type>     gt;
  typedef Triang2D                 pattern_grid_type;
  typedef stored_geometry_triang2d pattern_geom_type;
  typedef transformation_pattern_config<grid_type, pattern_grid_type, pattern_geom_type> pcfg;
  typedef barycentric_subdivision_transformation_pattern<pcfg>        pattern_type;
  typedef double ct[2];

  int conn  [] = { 0,1,2};
  ct  coords[] = { {0.0, 0.0}, {1.0, 0.0}, {0.5,sqrt(3.0)/2.0}};
  int nc = sizeof(conn)/(3*sizeof(int));
  int nv = sizeof(coords)/sizeof(ct);
  grid_type T((int *)conn, nc, nv);
  geom_type GeomT(T, (double *)coords);

  namespace tgv = transformed_grid_view;
  typedef transformed_grid_view::cfg<grid_type, geom_type, pattern_type>  tgv_cfg;

  {  
    tgv::grid_view<tgv_cfg>  V(T, GeomT);
    V.transform_cell(*T.FirstCell());
    V.transform();
    
    REQUIRE_ALWAYS(V.NumOfVertices() == 7, "nv=" << V.NumOfVertices(), 1);
    REQUIRE_ALWAYS(V.NumOfCells()    == 6, "nc=" << V.NumOfCells(), 1);
  }
  

  unsigned N = 5;

  vector<grid_type> Tv(N);
  vector<geom_type> GeomTv(N);

  IstreamComplex2DFmt In("triang.complex2d");
  ConstructGrid(Tv[0], GeomTv[0], In, In);

  for(int i = 1; i < N; ++i) {
    cout << "Refinement step " << i << ": " << flush;
    tgv::grid_view<tgv_cfg> V(Tv[i-1], GeomTv[i-1]);
    for(gt::CellIterator c(Tv[i-1]); !c.IsDone(); ++c)
      V.transform_cell(*c);
    V.transform();

    cout << V.NumOfCells() << " cells, " << V.NumOfVertices() << " vertices." << endl;
    ConstructGrid(Tv[i], GeomTv[i], V, V);
    
    OstreamGMV2DFmt GMV("t" + as_string(i) + ".gmv");
    ConstructGrid(GMV, Tv[i], GeomTv[i]);

    OstreamComplex2DFmt Out("t" + as_string(i)+ ".complex2d");
    ConstructGrid(Out, Out, Tv[i], GeomTv[i]);

  }
}
