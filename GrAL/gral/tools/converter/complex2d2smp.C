
/* ------------------------------------------------------------

    Copyright (C) 2001 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */

#include "Gral/IO/complex2d-format-input.h"
#include "Gral/Grids/Complex2D/all.h"
#include "Gral/Geometries/simple-geometry.h"

#include "Container/tuple-point-traits.h"

#include "IO/control-device.h"

#include <string>

/*! \brief complex2d2smp - Convert into SMP format

   Format used for vxt voxeliser.
*/

int main(int argc, char* argv[]) {
  using namespace std;
  using namespace GrAL;

  ControlDevice Ctrl =
    GetCommandlineAndFileControlDevice(argc,argv,"","main");

  string helptxt =
    "complex2smp -g|-i|-in <grid> -o|-out <output> -d <spacedim=3> -off <offset=0>\n";
  string grid_in;
  RegisterAt(Ctrl, "-g" , grid_in);
  RegisterAt(Ctrl, "-i" , grid_in);
  RegisterAt(Ctrl, "-in", grid_in);

  string grid_out;
  RegisterAt(Ctrl, "-o",   grid_out);
  RegisterAt(Ctrl, "-out", grid_out);

  unsigned sdim = 3;
  RegisterAt(Ctrl, "-d", sdim);

  unsigned offset = 0;
  RegisterAt(Ctrl, "-off", offset);

  Mutator helpmsg =  GetMessageOnReadMutator(cerr,helptxt);
  Ctrl.add("-h",     helpmsg);
  Ctrl.add("--help", helpmsg);
  Ctrl.add("-?",     helpmsg);

  Ctrl.update();

  IstreamComplex2DFmt In(grid_in, offset);
  In.set_spacedim(sdim);

  typedef tuple<double,3>          coord_type;
  typedef point_traits<coord_type> pt;
 
  Complex2D G;
  simple_geometry<Complex2D, coord_type> GeomG;
  ConstructGrid(G,GeomG, In,In);
  typedef grid_types<Complex2D> gt;

  ofstream grid_out_file(grid_out.c_str());
  int num_non_triangles = 0;

  for(gt::CellIterator c(G); !c.IsDone(); ++c) {
    if((*c).NumOfVertices() == 3) {
      grid_out_file << "triangle ";
      for(gt::VertexOnCellIterator vc(*c); !vc.IsDone(); ++vc)
	grid_out_file << "(" << pt::x(GeomG.coord(*vc)) << " " << pt::y(GeomG.coord(*vc)) << " " << pt::z(GeomG.coord(*vc)) << ")";
      grid_out_file << ";\n";
    }
    else
      ++num_non_triangles;
  }

  if(num_non_triangles > 0)
    cerr << "WARNING: " << num_non_triangles << " cells with more than 3 vertices ignored!" << endl;
}
