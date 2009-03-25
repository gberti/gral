
/* ------------------------------------------------------------

    Copyright (C) 2001 - 2009 Guntram Berti,
    Contact: guntram AT users DOT berlios DOT de

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */

#include "Gral/IO/complex2d-format-input.h"
#include "Gral/IO/complex2d-format-output.h"
#include "Gral/Grids/Complex2D/all.h"

#include "Gral/Geometries/simple-geometry.h"
#include "Gral/Geometries/bounding-box.h"

#include "IO/control-device.h"

#include "Geometry/point.h"

#include <fstream>
#include <vector>

int main(int argc, char* argv[]) {
  using namespace GrAL;
  using namespace std;

  typedef Complex2D grid_type;
  typedef grid_types<grid_type> gt;
  //  typedef stored_geometry_complex2D geom_type;
  typedef point coord_type;
  typedef simple_geometry<Complex2D,point> geom_type;

  ControlDevice Ctrl = 
    GetCommandlineAndFileControlDevice(argc,argv,"gridinfo.in","main");
  string help =
    "gridinfo: Get information on a grid (in complex2d format)\n";
  (help 
    += "Usage: gridinfo <options>\n")
    += "where <options> are\n";

  string grid_in;
  RegisterAt(Ctrl, "-g", grid_in); help += " -g <grid>\n";
  string grid_out = "grid.out";
  RegisterAt(Ctrl, "-o", grid_out); help += " -o <output grid>\n";

  string vertex_in;
  RegisterAt(Ctrl, "-v", vertex_in); help += " -v <vertexfile> (vertices to process)\n";
  unsigned sdim = 2;
  RegisterAt(Ctrl, "-d", sdim);      
  help += " -d <space dim>  (dimension of geometry, 2 (default) or 3)\n";
  unsigned offset = 0;
  RegisterAt(Ctrl, "-off", offset); 
  help += " -off <offset> (offset of vertex number, 0 (default) or 1)\n";

  bool do_bbox = false;
  Ctrl.add("-bbox", GetTrueOnReadMutator(do_bbox)); help += " -bbox (print bounding box)\n";
 
  AddHelp(Ctrl,help);
  Ctrl.update();

  IstreamComplex2DFmt In(grid_in, offset);
  In.set_spacedim(sdim);

  grid_type G;
  geom_type GeomG;
  ConstructGrid(G, GeomG, In, In);

  std::ifstream vert_file(vertex_in.c_str());
  std::vector<gt::vertex_handle> vertices;
  if(! vert_file.is_open())
    cerr << "no vertex file\n";
  else {
    gt::vertex_handle v;
    while(vert_file >> v)
      vertices.push_back(v);
  }
  for(int i = 0; i < vertices.size(); ++i) {
    // cout << "v" << vertices[i] << "  ";
    cout << GeomG.coord(gt::Vertex(G,vertices[i]-offset)) << "\n";
  }

  if(do_bbox) {
    box<coord_type> bb = get_grid_bounding_box(G,GeomG);
    cout << "Bounding box: [" << bb.the_min() << "]  [" 
	 << bb.the_max() << "]" << endl;
  }

  OstreamComplex2DFmt Out(grid_out); 
  ConstructGrid(Out,G,GeomG);
}  



