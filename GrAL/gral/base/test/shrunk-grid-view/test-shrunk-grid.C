#include "Gral/Views/shrunk-grid-view.h"

#include "Gral/Grids/Complex3D/all.h"
#include "Gral/Grids/Cartesian3D/all.h"
#include "Gral/Geometries/simple-geometry.h"

#include "Gral/IO/gmv-format-output3d.h"

#include "Gral/Test/all.h"




int main() {
 
  // create Cartesian grid & geometry
  typedef CartesianGrid3D reg_grid_type; 
  typedef coordN<3> coord_type;
  typedef std::identity<coord_type>  mapping_type;
  typedef mapped_geometry_cartesian3d<mapping_type> reg_geom_type;

  reg_grid_type R(3,2,2);
  mapping_type  id; 
  reg_geom_type GeomR(R, id);


  // create disjoint union view of above grids and geometries
  namespace sgv = shrink_grid_view;
  typedef sgv::grid_view<reg_grid_type>       shrunk_grid_type;
  sgv::grid_view<reg_grid_type>               ShrunkG(R);
  sgv::geom_view<reg_grid_type,reg_geom_type> ShrunkGeom(ShrunkG, GeomR);
  ShrunkGeom.set_shrink_factor(0.5);


  // test functionality of union view
  test_vertex_iterator        (ShrunkG,cout);
  test_cell_iterator          (ShrunkG,cout);
  test_vertex_on_cell_iterator(ShrunkG,cout);


  // copy ShrunkG into unstructured grid
  
  Complex3D UG;
  simple_geometry<Complex3D, coord_type> GeomUG; //(UG);
  ConstructGrid(UG, GeomUG, ShrunkG, ShrunkGeom);
  
  // output ShrunkG to files

  {
    OstreamGMV3DFmt Out("shrunk1.gmv.out");
    ConstructGrid(Out, UG, GeomUG);
  }

  {
    OstreamGMV3DFmt Out2("shrunk2.gmv.out");
    ConstructGrid(Out2, ShrunkG, ShrunkGeom);
  }
}
