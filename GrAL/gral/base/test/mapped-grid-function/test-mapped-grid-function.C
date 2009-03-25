/*! \file
 */


/* ------------------------------------------------------------

    Copyright (C) 2005 - 2009 Guntram Berti, 
    Contact: guntram AT users DOT berlios DOT de

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include "Gral/Base/mapped-grid-function.h"
#include "Gral/Grids/CartesianND/all.h"

#include <iostream>

class mymap {
public:
  typedef double result_type;
  typedef int    argument_type;

  result_type operator()(argument_type x) const { return 0.5*x;}
};

int main() {

  typedef GrAL::cartesiannd::grid<2> grid_type;
  typedef GrAL::grid_types<grid_type > gt;
  typedef gt::vertex_index_type it;
  GrAL::cartesiannd::grid<2> R(it(3,3));
  typedef GrAL::grid_function<gt::Vertex, int> gf_type;

  gf_type gf(R, 1);
  int cnt = 0;
  for(gt::VertexIterator v(R); !v.IsDone(); ++v)
    gf[*v] = ++cnt;

  typedef mymap map_type;
  map_type f;
  GrAL::mapped_grid_function<const gf_type, const map_type> mgf(gf, f);
  
  for(gt::VertexIterator v(R); !v.IsDone(); ++v)
    std::cout << gf(*v) << "->" << mgf(*v) << "\n"; 

}
