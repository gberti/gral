
// $LICENSE

/*! \file
 */

#include "Gral/Grids/Cartesian2D/mapped-geometry.h"
#include "Gral/Grids/Cartesian2D/stored-geometry.h"
#include "Gral/Grids/Cartesian2D/test-cartesian-geometry2d.h"

#include "Container/functions.h"
#include "Container/tuple.h"
#include "Container/tuple-point-traits.h"


// using namespace GrAL;

namespace GrAL { namespace cartesian2d {
  typedef tuple<double,2>              coord_type;
  typedef stdext::identity<coord_type> mapping_type;
  
  template class stored_geometry<coord_type>;
} }

int main() {
  using namespace std;
  using namespace GrAL;
  using namespace GrAL::cartesian2d;
  typedef grid_types<CartesianGrid2D> gt;

  {
    cout << "Test grid 1x1 cells \n";
    CartesianGrid2D R(2,2);
    mapped_geometry<mapping_type> Geo(R);
    stored_geometry<coord_type>   GeomR(R);
    for(gt::VertexIterator v(R); !v.IsDone(); ++v)
      GeomR.coord(*v) = Geo.coord(*v);
    test_cartesian2d_geometry(GeomR, cout);
  }
  {
    cout << "Test grid 2x2 cells \n";
    CartesianGrid2D R(3,3);
    mapped_geometry<mapping_type> Geo(R);
    stored_geometry<coord_type>   GeomR(R);
    for(gt::VertexIterator v(R); !v.IsDone(); ++v)
      GeomR.coord(*v) = Geo.coord(*v);
    test_cartesian2d_geometry(GeomR, cout);
  }

}
