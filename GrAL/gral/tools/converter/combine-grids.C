
/* ------------------------------------------------------------

    Copyright (C) 2001 - 2009 Guntram Berti,
    Contact: guntram AT users DOT berlios DOT de

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */

#include <Gral/IO/complex2d-format.h>
#include <Gral/Grids/Complex2D/all.h>

#include "Gral/Views/disjoint-union-view.h"
#include <Gral/Geometries/simple-geometry.h>

#include <IO/control-device.h>

#include <Container/tuple-point-traits.h>

#include <string>
#include <vector>

int main(int argc, char* argv[]) {
  using namespace GrAL;
  using namespace std;

  ControlDevice Ctrl = 
    GetCommandlineAndFileControlDevice(argc,argv,"","main");

  string commandname = argv[0];
  string h; 
  h += commandname + ": Combine a number of grids into one.\n";

  vector<string> grid_in;
  Ctrl.add( "-i",  GrAL::GetPushbackMutator(grid_in));
  Ctrl.add( "-in", GrAL::GetPushbackMutator(grid_in));
  Ctrl.add( "+i",  GrAL::GetPushbackMutator(grid_in));
  Ctrl.add( "+in", GrAL::GetPushbackMutator(grid_in));
  // read all file names on remainder of command line 
  // Ctrl.add( "-grids", GrAL::GetPushbackMutator(grid_in, GrAL::Mutator::all));
  h += "   -i|-in|+i|+in <grid>  (add input grid, complex2d format)\n";


  string grid_out;
  RegisterAt(Ctrl, "-o",   grid_out);
  RegisterAt(Ctrl, "-out", grid_out);
  h += "   -o|-out <grid>  (output grid, complex2d format)\n";



  Mutator helpmsg = GetMessageOnReadMutator(cerr,h);
  Ctrl.add("-h",     helpmsg);
  Ctrl.add("--help", helpmsg);
  Ctrl.add("-?",     helpmsg);

  Ctrl.update();

  if(grid_in.size() == 0) {
    return 0;
  }
  if(grid_out == "")
    grid_out = "-";

  typedef tuple<double,3>                               coord_type;
  typedef Complex2D                                     grid_type;
  typedef simple_geometry<grid_type, coord_type>        geom_type;
  
  int curr = 0;

  IstreamComplex2DFmt In(grid_in[curr]);
  ref_ptr<grid_type>  G     = new_ref_ptr(new grid_type());
  ref_ptr<geom_type>  GeomG = new_ref_ptr(new geom_type());
  ConstructGrid(*G,*GeomG, In,In);
  
  ++curr;
  while(curr < grid_in.size()) {
    IstreamComplex2DFmt In(grid_in[curr]);

    ref_ptr<grid_type>  CurrG     = new_ref_ptr(new grid_type());
    ref_ptr<geom_type>  GeomCurrG = new_ref_ptr(new geom_type());
    ConstructGrid(*CurrG,*GeomCurrG, In,In);
    disjoint_union_view::grid_view<grid_type, grid_type> union_grid_view(G, CurrG);
    disjoint_union_view::geom_view<geom_type, geom_type> union_geom_view(union_grid_view, GeomG, GeomCurrG);
    
    ref_ptr<grid_type>  UnionG     = new_ref_ptr(new grid_type());
    ref_ptr<geom_type>  GeomUnionG = new_ref_ptr(new geom_type());
    ConstructGrid(*UnionG, *GeomUnionG, union_grid_view, union_geom_view);
    swap(G,     UnionG);
    swap(GeomG, GeomUnionG);

    ++curr;
  }
  cerr << "Final grid: " << G->NumOfCells() << " cells, " << G->NumOfVertices() << " vertices.\n";

  cerr << "Writing grid to \"" << grid_out << "\" ... " << flush;  
  OstreamComplex2DFmt Out(grid_out);
  ConstructGrid(Out, *G, *GeomG);
  cerr << " done." << endl;

}
