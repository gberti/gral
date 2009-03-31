

/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */



/*! \file 
    \brief Test cases for \c Complex2D

    <H4> Degenerate Test Cases </H4>
    <TABLE BORDER BGCOLOR="#FFFFF0">
     <TR ALIGN=CENTER> <TH> file </TH>  <TH> description </TH>  <TH> outcome </TH> </TR>
     <TR ALIGN=CENTER> <TD>  <A HREF="case1.grid"> case1.grid </A> </TD> 
	<TD> loop (1 Vertex, 1 Cell) </TD> <TD> normals broken</TD>
      <TR ALIGN=CENTER> <TD>  <A HREF="case2.grid"> case2.grid </A> </TD> 
	<TD> loop (1 Vertex, 2 Cells) </TD> <TD> normals broken </TD>
      <TR ALIGN=CENTER> <TD>  <A HREF="case3.grid"> case3.grid </A> </TD> 
	<TD> 2gon (2 V, 1 C) </TD> <TD> Cells nb of itself </TD>
      <TR ALIGN=CENTER> <TD>  <A HREF="case4.grid"> case4.grid </A> </TD> 
	<TD> 2gon + outside (2V, 2C) </TD> <TD> Cells are nb. of themselves </TD>
      <TR ALIGN=CENTER> <TD>  <A HREF="case5.grid"> case5.grid </A> </TD>
	<TD> one %triangle (3V, 1C)</TD> <TD> OK</TD>
      <TR ALIGN=CENTER> <TD>  <A HREF="case6.grid"> case6.grid </A> </TD> 
	<TD> %triangle + outside (3V, 2C)</TD> <TD> OK </TD>
      <TR ALIGN=CENTER> <TD>  <A HREF="case7.grid"> case7.grid </A> </TD> 
	<TD> 2 triangles w. one common vertex (5V, 2C) </TD>  <TD> OK</TD>
      <TR ALIGN=CENTER> <TD>  <A HREF="case8.grid"> case8.grid </A> </TD> 
      <TD> same with outside (5V, 3C)</TD> <TD> shared vertex has 4 cells (should be 3)</TD>
    </TABLE>
    
   <H4> Good test cases </H4>
      <LI> <A HREF="grid.in"> grid.in </A> simple 2x2 (4 cells) regular grid
*/

#include <algorithm>
#include <vector>

#include <fstream>

// External control
#include "IO/control-device.h"

// Utility
#include "Utility/safe-file.h"
#include "Utility/pre-post-conditions.h"
#include "Utility/as-string.h"

// Gral Base
#include "Gral/IO/complex2d-format.h"

#include "Gral/Test/test-archetypes.h"
#include "Gral/Test/test-sequence-iterator.h"
#include "Gral/Test/test-incidence-iterator.h"

// Gral Complex2D
#include "Gral/Grids/Complex2D/complex2d.h"
#include "Gral/Grids/Complex2D/grid-functions.h"
#include "Gral/Grids/Complex2D/partial-grid-functions.h"
#include "Gral/Grids/Complex2D/construct.h"
#include "Gral/Grids/Complex2D/test-complex2d.h"
#include "Gral/Grids/Complex2D/stored-geometry.h"


typedef GrAL::stored_geometry_complex2D geometry_type;
typedef geometry_type::coord_type coord_type;



int main(int argc, char* argv[]) {
  using namespace GrAL;
  using namespace std;

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
    OstreamComplex2DFmt OG1(testout,1);
    ConstructGrid(OG1, OG1, CC[i], geometry_type(CC[i]));
    OstreamComplex2DFmt OG2(outfile,1);
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
     OstreamComplex2DFmt OG(casei_out,1);
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

  

  test_sequence_iterator<gt::Vertex>(CC[1], testout);
  test_sequence_iterator<gt::Edge>  (CC[1], testout);
  test_sequence_iterator<gt::Cell>  (CC[1], testout);

  test_incidence_iterator<gt::Vertex, gt::Cell>(CC[1], testout);
  test_incidence_iterator<gt::Edge,   gt::Cell>(CC[1], testout);
  test_incidence_iterator<gt::Cell,   gt::Cell>(CC[1], testout);

  test_incidence_iterator<gt::Vertex, gt::Edge>(CC[1], testout);

  test_archetypes(CC[0], testout);
  

  return 0;
}
