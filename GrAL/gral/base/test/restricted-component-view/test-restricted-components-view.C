
// $LICENSE_NEC

/*! \file
 
*/


#include "Gral/Views/restricted-component-view.h"
#include "Gral/Grids/Cartesian2D/all.h"
#include "Gral/Geometries/cell-intersects-pred.h"

#include "Gral/IO/gmv-format-output2d.h"
#include "Gral/Test/all.h"

#include "Geometry/geometric-shapes.h"
#include "Container/tuple-point-traits.h"
#include "Container/functions.h"

#include <algorithm>

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
  typedef rcv::grid_view<grid_type, pred_type>   component_type;
  
  grid_type R(4,4);
  geom_type GeomR(R);
  {
    shape_type h(coord_type(1.0,1.0), 1.01);
    pred_type  p(h,GeomR);
    component_type component(R,p, * R.FirstCell());
    typedef grid_types<component_type> cgt;

    {
      component_type comp(R,p, *R.FirstCell());
      cgt::size_type nv  = std::distance(comp.FirstVertex(), comp.EndVertex());
      cgt::size_type nv1 = comp.NumOfVertices();
      REQUIRE_ALWAYS(nv == nv1, "NumOfVertices() error!",1);
    }
    {
      component_type comp(R,p, *R.FirstCell());
      cgt::size_type nc  = std::distance(comp.FirstCell(), comp.EndCell());
      cgt::size_type nc1 = comp.NumOfCells();
      REQUIRE_ALWAYS(nc == nc1, "NumOfCells() error!",1);
    }

    cgt::vertex_handle vh = component.FirstVertex()->handle();
    cgt::cell_handle   ch = component.FirstCell()  ->handle();

    cout << "Component: " 
	 << component.NumOfVertices() << " vertices, "
	 << component.NumOfCells()    << " cells\n";

    test_sequence_iterator<cgt::Vertex>(component, cout);
    test_sequence_iterator<cgt::Edge  >(component, cout);
    test_sequence_iterator<cgt::Cell  >(component, cout);

    test_incidence_iterator<cgt::Vertex,cgt::Edge>(component, cout);
    test_incidence_iterator<cgt::Vertex,cgt::Cell>(component, cout);
    test_incidence_iterator<cgt::Edge,  cgt::Cell>(component, cout);
    //    test_incidence_iterator<cgt::Cell,  cgt::Cell>(component, cout);

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

    cout << "checkgt<grid_types<rcv::grid_view<grid_type, pred_type> > >\n";
    checkgt<grid_types<rcv::grid_view<grid_type, pred_type> > >(cout);
  }
}
