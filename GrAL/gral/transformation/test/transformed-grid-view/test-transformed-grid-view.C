
/*! \file
 */

/* ------------------------------------------------------------

    Copyright (C) 2005 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include "Gral/Transformation/transformed-grid-view.h"
#include "Gral/Transformation/trivial-transformation-pattern.h"


#include "Gral/Grids/Triang2D/all.h"
#include "Gral/IO/complex2d-format.h"
#include "Gral/IO/gmv-format-output2d.h"

namespace GrAL {
  typedef Triang2D                                  grid_type;
  typedef stored_geometry_triang2d                  geom_type;
  typedef trivial_transformation_pattern<grid_type> pattern_type;
  typedef transformed_grid_view::cfg<grid_type, geom_type, pattern_type>  tgv_cfg;

  namespace   transformed_grid_view {
    template class grid_view<tgv_cfg>;
    template class vertex_handle_t<tgv_cfg>;
    template class vertex_t<tgv_cfg>;
    template class vertex_iterator_t<tgv_cfg>;
    template class cell_handle_t<tgv_cfg>;
    template class cell_iterator_t<tgv_cfg>;
    template class vertex_on_cell_iterator_t<tgv_cfg>;
  }
}


int main() {
  using namespace GrAL;

  typedef Triang2D                  grid_type;
  typedef stored_geometry_triang2d  geom_type;
  typedef trivial_transformation_pattern<grid_type> pattern_type;
  typedef double ct[2];

  int conn  [] = { 0,1,2};
  ct  coords[] = { {0.0, 0.0}, {1.0, 0.0}, {0.0,1.0}};
  
  
  int nc = sizeof(conn)/(3*sizeof(int));
  int nv = sizeof(coords)/sizeof(ct);
  grid_type T((int *)conn, nc, nv);
  geom_type Geom(T, (double *)coords);
  
  namespace tgv = transformed_grid_view;
  typedef transformed_grid_view::cfg<grid_type, geom_type, pattern_type>  tgv_cfg;

  {
    tgv::grid_view<tgv_cfg> V(T, Geom);
    V.transform();
      
    REQUIRE_ALWAYS(V.NumOfCells()    == T.NumOfCells(),    "", 1);
    REQUIRE_ALWAYS(V.NumOfVertices() == T.NumOfVertices(), "", 1);
  }   

  {
    tgv::grid_view<tgv_cfg> V(T, Geom);
    V.transform_cell(* T.FirstCell());
    V.transform();
      
    typedef grid_types<tgv::grid_view<tgv_cfg> > gt;
    REQUIRE_ALWAYS(V.NumOfCells()    == T.NumOfCells(),    "", 1);
    REQUIRE_ALWAYS(V.NumOfVertices() == T.NumOfVertices(), "", 1);

    int v_cnt = 0;
    for(gt::VertexIterator v(V); !v.IsDone(); ++v)
      ++v_cnt;
    REQUIRE_ALWAYS(v_cnt == V.NumOfVertices(), "", 1);

    int c_cnt = 0;
    for(gt::CellIterator c(V); !c.IsDone(); ++c) {
      ++c_cnt;
      int vc_cnt = 0;
      for(gt::VertexOnCellIterator vc(*c); !vc.IsDone(); ++vc)
	++vc_cnt;
      REQUIRE_ALWAYS(vc_cnt == (*c).NumOfVertices(), "", 1);
    }
    REQUIRE_ALWAYS(c_cnt == V.NumOfCells(), "", 1);

    OstreamComplex2DFmt Out("t1.complex2d");
    ConstructGrid(Out, Out, V, V);

    OstreamGMV2DFmt GMV("t1.gmv");
    ConstructGrid(GMV, V, V);
  }


}
