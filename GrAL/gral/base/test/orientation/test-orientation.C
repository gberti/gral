/*! \file
 */

#include "Gral/Algorithms/orientation.h"
#include "Gral/Grids/Complex2D/all.h"
#include "Gral/IO/complex2d-format-input.h"

#include <iostream>
#include <string>

void test_grid(std::string const& gfile) 
{
  typedef GrAL::Complex2D             grid_type;
  typedef GrAL::grid_types<grid_type> gt;
 
  std::cout << "Testing grid \"" << gfile << "\" ...\n";
  GrAL::IstreamComplex2DFmt In(gfile);
  grid_type G;
  GrAL::ConstructGrid0(G,In);
  std::cout << G.NumOfCells() << " cells, " << G.NumOfVertices() << " vertices." << std::endl;
  GrAL::grid_function<gt::Cell, int> orientation(G);
  bool orientable = GrAL::get_orientation(G,orientation);
  std::cout << "Grid is " << (orientable ? "" : "not ") << "orientable\n";
  if(orientable)
    for(gt::CellIterator c(G); !c.IsDone(); ++c)
      std::cout << "Or(cell " << c.handle() << ") = " << orientation(*c) << "\n";
}

int main() {
  
  std::string files[] = 
    { "2triangles.complex2d", 
      "2triangles-wrongorient.complex2d",
      "moebius.complex2d",
      "cylinder.complex2d",
      "4triangles-2components.complex2d",
      "4triangles-wrongorient-2components.complex2d"
    };

  for(int i = 0; i < sizeof(files)/sizeof(std::string); ++i)
    test_grid(files[i]);
}
