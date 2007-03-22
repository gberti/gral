#include "Gral/IO/complex2d-format.h"

#include "Container/tuple.h"
#include "Container/tuple-point-traits.h"
#include "IO/control-device.h"

//#include "Gral/Grids/CartesianND/all.h"
#include "Gral/Grids/Cartesian2D/all.h"
#include "Geometry/affine-mapping.h"
#include "Geometry/matrix.h"


#include <string>

int main(int argc, char* argv[]) {
  using namespace GrAL;
  using namespace std;

  //  typedef grid_types<cartesiannd::grid<2> > gt;
  //typedef cartesiannd::grid<2>              grid_type;
  typedef cartesian2d::CartesianGrid2D        grid_type;
  typedef grid_types<grid_type>               gt;

  //  typedef cartesiannd::default_coord<grid_type>::type   coord_type;
  typedef tuple<double,2>                               coord_type;
  typedef matrix<2,2,0>                                 matrix_type;
  typedef affine_mapping<matrix_type, coord_type>       mapping_type;
  //  typedef cartesiannd::mapped_geometry<grid_type, mapping_type> geom_type;
  typedef cartesian2d::mapped_geometry<mapping_type> geom_type;
  typedef point_traits<coord_type>  pt;


  ControlDevice Ctrl =
    GetCommandlineAndFileControlDevice(argc,argv,"","main");

  string h;
  h += "cartesian2complex2d -o <output> [-off <offset=0> -vx <int> -vy<int> -ll <x y> -ur <x y>]\n";

  string grid_out;
  RegisterAt(Ctrl, "-o", grid_out);

  int vx = 0;
  int vy = 0;
  RegisterAt(Ctrl, "-vx", vx);
  RegisterAt(Ctrl, "-vy", vy);

  coord_type ll(0.0);
  coord_type ur(1.0);
  RegisterAt(Ctrl, "-ll", ll);
  RegisterAt(Ctrl, "-ur", ur);

  unsigned offset_out = 0;
  RegisterAt(Ctrl, "-off", offset_out);


  AddHelp(Ctrl, h);

  Ctrl.update();


  grid_type R(gt::index_type(0,0), gt::index_type(vx-1,vy-1));

  matrix_type A(0.0);
  A(0,0) = pt::x(ur) - pt::x(ll);
  A(1,1) = pt::y(ur) - pt::y(ll);
  coord_type translation(ll);
  mapping_type M(A, translation);
  geom_type GeomR(R, M);


  OstreamComplex2DFmt Out(grid_out, offset_out);

  ConstructGrid(Out, R, GeomR);
}
