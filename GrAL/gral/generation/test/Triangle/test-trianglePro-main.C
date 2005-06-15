/*! \file
 */

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

class SizeControl {
  public:
      SizeControl() {}
      int operator()(const double *a, const double *b, const double *c, 
                     const double area) const {
        std::cerr << "Testing triangle\n";
        if (area < 0.1) {
          return 0;
        } else {
          return 1;
        }
      }
};


void test_simple_input() {
  typedef Complex2D grid_type;
  typedef grid_types<grid_type> gt;
  
  triangle_generator::TriangleGeneratorPro<SizeControl, triangle_generator::ConstEllipseMapper> generator;
  triangle_generator::TriangleInput input;
  triangle_adapter::TriangleAdapter triangulation;
  triangle_generator::ConstEllipseMapper mapper(1, 4, sqrt(3)/2.0, 1.0/2.0);
  generator.setIncircleMap(mapper);
/*  input.addVertex(-1.0, -1.0);
  input.addVertex(1.0, -1.0);
  input.addVertex(1.0, 1.0);
  input.addVertex(-2.0, 5.0);
  input.addVertex(-3.0, 4.0);
*/
int n = 15; //  # number of poins on Lissajou knot
int nb = 10; // # number of segments on boundary
double L = 1;//    # half length of domain side
double r = 0.9;//  # ammplitude for Lissajou knot 

double b = 10;
double a = 3/4.00;
double af=0;
double bf=M_PI;
double dt = 8*M_PI/n;

//add vertices from Lissajou knot
for (int i=0; i<n; i++){
  double t = dt*i;
  input.addVertex(r*cos(a*t+af), r*sin(t));
}

double d = 2*L/nb;

// add vertices at boundary sides
for (int i=1; i<nb; i++){
  input.addVertex(-L + d*i, -L      );
  input.addVertex( L      , -L + d*i);
  input.addVertex(-L +d*i ,  L      );
  input.addVertex(-L      , -L + d*i);
}
// add vertices at corners
input.addVertex(-L, -L);
input.addVertex( L, -L);
input.addVertex( L,  L);
input.addVertex(-L,  L);

  SizeControl s;
  generator.setTriangleTestFunction(s);
  generator.triangulate(input, "TpczS0a", triangulation);
  
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

  generator.triangulate(input, "qpcza", triangulation);
  
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
