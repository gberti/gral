

#include <algorithm>
#include <fstream.h>
#include "IO/control-device.h"
#include "IO/safe-file.h"

#include "Utility/pre-post-conditions.h"
#include "Utility/as-string.h"

#include "Gral/Grids/Complex2D/complex2d.h"
#include "Gral/Grids/Complex2D/grid-functions.h"
#include "Gral/Grids/Complex2D/partial-grid-functions.h"
#include "Gral/Grids/Complex2D/construct.h"

#include "Grids/Reg2D/cartesian-grid2d-with-holes.h"
#include "Visualization/generic/meshview.simple.h"

#include "Gral/Grids/Complex2D/test-complex.h"
#include "./problem-types.h"

//typedef istream_iterator<int,ptrdiff_t> istream_it;
typedef istream_iterator<int>           istream_it;


int main(int argc, char* argv[]) {

  typedef grid_types<Complex2D> gt; 

  ControlDevice Ctrl = GetCommandlineAndFileControlDevice(argc,argv,"test.in","main");
  

  int nx = 4;
  int ny = 5;
  int llx = nx/2;
  int lly = 0;
  int urx = nx-1;
  int ury = ny/2;

  RegisterAt(Ctrl,"ngrid_x",nx);
  RegisterAt(Ctrl,"ngrid_y",ny);
  RegisterAt(Ctrl,"llx",llx);  
  RegisterAt(Ctrl,"lly",lly);  
  RegisterAt(Ctrl,"urx",urx);  
  RegisterAt(Ctrl,"ury",ury);  

  string grid_file = "grid.in";
  RegisterAt(Ctrl,"grid-file",grid_file);


  TestComplex2D T;
  T.register_at(Ctrl);

  Ctrl.update();

  ostream& testout(cout);  

  // ifstream infile;
  // infile.open("grid.cell2d");
  // CC.read(infile);


  //unit_quad2d geom_mapping;

  // kopiere CC von einem regulaeren Gitter mit ngrid x ngrid Knoten,
  // mit den durch die Abb. mapping_type : [0,1]^2 -> R^d gegebenen Koordinaten.

   source_mapping_type mapping;
   //RegGrid2D  source(nx,ny);
    RegGrid2DWithHoles::rectangle rect(llx,lly,urx,ury);
   RegGrid2DWithHoles  source(nx,ny,rect);

   
   ifstream infile;

    int NCC = 6;
    vector<Complex2D> CC(NCC);
   // construct from arrays
   
    file_interactive::open(infile, grid_file);
    //if(infile.good()) {
    //  cerr << "closing infile\n";
    //  infile.close();
    // }
    //const char * fn = grid_file.c_str();
    //cerr << '"' << fn << '"' << '\n';
    // infile.open(fn);
    // infile.open(grid_file.c_str());
    // if(!infile)
    //   cerr << "konnte " << fn << " nicht oeffnen!\n";
    vector<int> griddescr;
   int nvert, ncell;
   infile >> nvert >> ncell;

    griddescr.push_back(nvert);
    griddescr.push_back(ncell);

    vector<coord_type> gridcoord(nvert);
    for(int v = 0; v < nvert; ++v)
      infile >> gridcoord[v];
    
    istream_it begfile(infile);
    istream_it endfile;
    copy(begfile,endfile,back_inserter(griddescr));
    infile.close();
  
    /*
      copy(griddescr.begin(),griddescr.end(),ostream_iterator<int>(cout,"\n"));
      copy(gridcoord.begin(),gridcoord.end(),ostream_iterator<coord_type>(cout,"\n"));
    */



    testout << "constructing grid CC0 from coord-array and connectivity-array\n";
    CC[0].construct(griddescr,gridcoord);



   testout << "constructing Complex2D CC1 from regular source\n";
   Construct(CC[1],source, source_geom_type(mapping,source.BaseGrid()));
   
   
   testout << "constructing Complex2D CC2 from CC1\n";
   CC[2] = CC[1];
   
   testout << "constructing Complex2D CC3 from file " << grid_file << "\n";

   ifstream infile2; 
   file_interactive::open(infile2,grid_file);
   CC[3].read(infile2);
   infile2.close();

   /*   
   file_interactive::open(infile,grid_file);
   CC[3].read(infile);
   infile.close();
   */
   testout << "constructing Complex2D CC4 from file (2 times) " << grid_file << "\n";
   for(int k = 1; k <= 2; ++k) {
     ifstream infile3;
     file_interactive::open(infile3,grid_file);
     CC[4].read(infile3);
     infile3.close();
   }
 
   testout << "leaving CC5 empty \n"; 

   ofstream outfile;
   for(int i =0; i < NCC; ++i) {
     string nm = "CC" + as_string(i) + ".out";
     outfile.open(nm.c_str());
     CC[i].write(testout);
     CC[i].write(outfile);
     outfile.close();
  }
 
   int Ncases = 8;
   for(int ii = 1; ii <= Ncases; ++ii) {
     ifstream casei;
     file_interactive::open(casei, ("case" + as_string(ii)  + ".grid"));
     CC.push_back(Complex2D());
     CC.back().read(casei);
     casei.close();

     ofstream casei_out;
     casei_out.open(("case" + as_string(ii)  + ".grid.out").c_str());
     CC.back().write(casei_out);
     casei_out.close();
   }


  // graphical output  
  ofstream oogl_output;
  oogl_output.open("mesh.oogl");
  MeshView<Complex2D,geometry_type> view(CC[1],geometry_type(CC[1]));
  oogl_output << view;
  oogl_output.close();

  // textual output

   vector<Complex2D>::const_iterator CCi;
  for(CCi = CC.begin(); CCi != CC.end()-Ncases; ++CCi) { 
    testout << "Testing CC[" << CCi - CC.begin() << "]\n";
    T.test_complex2d_iterators(*CCi,geometry_type(*CCi),testout);
    testout << "---------------------------------------------------\n"
	    << "---------------------------------------------------\n\n";
  }

  partial_grid_function<gt::Vertex, int> pgfv(CC[1]);
  typedef partial_grid_function<gt::Vertex, int>::VertexIterator pgfVertexIterator;
  for(pgfVertexIterator v = pgfv.FirstVertex(); ! v.IsDone(); ++v)
    pgfv[*v] = - pgfv(*v);

  partial_grid_function<gt::Edge, int> pgfe(CC[1]);
  typedef partial_grid_function<gt::Edge, int>::EdgeIterator     pgfEdgeIterator;
  for(pgfEdgeIterator e = pgfe.FirstEdge(); ! e.IsDone(); ++e)
    pgfe[*e] = - pgfe(*e);

  partial_grid_function<gt::Cell, int> pgfc(CC[1]);
  typedef partial_grid_function<gt::Cell, int>::CellIterator     pgfCellIterator;
  for(pgfCellIterator c = pgfc.FirstCell(); ! c.IsDone(); ++c)
    pgfc[*c] = - pgfc(*c);
  
  return 0;
}
