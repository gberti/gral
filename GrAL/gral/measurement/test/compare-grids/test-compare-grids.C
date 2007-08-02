/*! \file

*/

#include "Gral/Measurement/compare-grids.h"
#include "Gral/Grids/CartesianND/all.h"
#include "Gral/Grids/CartesianND/mapped-geometry.h"

#include "Container/tuple-point-traits.h"
#include "Geometry/affine-mapping.h"
#include "Geometry/matrix.h"

#include <iostream>

int main() {
  using namespace std;

  typedef GrAL::cartesiannd::grid<2> grid2d_type;
  grid2d_type R1(2,2);
  grid2d_type R2(2,2);
  
  typedef GrAL::tuple<double,2> coord2d_type;
  typedef GrAL::matrix<2,2,0> matrix2d_type;
  typedef GrAL::affine_mapping<matrix2d_type, coord2d_type> mapping2d_type;
  typedef GrAL::cartesiannd::mapped_geometry<grid2d_type, mapping2d_type> geom2d_type;
  geom2d_type GeomR1(R1);
  geom2d_type GeomR2(R2);
  
  int res = GrAL::compare_grids(R1, R2);
  REQUIRE_ALWAYS(res == GrAL::grid_comparison_result::trivially_isomorphic,"", 1);

  coord2d_type d = GrAL::max_vertex_distance(GeomR1, GeomR2);
  cout << "Max difference: " << d << "\n";
}
