/*! \file 
 */

/* ------------------------------------------------------------

    Copyright (C) 2008 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include "Gral/Geometry/vertex-normals.h"
#include "Gral/Geometries/simple-geometry.h"
#include "Gral/Grids/Complex2D/all.h"
#include "Gral/IO/complex2d-format.h"

#include "Container/tuple-point-traits.h"

#include <iostream>

void test_normals(std::string file)
{

  typedef GrAL::tuple<double,3>                        coord_type;
  typedef GrAL::Complex2D                              grid_type;
  typedef GrAL::simple_geometry<grid_type, coord_type> geom_type;
  typedef GrAL::grid_types<grid_type>                  gt;
  
  {
    grid_type G;
    geom_type GeomG(G);
    GrAL::IstreamComplex2DFmt IG(file);
    ConstructGrid(G, GeomG, IG, IG);
    
    GrAL::grid_function<gt::Vertex, coord_type> vertex_normals(G, coord_type(0,0,0));
    GrAL::compute_outer_vertex_normals(G, GeomG, vertex_normals);
    std::cout << "Outer vertex normals for grid \"" << file << "\":\n";
    for(gt::VertexIterator v = begin_x(G); !v.IsDone(); ++v)
      std::cout << "Vertex " << v.handle() << " @ " << GeomG.coord(*v) << " has normal " << vertex_normals(*v) << "\n";
    std::cout << "\n";
  }
}

int main(int argc, char* argv[]) 
{
  test_normals("unit-tet.complex2d");
  test_normals("unit-tet-neg.complex2d");
  test_normals("unit-cube.complex2d");
  test_normals("unit-cube-centered.complex2d");
  test_normals("unit-cube-1st-octant.complex2d");
  test_normals("unit-cube-not-oriented.complex2d");

  // the current implementation does not give
  // satisfying results for this example.
  // ideally it should be the same as the above examples.
  test_normals("unit-cube-triangulated.complex2d");

  for(int i = 1; i < argc; ++i) {
    test_normals(argv[i]);
  }
}


