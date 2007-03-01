/*! \file */

#include "Gral/IO/complex2d-format-input.h"
#include "Gral/Grids/Complex2D/all.h"
#include "Gral/Geometries/simple-geometry.h"

#include "Container/tuple-point-traits.h"
#include "Geometry/algebraic-primitives.h"

#include "IO/control-device.h"

#include <string>

/*! \brief complex2d2stl - Convert into STL format

    Inferior format used by a number of commercial tools ("industry standard")
*/


int main(int argc, char* argv[]) {
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

  typedef tuple<double,3>                  coord_type;
  typedef point_traits<coord_type>         pt;
  typedef algebraic_primitives<coord_type> ap;
 
  Complex2D G;
  simple_geometry<Complex2D, coord_type> GeomG;
  ConstructGrid(G,GeomG, In,In);
  typedef grid_types<Complex2D> gt;

  ofstream stl(grid_out.c_str());
  int num_non_triangles = 0;

  stl << "solid <<" << stl_geom_name << ">>\n";
  for(gt::CellIterator c(G); !c.IsDone(); ++c) {
    if((*c).NumOfVertices() == 3) {
      coord_type v[3];
      gt::VertexOnCellIterator vc(*c);
      v[0] = GeomG.coord(*vc);
      v[1] = GeomG.coord(*++vc);
      v[2] = GeomG.coord(*++vc);
      stl << "  "   << "facet normal " 
	  << ap::normalization(ap::vectorproduct(v[1]-v[0],v[2]-v[0])) << "\n";
      stl << "    " << "outer loop\n";
      for(gt::VertexOnCellIterator vc(*c); !vc.IsDone(); ++vc) {
	stl << "     " << "vertex " << GeomG.coord(*vc) << "\n";
      }
      stl << "    " << "endloop\n";
      stl << "  " << "end facet\n";

    }
    else
      ++num_non_triangles;
  }
  stl << "endsolid <<" << stl_geom_name << ">>\n";

  if(num_non_triangles > 0)
    cerr << "WARNING: " << num_non_triangles << " cells with more than 3 vertices ignored!" << endl;
}
