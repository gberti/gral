
// $LICENSE_NEC

/*! \file
 
*/


#include "Gral/Views/restricted-component-view.h"
#include "Gral/Grids/Cartesian2D/all.h"
#include "Gral/Geometries/cell-intersects-pred.h"

#include "Gral/IO/gmv-format-output2d.h"

#include "Geometry/geometric-shapes.h"
#include "Container/tuple-point-traits.h"
#include "Container/functions.h"



int main() {
  using namespace GrAL;
  using namespace std;
  namespace rcv  = restricted_grid_component_view;
  namespace cart = cartesian2d;
  namespace shapes = geometric_shapes;

  typedef cart::CartesianGrid2D grid_type;
  typedef tuple<float, 2>       coord_type;
  typedef stdext::identity<coord_type>        mapping_type;
  typedef cart::mapped_geometry<mapping_type> geom_type;
  typedef algebraic_primitives<coord_type>    ap;

  typedef shapes::halfspace<coord_type>          shape_type;
  typedef cell_intersects<shape_type, geom_type> pred_type;
  
  grid_type R(4,4);
  geom_type GeomR(R);
  {
    shape_type h(coord_type(1.0,1.0), 1.01);
    pred_type  p(h,GeomR);
    
    rcv::grid_view<grid_type, pred_type> component(R,p, * R.FirstCell());
    cout << "Component: " 
	 << component.NumOfVertices() << " vertices, "
	 << component.NumOfCells()    << " cells\n";
    
    OstreamGMV2DFmt Out("component-1.01.gmv.out");
    ConstructGrid(Out,component,GeomR);
  }
  {
    shape_type h(coord_type(1.0,1.0), 0.99);
    pred_type  p(h,GeomR);
    
    rcv::grid_view<grid_type, pred_type> component(R,p, * R.FirstCell());
    cout << "Component: " 
	 << component.NumOfVertices() << " vertices, "
	 << component.NumOfCells()    << " cells\n";
    
    OstreamGMV2DFmt Out("component-0.99.gmv.out");
    ConstructGrid(Out,component,GeomR);
  }
  {
    shape_type h(coord_type(1.0,1.0), 0.99);
    pred_type  p(h,GeomR);
    
    rcv::grid_view<grid_type, pred_type> component(R,p, R.FirstCell(), R.EndCell());
    cout << "Component: " 
	 << component.NumOfVertices() << " vertices, "
	 << component.NumOfCells()    << " cells\n";
    
    OstreamGMV2DFmt Out("component-0.99.gmv.out");
    ConstructGrid(Out,component,GeomR);
  }
}
