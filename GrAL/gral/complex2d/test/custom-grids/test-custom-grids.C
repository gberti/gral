
// $LICENSE_NEC_2005

/*! \file

*/

#include "Gral/Grids/Complex2D/custom-grids.h"
#include "Gral/IO/gmv-format-output2d.h"


int main() {
  using namespace GrAL;

  typedef custom_grids CG; 
  {
    OstreamGMV2DFmt Out("simplex.gmv");
    ConstructGrid(Out, CG::Tetrahedron(), CG::TetrahedronGeom());
  }
  {
    OstreamGMV2DFmt Out("pyramid.gmv");
    ConstructGrid(Out, CG::Pyramid(), CG::PyramidGeom());
  }
  {
    OstreamGMV2DFmt Out("prism.gmv");
    ConstructGrid(Out, CG::Prism(), CG::PrismGeom());
  }
  {
    OstreamGMV2DFmt Out("cube.gmv");
    ConstructGrid(Out, CG::Cube(), CG::CubeGeom());
  }
}
