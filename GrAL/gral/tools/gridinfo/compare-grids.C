
/* ------------------------------------------------------------

    Copyright (C) 2001 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */

#include <Gral/Measurement/compare-grids.h>

#include <Gral/IO/complex2d-format.h>
#include <Gral/Grids/Complex2D/all.h>

#include "Gral/Geometries/bounding-box.h"
#include <Gral/Geometries/simple-geometry.h>

#include <Geometry/box.h>
#include <Geometry/algebraic-primitives.h>
#include <Container/tuple-point-traits.h>

#include <IO/control-device.h>

#include <iostream>
#include <limits>

int main(int argc, char * argv[]) {
  using namespace GrAL;
  using namespace std;
  enum { combinatorial_difference = 1,
	 geometric_difference = 2,
	 equality = 0 };

  ControlDevice Ctrl =
    GetCommandlineAndFileControlDevice(argc,argv,"","main");

  string p = "     ";
  string command = argv[0];
  string h = command + ": Check if two grids are equal combinatorially and close geometrically\n";
  h += "Usage: " + command + " <options>\n";
  string grid_in_1, grid_in_2;
  RegisterAt(Ctrl, "-g1", grid_in_1);
  RegisterAt(Ctrl, "-g2", grid_in_2);
  h += (p + "-g1 <grid 1> -g2 <grid 2> (two grid to compare)\n"); 

  double tol = 10*numeric_limits<double>::epsilon();
  RegisterAt(Ctrl, "-tol", tol);
  h += (p + "-tol <double = 10*eps> (tolerance for geometric difference)\n");
  
  AddHelp(Ctrl,h);
  Ctrl.update();

  typedef tuple<double,3>                               coord_type;
  typedef Complex2D                                     grid_type;
  typedef simple_geometry<grid_type, coord_type>        geom_type;
  typedef algebraic_primitives<coord_type>              ap;

  IstreamComplex2DFmt In1(grid_in_1);
  grid_type G1;
  geom_type Geom1(G1);
  ConstructGrid(G1,Geom1,In1,In1);

  IstreamComplex2DFmt In2(grid_in_2);
  grid_type G2;
  geom_type Geom2(G2);
  ConstructGrid(G2,Geom2,In2,In2);

  if(compare_grids(G1,G2) != grid_comparison_result::trivially_isomorphic) {
    cerr << "Grids differ combinatorially.\n";
    return combinatorial_difference;
  }
  
  double max_diff = ap::norm_2(max_vertex_distance(Geom1, Geom2));
  box<coord_type> b = get_grid_bounding_box(G1,Geom1);
  b = b | get_grid_bounding_box(G2,Geom2);
  double diam = diameter(b);
  max_diff /= diam;
  if(max_diff > tol) {
    cerr << "Grids differ geometrically, max relative vertex difference = " << max_diff << endl;
    return geometric_difference;
  }
  
  
  return equality;
}
