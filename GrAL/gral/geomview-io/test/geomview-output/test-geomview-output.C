/*! \file 

    Test and demonstrate output to Geomview OFF format 

 */

#include "Gral/IO/geomview-format-output.h"
#include "Gral/Grids/Cartesian2D/all.h"

#include "Geometry/coords.h"
#include <functional>

template<class COORD>
struct Id {
  typedef COORD coord_type;
  typedef COORD argument_type;
  typedef COORD result_type;

  COORD operator()(COORD const& p) const
    { return p;}
  COORD operator()(double x, double y, double z) const 
    { return COORD(x,y,z);}
};

int main() {

  typedef CartesianGrid2D grid_type;
  typedef grid_types<grid_type> gt;
  typedef Id<coordN<2> > mapping_type;
  typedef mapped_geometry_cartesian2d<mapping_type> geometry_type;

  {
    grid_type R(2,2);
    geometry_type GeomR(R, mapping_type());
    OstreamOFF2DFmt Out("2x2.out");
    ConstructGrid(Out,R,GeomR);
  }

  {
    grid_type R(2,3);
    geometry_type GeomR(R, mapping_type());
    OstreamOFF2DFmt Out("2x3.out");
    ConstructGrid(Out,R,GeomR);
  } 

}
