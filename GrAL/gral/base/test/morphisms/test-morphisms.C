/*! \file */


/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include "Gral/Base/grid-morphism.h"

#include "Gral/Grids/Complex2D/all.h"
#include "Gral/Grids/CartesianND/all.h"

#include <iostream>

namespace GrAL {
  typedef cartesiannd::grid<2> def_grid_type;
  typedef Complex2D            img_grid_type;
  typedef grid_types<def_grid_type> gt_def;
  typedef grid_types<img_grid_type> gt_img;

  template class vertex_morphism<def_grid_type, img_grid_type>;
  template class cell_morphism  <def_grid_type, img_grid_type>;
  template class element_morphism<gt_def::Edge, gt_img::Edge>;
}


int main() {
  using namespace GrAL;
  using namespace std;

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

  element_morphism<dgt::Edge, igt::Edge> Me(R,C);
  element_morphism<dgt::Edge, igt::Edge> Me_from_v(Mv);

  dgt::EdgeIterator eR;
  igt::Edge eC;
  for(eR=R.FirstEdge(); !eR.IsDone(); ++eR) {
    Me[*eR] = Me_from_v(*eR);
    eC = Me(*eR);
    REQUIRE_ALWAYS(Me(eR.handle()) == eC.handle(), "", 1);
    Me[eR.handle()] = eC.handle();
    REQUIRE_ALWAYS(Me(*eR) == eC, "", 1);

    vertex_set<dgt::Edge> vR(*eR);
    vertex_set<igt::Edge> vC(eC);
    // cout << vR[0] << " " << vR[1]  << " ->  " << eC.v1() << " " << eC.v2() << "\n"; 
    
    vertex_set<igt::Edge> mapped_vR(2);
    mapped_vR[0] = Mv(vR[0]);
    mapped_vR[1] = Mv(vR[1]);
    REQUIRE_ALWAYS(vC == mapped_vR, "", 1);
  }
  

};
