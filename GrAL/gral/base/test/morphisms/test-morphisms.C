/*! \file */

#include "Gral/Base/grid-morphism.h"

#include "Gral/Grids/Complex2D/all.h"
#include "Gral/Grids/CartesianND/all.h"

namespace GrAL {
  typedef cartesiannd::grid<2> def_grid_type;
  typedef Complex2D            img_grid_type;
  
  template class vertex_morphism<def_grid_type, img_grid_type>;
  template class cell_morphism  <def_grid_type, img_grid_type>;
}


int main() {
  using namespace GrAL;

  typedef cartesiannd::grid<2> def_grid_type;
  typedef Complex2D            img_grid_type;
  typedef grid_types<def_grid_type> dgt;
  typedef grid_types<img_grid_type> igt;
  
  def_grid_type R(3,3);
  img_grid_type C;
  ConstructGrid0(C, R);

  cell_morphism  <def_grid_type, img_grid_type> Mc(R,C);
  cell_morphism  <def_grid_type, img_grid_type> Mch(R,C);
  dgt::CellIterator cR;
  igt::CellIterator cC;
  for(cR=R.FirstCell(), cC=C.FirstCell(); !cR.IsDone(); ++cR, ++cC) {
    Mc[*cR] = *cC; 
    REQUIRE_ALWAYS(Mc(*cR) == *cC, "", 1);
    REQUIRE_ALWAYS(Mc(cR.handle()) == cC.handle(), "", 1);
    Mch[cR.handle()] = cC.handle();
    REQUIRE_ALWAYS(Mch(*cR) == *cC, "", 1);
    REQUIRE_ALWAYS(Mch(cR.handle()) == cC.handle(), "", 1);
  }



  vertex_morphism<def_grid_type, img_grid_type> Mv(R,C);
  vertex_morphism<def_grid_type, img_grid_type> Mvh(R,C);
  dgt::VertexIterator vR;
  igt::VertexIterator vC;
  for(vR=R.FirstVertex(), vC=C.FirstVertex(); !vR.IsDone(); ++vR, ++vC) {
    Mv[*vR] = *vC; 
    REQUIRE_ALWAYS(Mv(*vR) == *vC, "", 1);
    REQUIRE_ALWAYS(Mv(vR.handle()) == vC.handle(), "", 1);
    Mvh[vR.handle()] = vC.handle();
    REQUIRE_ALWAYS(Mvh(*vR) == *vC, "", 1);
    REQUIRE_ALWAYS(Mvh(vR.handle()) == vC.handle(), "", 1);
  }
};
