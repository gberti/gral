/*! \file */

#include "Gral/IO/complex2d-format-input.h"
#include "Gral/IO/write-stl.h"

#include "Gral/Grids/Complex2D/all.h"
#include "Gral/Geometries/simple-geometry.h"

#include "Container/tuple-point-traits.h"
#include "Geometry/algebraic-primitives.h"

#include "IO/control-device.h"
#include "Utility/file-utils.h"


#include <string>

/*! \brief complex2d2stl - Convert into STL format

    STL is an inferior format used by a number of commercial tools ("industry standard").
    It contains a simple list of triangles given by vertex coordinates and thus looses
    connectivity information.
*/


int main(int argc, char* argv[]) {
  bool res = true;

  using namespace std;
  using namespace GrAL;

  ControlDevice Ctrl =
    GetCommandlineAndFileControlDevice(argc,argv,"","main");

  string helptxt =
    "complex2d2stl -g|-i|-in <grid> -o|-out <output> -d <spacedim=3> -off <offset=0> -name <stl geom name=\"\">\n";
  string grid_in;
  RegisterAt(Ctrl, "-g" , grid_in);
  RegisterAt(Ctrl, "-i" , grid_in);
  RegisterAt(Ctrl, "-in", grid_in);

  string grid_out;
  RegisterAt(Ctrl, "-o",   grid_out);
  RegisterAt(Ctrl, "-out", grid_out);

  string  stl_geom_name;
  RegisterAt(Ctrl, "-name", stl_geom_name);

  unsigned sdim = 3;
  RegisterAt(Ctrl, "-d", sdim);

  unsigned offset = 0;
  RegisterAt(Ctrl, "-off", offset);

  Mutator helpmsg =  GetMessageOnReadMutator(cerr,helptxt);
  Ctrl.add("-h",     helpmsg);
  Ctrl.add("--help", helpmsg);
  Ctrl.add("-?",     helpmsg);

  Ctrl.update();

  if(grid_in == "" || grid_out == "") {
    std::cerr << helptxt;
    return 2;
  }

  IstreamComplex2DFmt In(grid_in, offset);
  In.set_spacedim(sdim);
 
  typedef tuple<double,3>    coord_type;
  Complex2D                              G;
  simple_geometry<Complex2D, coord_type> GeomG;
  ConstructGrid(G,GeomG, In,In);

  ref_ptr<std::ostream> stl = get_file_or_stdout(grid_out);
  if(stl == 0) return 2;
  int num_non_triangles = 0;
  res = res && write_stl_ascii(*stl, G, GeomG, stl_geom_name, num_non_triangles);
  if(num_non_triangles > 0)
    cerr << "WARNING: " << num_non_triangles << " cells with more than 3 vertices ignored!" << endl;

  return res;
}
