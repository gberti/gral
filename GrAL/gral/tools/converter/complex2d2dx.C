#include "Gral/IO/complex2d-format-input.h"
#include "Gral/Grids/Complex2D/all.h"
#include "Gral/Geometries/simple-geometry.h"

//#include "Gral/IO/opendx-format-output2d.h"

#include "IO/control-device.h"

#include <fstream>

int main(int argc, char* argv[]) {
  using namespace std;

  cout << "------ Starting complex2d2dx -------\n" << endl;

  ControlDevice Ctrl = 
    GetCommandlineAndFileControlDevice(argc,argv,"","main");
  string h = "complex2d2dx - converted mesh in complex2d format to Open DX format\n";
  h += "Usage: complex2d2dx -g <complex2d> -o <gmv>  -d <space dim = 2> -off <offset = 0>\n";
  string grid_in;
  RegisterAt(Ctrl, "-g", grid_in);
  string grid_out;
  RegisterAt(Ctrl, "-o", grid_out);
  unsigned sdim = 2;
  RegisterAt(Ctrl, "-d", sdim);
  unsigned offset = 0;
  RegisterAt(Ctrl, "-off", offset);

  Mutator* helpmsg = new MessageOnReadMutator(cerr,h);
  Ctrl.add("-h",     helpmsg);
  Ctrl.add("--help", helpmsg);
  Ctrl.add("-?",     helpmsg);


  Ctrl.update();

  IstreamComplex2DFmt In(grid_in, offset);
  In.set_spacedim(sdim);

  cout << "Reading grid from file " << grid_in << " ... " << flush;

  Complex2D G;
  typedef coordN<3> coord_type;
  simple_geometry<Complex2D, coordN<3> >  GeomG(G);
  //  stored_geometry_complex2D GeomG(G);
  ConstructGrid(G, GeomG, In, In);
  cout << " done." << endl; 

  typedef grid_types<Complex2D> gt;
  // typedef stored_geometry_complex2D::coord_type coord_type;

  typedef point_traits<coord_type> pt;

  cout << "Writing DX file to \"" << grid_out << "\" ... ";
  ofstream out(grid_out.c_str());

  out << "# Created by complex2d2dx\n";
  // vertices
  out << "object 1 class array type float rank 1 "
      << "shape 3 " 
      << " items " << G.NumOfVertices()
      << " data follows " << std::endl;
  for(gt::VertexIterator v(G); ! v.IsDone(); ++v) {
    out << pt::x(GeomG.coord(*v)) << ' '  << pt::y(GeomG.coord(*v)) << ' ' << pt::z(GeomG.coord(*v))  << '\n';
  }

  if(G.NumOfArchetypes() > 1) {
    std::cerr << "Cannot handle different 2d cell types, quitting." << endl;
    out.close();
    exit(2);
  }

  out << "object 2 class array type int rank 1 "
      << "shape " << (*G.BeginArchetype()).NumOfVertices() << " "
      << "items " << G.NumOfCells() << " "
      << "data follows" << "\n";
  for(gt::CellIterator c(G); ! c.IsDone(); ++c) {
    if((*G.BeginArchetype()).NumOfVertices() == 3) {
      for(gt::VertexOnCellIterator vc(*c); !vc.IsDone(); ++vc)
	out << vc.handle() << ' ';
      out << '\n';
    }
    else if((*G.BeginArchetype()).NumOfVertices() == 4) {
      out << (*c).V(1).handle() << ' ' << (*c).V(2).handle() << ' '
	  << (*c).V(4).handle() << ' ' << (*c).V(3).handle() << "\n";
    }
    else {
      cerr << "Cannot handle 2D elements with more than 4 vertices, quitting!" << endl;
      exit(2);
    }
  }

  out << "attribute \"element type\" "
      << "string \"" 
      << ((*G.BeginArchetype()).NumOfVertices()== 3 ? "triangles" : "quads") << "\"\n"
      << "attribute \"ref\" string \"positions\"\n";
  out << "object \"irregular positions irregular connections\" class field\n"
      << "component \"positions\" value 1\n"
      << "component \"connections\" value 2\n";
  out << "end" << endl;
  out.close();

  cout << " done." << endl;
}
