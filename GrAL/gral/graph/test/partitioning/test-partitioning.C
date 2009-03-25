/*! \file
 */


/* ------------------------------------------------------------

    Copyright (C) 2001 - 2009 Guntram Berti,
    Contact: guntram AT users DOT berlios DOT de

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */

#include "Gral/Graph/min-cut.h"
#include "Gral/Grids/Cartesian2D/all.h"
#include "Gral/Subranges/enumerated-subrange.h"

#include "Gral/IO/gmv-format-output2d.h"

#include "Container/tuple-point-traits.h"
#include "IO/control-device.h"

#include <cmath>
#include <iostream>

class my_mapping {
public:
  typedef GrAL::tuple<double,2> coord_type;
  typedef coord_type argument_type;
  typedef coord_type result_type;
  typedef GrAL::point_traits<coord_type> pt;
  typedef pt::component_type real_type;

private:
  int n;
public:
  my_mapping(int nn = 1) : n(nn) {}

  coord_type operator()(coord_type p) const { 
    real_type x = pt::x(p), y = pt::y(p);
    // return coord_type(x, y * (1 +(x-0.5)*(x-0.5))/1.25);
    double scale_y = sin(x * n * M_PI);
    return coord_type(x, y * (1 - 0.25*scale_y *scale_y));
  }
};

int main(int argc, char* argv[])
{ 
 
  GrAL::ControlDevice Ctrl =
    GrAL::GetCommandlineAndFileControlDevice(argc,argv,"","main");
  int nx = 7, ny = 7;
  GrAL::RegisterAt(Ctrl, "-nx", nx);
  GrAL::RegisterAt(Ctrl, "-ny", ny);
  int nparts = 2;
  GrAL::RegisterAt(Ctrl, "-np", nparts);
  Ctrl.update();
 
   {
     typedef my_mapping                                        mapping_type;
     typedef GrAL::cartesian2d::CartesianGrid2D                grid_type;
     typedef GrAL::cartesian2d::mapped_geometry<mapping_type>  geom_type;
     typedef GrAL::grid_types<grid_type>                       gt;

     grid_type    R(nx,ny);
     mapping_type f(nparts-1);
     geom_type    GeomR(R, f);
 
     GrAL::grid_function<gt::Facet, int> cutweights(R,1);
     double min_length = GeomR.length(*R.FirstEdge());
    for(gt::EdgeIterator e(R); !e.IsDone(); ++e)
      min_length = std::min(min_length, GeomR.length(*e));

    for(gt::EdgeIterator e(R); !e.IsDone(); ++e)
      cutweights[*e] = round(10000*GeomR.length(*e)/min_length);

    /*
     for(gt::FacetIterator f(R); !f.IsDone(); ++f)
       cutweights[*f] = GeomR.volume(*f);
    */

     GrAL::grid_function<gt::Cell, int>             partition(R,0);
     // typedef GrAL::enumerated_cell_range<grid_type> range_type;
     typedef std::vector<gt::Cell> range_type;
     std::vector<range_type> seeds(nparts);
     int delta = (R.urx()-1) / (nparts-1);
     for(int i = 0; i < nparts; ++i) {
       std::cout << "seeds at x=" << i*delta << "\n";
       for(int y = 0; y < R.ury(); ++y) {
	 seeds[i].push_back(R.cell(i*delta, y));
	 // seeds[0].push_back(R.cell(        0,y)); 
	 // seeds[1].push_back(R.cell(R.urx()-1,y)); 
       }
     }
     try {
       int cuts = GrAL::graph::min_cut_kway_partition(R, seeds, cutweights, partition);
     }
     catch(std::exception & e) {
       std::cerr << "Exception caught: " << e.what()  << std::endl;
     }
     catch(...) {
       std::cerr << "Exception caught!" << std::endl;
     }
     std::string gridoutfile("grid.gmv");
     GrAL::OstreamGMV2DFmt Out(gridoutfile);
     GrAL::ConstructGrid_GF(Out,R,GeomR,
			    (Out.BeginGFs(), Out.pair("part", partition)));
  } 
}
