/*! \file
 */

/* ------------------------------------------------------------

    Copyright (C) 2005 - 2009 Roman Putanowicz

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


// GrAL Generation
#include "Gral/Generation/Grummp/grummp-generator.h"
#include "Gral/Generation/Grummp/grummp-adapter.h"
#include "Gral/Generation/Grummp/grummp-input.h"

// GrAL Complex2D
#include "Gral/Grids/Complex2D/complex2d.h"
#include "Gral/Grids/Complex2D/construct.h"
#include "Gral/Grids/Complex2D/grid-functions.h"

// Gral Base
#include "Gral/IO/complex2d-format-output.h"
#include "Gral/IO/dx-format-output2d.h"


// Utility
#include "Utility/as-string.h"

// STD
#include <fstream>
#include <iostream>
#include <algorithm>


int main() {
  using namespace GrAL;
  using namespace grummp_generator;

  typedef Complex2D grid_type;
  typedef grid_types<grid_type> gt;
  
  Mesh2DAdapter gadapt;
  TriGenerator generator;
  Boundary2D boundary;

  typedef Boundary2D::PointIterator PointIterator;

  boundary.addPoint(0.0, 0.0);
  boundary.addPoint(1.0, 0.0);
  boundary.addPoint(1.0, 1.0);
  boundary.addPoint(0.0, 1.0);
  boundary.addPoint(0.5, 0.5); 

  Boundary2D::Polyline p(boundary, Boundary2D::REGION, 1, 
                                   Boundary2D::BOUNDARY, 1);
  p.addPoint(0);
  p.addPoint(1);
  p.addPoint(2);
  p.addPoint(3);
  p.addPoint(0);

  Boundary2D::Circle c(boundary, Boundary2D::BOUNDARY, 2, 
                                 Boundary2D::REGION, 1, 4, 0.3); 

  boundary.addPolyline(p);
  boundary.addCircle(c); 
  
  for (PointIterator v = boundary.FirstPoint(); 
                     v != boundary.EndPoint();
                     ++v) {
     std::cerr << (*v).x() << "  " << (*v).y() << "\n"; 
  }
  for (Boundary2D::Polyline::PointHandleIterator i=p.FirstPointHandle(); 
       i != p.EndPointHandle(); ++i) {
     std::cerr << *i << "\n";
  }

  generator.triangulate(boundary, "flags", gadapt);
  
  std::cerr << gadapt.NumOfVertices() << "\n";
  std::cerr << gadapt.NumOfCells() << "\n";
/*
  GrummpGenerator mg;
  GrummpInput inp;
  double a=12;
  inp.addVertex(0.0, 0.0, 1, 1, &a);
  inp.addVertex(1.0, 0.0, 2, 1, &a);
  inp.addVertex(1.0, 10.0,3, 1, &a);
  inp.addVertex(0.0, 10.0,4, 1, &a);
  inp.addVertex(1.0, 3.0,4, 1, &a);
  mg.triangulate(inp, "pczAevn");
  //mg.report(1, 1, 1, 1, 1, 0);
  GrummpGridAdapter const &og = mg.TheGrid();
  GrummpGeometryAdapter const &geom = mg.TheGeometry();
*/
  Complex2D T;
  stored_geometry_complex2D GeomT(T);
  ConstructGrid(T, GeomT, gadapt, gadapt);
//  OstreamComplex2DFmt Out("2x2.dx");
  OstreamDX2DFmt Out("2x2.dx");

  ConstructGrid(Out,T,GeomT);

  return 0;
}
