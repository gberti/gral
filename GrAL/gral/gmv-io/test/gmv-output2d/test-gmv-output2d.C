/*! \file 

    Test and demonstrate output to GMV format (2D)

 */

/* ------------------------------------------------------------

    Copyright (C) 2001 - 2009 Guntram Berti,
    Contact: guntram AT users DOT berlios DOT de

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */

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
  using namespace GrAL;
  namespace c2d = cartesian2d;

  typedef c2d::CartesianGrid2D               grid_type;
  typedef grid_types<grid_type>              gt;
  typedef coordN<2>                          coord_type;
  typedef point_traits<coord_type>           pt;
  typedef Id<coord_type>                     mapping_type;
  typedef c2d::mapped_geometry<mapping_type> geometry_type;

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
    partial_grid_function<gt::Vertex, int> gf2(R,2);
    OstreamGMV2DFmt Out("3x3.out");

    // Try the  experimental version of passing an abritrary list
    // of grid functions to GMV
    namespace hl = heterogeneous_list;
    hl::BEGIN B;
    ConstructGrid_GF(Out,R,GeomR,
		     (B, Out.pair("gf1",gf1), Out.pair("gf2",gf2)));
    // the construct (B, Out.pair(name, gf), ...) uses an overloaded
    // operator, (comma-operator) to create a list of (name,grid-function)
    // pairs to pass to GMV.
  } 

  {
    grid_type R(3,3);
    geometry_type GeomR(R, mapping_type());
    grid_function<gt::Cell, int> mat(R,0);
    for(gt::CellIterator c(R); !c.IsDone(); ++c)
      mat[*c] = (*c).index()[0];

    partial_grid_function<gt::Vertex, double> gf2(R,0);
    for(gt::VertexIterator v(R); !v.IsDone(); ++v)
      gf2[*v] = pt::x(GeomR.coord(*v)) + 0.25 * pt::y(GeomR.coord(*v));

    OstreamGMV2DFmt Out("3x3-mat.out");
    Out.output_materials();
    // Try the  experimental version of passing an abritrary list
    // of grid functions to GMV
    namespace hl = heterogeneous_list;
    hl::BEGIN B;
    ConstructGrid_GF(Out,R,GeomR,
		     (B, Out.pair("material",mat), Out.pair("gf2",gf2)));
    // the construct (B, Out.pair(name, gf), ...) uses an overloaded
    // operator, (comma-operator) to create a list of (name,grid-function)
    // pairs to pass to GMV.
  } 



}
