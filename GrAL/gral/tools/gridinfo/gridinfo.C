
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
  using std::string;

  typedef Complex2D grid_type;
  typedef grid_types<grid_type> gt;
  //  typedef stored_geometry_complex2D geom_type;
  typedef point coord_type;
  typedef simple_geometry<Complex2D,point> geom_type;

  ControlDevice Ctrl = 
    GetCommandlineAndFileControlDevice(argc,argv,"gridinfo.in","main");

  string grid_in;
  RegisterAt(Ctrl, "-g", grid_in);
  string grid_out = "grid.out";
  RegisterAt(Ctrl, "-o", grid_out);

  string vertex_in;
  RegisterAt(Ctrl, "-v", vertex_in);
  unsigned sdim = 2;
  RegisterAt(Ctrl, "-d", sdim);
  unsigned offset = 0;
  RegisterAt(Ctrl, "-off", offset);

  bool do_bbox = false;
  Ctrl.add("-bbox", GetTrueOnReadMutator(do_bbox));

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



