
#include "Gral/IO/gmv-format-output.h"
#include "Gral/Grids/Cartesian3D/all.h"

#include "Geometry/coords.h"
#include <functional>

template<class COORD>
struct Id {
  COORD operator()(double x, double y, double z) const 
    { return COORD(x,y,z);}
  typedef COORD result_type;
};

int main() {
  {
    CartesianGrid3D R(2,2,2);
    typedef Id<coordN<3> > mapping_type;
    mapped_geometry_cartesian3d<mapping_type> GeomR(R, mapping_type());
    OstreamGMV3DFmt Out("2x2x2.out");
    ConstructGrid(Out,R,GeomR);
  }

  {
    CartesianGrid3D R(5,3,2);
    typedef Id<coordN<3> > mapping_type;
    mapped_geometry_cartesian3d<mapping_type> GeomR(R, mapping_type());
    OstreamGMV3DFmt Out("5x3x2.out");
    ConstructGrid(Out,R,GeomR);
  } 

}
