

#include <algorithm>
#include <fstream.h>

// External control
#include "IO/control-device.h"
#include "IO/safe-file.h"

// Utility
#include "Utility/pre-post-conditions.h"
#include "Utility/as-string.h"

// Gral Base
#include "Gral/IO/istream-grid-complex2d-fmt.h"

// Gral Complex2D
#include "Gral/Grids/Complex2D/complex2d.h"
#include "Gral/Grids/Complex2D/grid-functions.h"
#include "Gral/Grids/Complex2D/partial-grid-functions.h"
#include "Gral/Grids/Complex2D/construct.h"
#include "Gral/Grids/Complex2D/test-complex2d.h"
#include "Gral/Grids/Complex2D/stored-geometry.h"

typedef stored_geometry_complex2D geometry_type;
typedef geometry_type::coord_type coord_type;



int main(int argc, char* argv[]) {

  typedef grid_types<Complex2D> gt; 

  ControlDevice Ctrl = GetCommandlineAndFileControlDevice(argc,argv,"test.in","main");
  

  string grid_file = "grid.in";
  RegisterAt(Ctrl,"grid-file",grid_file);


  TestComplex2D T;
  T.register_at(Ctrl);

  Ctrl.update();

  ostream& testout(cout);  


   
  int NCC = 4;
  vector<Complex2D> CC(NCC);
  
  testout << "constructing Complex2D CC[0] from file " << grid_file << "\n";
  
  ifstream infile2; 
  file_interactive::open(infile2,grid_file);
  IstreamComplex2DFmt IG(infile2,1);
  geometry_type Geom3(CC[0]);
  ConstructGrid(CC[0], Geom3,  IG, IG);
  infile2.close();
  
  testout << "constructing Complex2D CC[1] from CC[0]\n";
  CC[1] = CC[0];
  
  testout << "constructing Complex2D CC[2] from file (2 times) " << grid_file << "\n";
  for(int k = 1; k <= 2; ++k) {
    ifstream infile;
    file_interactive::open(infile,grid_file);
    IstreamComplex2DFmt IG(infile,1);
    geometry_type Geom(CC[2]);
    ConstructGrid(CC[2], Geom,  IG, IG);
    infile.close();
  }
  
  testout << "leaving CC[3] empty \n"; 
  
  ofstream outfile;
  for(int i =0; i < NCC; ++i) {
    string nm = "CC" + as_string(i) + ".out";
    outfile.open(nm.c_str());
    OstreamComplex2DFmt OG1(testout);
    ConstructGrid(OG1, OG1, CC[i], geometry_type(CC[i]));
    OstreamComplex2DFmt OG2(outfile);
    ConstructGrid(OG2, OG2, CC[i], geometry_type(CC[i]));
    outfile.close();
  }
 
   int Ncases = 8;
   for(int ii = 1; ii <= Ncases; ++ii) {
     ifstream casei;
     file_interactive::open(casei, ("case" + as_string(ii)  + ".grid"));
     CC.push_back(Complex2D());
     IstreamComplex2DFmt IG(casei,1);
     geometry_type Geom(CC.back());
     ConstructGrid(CC.back(), Geom, IG, IG);
     casei.close();

     ofstream casei_out;
     casei_out.open(("case" + as_string(ii)  + ".grid.out").c_str());
     OstreamComplex2DFmt OG(casei_out);
     ConstructGrid(OG, OG, CC.back(), geometry_type(CC.back()));
     casei_out.close();
   }


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
