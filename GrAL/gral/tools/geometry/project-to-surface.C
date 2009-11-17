/*! \file Test harness for class point_locator<>
   
*/ 

/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include "Gral/Geometry/point-locator.h"

#include "Gral/IO/complex2d-format-input.h"
#include "Gral/Grids/Complex2D/all.h"
#include "Gral/Geometries/simple-geometry.h"


#include "Container/tuple-point-traits.h"
#include "IO/control-device.h"
#include "Geometry/algebraic-primitives.h"

#include <iostream>


int main(int argc, char* argv[]) {
  using namespace GrAL;
  using namespace std;

  typedef tuple<double,3>                       coord_type;
  typedef Complex2D                             grid_type;
  typedef simple_geometry<grid_type,coord_type> geom_type;
  typedef grid_types<grid_type>                 gt;

  ControlDevice Ctrl =
    GetCommandlineAndFileControlDevice(argc,argv,"","");
  string h = "project-to-surface - project query points to surface mesh points\n";
  h += "Usage:project-to-surface <OPTIONS>\n";
  string p = "   ";

  string grid_in;
  RegisterAt(Ctrl, "-g", grid_in);
  RegisterAt(Ctrl, "-i", grid_in);
  RegisterAt(Ctrl, "-in", grid_in);
  h += p + "-i|-in|-g <grid> (input surface grid in complex2d format)\n";

  double el = 0.1;
  RegisterAt(Ctrl, "-e", el);
  h += p + "-e <edge length> (edge length of background bucket grid)\n";

  double halo = 0.2;
  RegisterAt(Ctrl, "-halo", halo);
  h += p + "-halo <halo size = 0.2> (relative size of halo around input mesh resolved by\n"
    +  p + "    background bucket grid. 0.2 means 20% bigger than bbox.)\n";
 

  std::vector<coord_type> points;
  Ctrl.add("+p", GetPushbackMutator(points));
  h += p + "+p <query point> (point to be mapped to surface, can be repeated)\n";

  AddHelp(Ctrl, h);

  Ctrl.update(); 

  IstreamComplex2DFmt In(grid_in);
  cerr << "Reading grid from file " << grid_in << " ... " << flush;
  grid_type G;
  geom_type GeomG(G);
  ConstructGrid(G, GeomG, In, In);
  cerr << "done." << endl;

  typedef  point_locator<grid_type, geom_type, gt> locator_type;
  typedef locator_type::location_result_type       location_result_type; 
  typedef locator_type::bucket_vertex              bucket_vertex;

  point_locator<grid_type, geom_type, gt> Locator(G,GeomG);
  Locator.set_edge_length(el);
  Locator.set_halo_size(halo);
  Locator.init();
	    
  cout << "\n" <<"Bucket grid: ["
       << Locator.TheBucketGrid()->low_vertex_index() << " @ "
       << Locator.TheBucketGeometry()->coord(bucket_vertex(Locator.TheBucketGrid(),
							   Locator.TheBucketGrid()->low_vertex_index())) 
       << ","
       << Locator.TheBucketGrid()->high_vertex_index() << " @ "
       << Locator.TheBucketGeometry()->coord(bucket_vertex(Locator.TheBucketGrid(),
							   Locator.TheBucketGrid()->high_vertex_index()))
       << "]" << endl;

  // check 
  coord_type perturbation = coord_type(2*el,2*el,2*el);
  for(gt::VertexIterator v = GrAL::begin_x(G); !v.IsDone(); ++v) {
    points.push_back(GeomG.coord(*v) + perturbation);
  }


  for(int p = 0; p < points.size(); ++p) {
    location_result_type loc = Locator.locate(points[p]);
    cout << points[p] << " -> " << loc.TheCoord() 
	 << " (" << loc.tagname() << " in cell " << loc.TheCell().handle() << ") "; 
    // slow checking - find nearest vertex
    typedef algebraic_primitives<coord_type> ap;
    gt::Vertex nearest_vertex = * G.FirstVertex();
    double min_dist = ap::distance(GeomG.coord(nearest_vertex), points[p]);
    for(gt::VertexIterator v = GrAL::begin_x(G); !v.IsDone(); ++v) {
      double dist =  ap::distance(GeomG.coord(*v), points[p]);
      if(dist < min_dist) {
	min_dist  = dist;
	nearest_vertex = *v;
      }
    }
    cout << "Nearest vertex: " << nearest_vertex.handle() 
	 << " @ " << GeomG.coord(nearest_vertex)
	 << " Error = " << ap::distance(loc.TheCoord(), loc.TheCoord())
	 << "\n";
  }
}
