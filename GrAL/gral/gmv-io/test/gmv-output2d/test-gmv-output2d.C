/*! \file 

    Test and demonstrate output to GMV format (2D)

 */

#include "Gral/IO/gmv-format-output2d.h"
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
    OstreamGMV2DFmt Out("2x2.out");
    ConstructGrid(Out,R,GeomR);
  }

  {
    grid_type R(2,3);
    geometry_type GeomR(R, mapping_type());
    OstreamGMV2DFmt Out("2x3.out");
    ConstructGrid(Out,R,GeomR);
  } 

  {
    grid_type R(3,3);
    geometry_type GeomR(R, mapping_type());
    grid_function<gt::Cell,   int> gf1(R,1);
    grid_function<gt::Vertex, int> gf2(R,2);
    OstreamGMV2DFmt Out("3x3.out");
    namespace hl = heterogeneous_list;
    hl::BEGIN B;
    ConstructGrid(Out,R,GeomR,
		  (B, Out.pair("gf1",gf1), Out.pair("gf2",gf2)));
    // ConstructGrid(Out,R,GeomR, (heterogeneous_list::BEGIN, gf1, gf2));
  } 



}
