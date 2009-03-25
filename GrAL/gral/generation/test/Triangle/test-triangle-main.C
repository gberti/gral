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
#include "Gral/Generation/Triangle/triangle-generator.h"
#include "Gral/Generation/Triangle/triangle-adapter.h"

// GrAL Complex2D
#include "Gral/Grids/Complex2D/complex2d.h"
#include "Gral/Grids/Complex2D/construct.h"
#include "Gral/Grids/Complex2D/grid-functions.h"

// Gral Base
#include "Gral/IO/dx-format-output2d.h"

// Utility
#include "Utility/as-string.h"

// STD
#include <fstream>
#include <iostream>
#include <algorithm>

using namespace GrAL;

#define REAL double
#include <triangle.h>

void test_simple_input() {
  typedef Complex2D grid_type;
  typedef grid_types<grid_type> gt;
  
  triangle_generator::TriangleGenerator generator;
  triangle_generator::TriangleInput input;
  triangle_adapter::TriangleAdapter triangulation;

  input.addVertex(-1.0, -1.0);
  input.addVertex(1.0, -1.0);
  input.addVertex(1.0, 1.0);
  input.addVertex(-2.0, 5.0);
  input.addVertex(-3.0, 4.0);

  generator.Triangulate(input, "pczAevna0.4", triangulation);
  
  Complex2D T;
  stored_geometry_complex2D GeomT(T);
  ConstructGrid(T, GeomT, triangulation, triangulation);

  OstreamDX2DFmt Out("simple_input.out");
  ConstructGrid(Out,T,GeomT);
}

void test_two_regions() {
  typedef Complex2D grid_type;
  typedef grid_types<grid_type> gt;
  
  triangle_generator::TriangleGenerator generator;
  triangle_generator::TriangleInput input;
  triangle_adapter::TriangleAdapter triangulation;

  input.addVertex(-1.0, -1.0);
  input.addVertex(1.0, -1.0);
  input.addVertex(1.0, 1.0);
  input.addVertex(-1.0, 1.0);

  input.addSegment(0,1);
  input.addSegment(1,2);
  input.addSegment(2,3);
  input.addSegment(3,0);
  input.addSegment(0,2);

  input.addRegion(0.8,0.2,1.0,0.04);
  input.addRegion(0.2,0.8,2.0,0.02);

  generator.Triangulate(input, "qpcza", triangulation);
  
  Complex2D T;
  stored_geometry_complex2D GeomT(T);
  ConstructGrid(T, GeomT, triangulation, triangulation);

  OstreamDX2DFmt Out("two_regions.out");
  ConstructGrid(Out,T,GeomT);
}


int main() {
  
 test_simple_input();
 test_two_regions();

  return 0;
}
