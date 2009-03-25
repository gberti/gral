/*! \file 

     Testing non conforming leaf grids of an octree.

*/

/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti,
    Contact: guntram AT users DOT berlios DOT de

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include "Gral/Hierarchical/non-conforming-leafgrid.h"
#include "Gral/Hierarchical/octree.h"
#include "Gral/Hierarchical/hierarchical-geometry.h"

#include "Gral/Grids/Cartesian2D/all.h"
#include "Gral/Grids/Cartesian3D/all.h"
#include "Gral/Grids/CartesianND/all.h"

#include "Gral/IO/geomview-format-output.h"
#include "Gral/IO/gmv-format-output2d.h"
#include "Gral/IO/gmv-format-output3d.h"

#include "Gral/Test/test-archetypes.h"

#include "Geometry/matrix.h"
#include "Geometry/affine-mapping.h"
//#include "Container/functions.h" // identity

//#include "IO/control-devices.h"

#include <math.h>

using namespace GrAL;

template<class OCTREE>
void print_state(OCTREE const& oct, std::ostream& out) 
{
  out << "Octree state: Number of levels: " << oct.num_of_levels()  
      << " ( " << oct.coarsest_level() << " .. " << oct.finest_level() << " )" << "\n"
      << "              Number of cells:  " << oct.NumOfCells()     << "\n"
      << "              Number of leafs:  " << oct.NumOfLeafCells() << "\n";
}



// explicit instantiations
namespace GrAL {
namespace octree {
  typedef Octree<cartesiannd::grid<2> > octree2d_type;
  typedef Octree<cartesiannd::grid<3> > octree3d_type;

  //  typedef Octree<cartesian2d::CartesianGrid2D> octree2d_type;
  //  typedef Octree<cartesian3d::CartesianGrid3D> octree3d_type;

  template class non_conforming_leafgrid<octree2d_type>;
  typedef non_conforming_leafgrid<octree2d_type> leafgrid2d;
  template class non_conforming_leafgrid<octree3d_type>;
  typedef non_conforming_leafgrid<octree3d_type> leafgrid3d;

  typedef non_conforming_leafgrid<octree2d_type>::element_base_type elem_base2d_type;
  typedef non_conforming_leafgrid<octree3d_type>::element_base_type elem_base3d_type;
}}


namespace GrAL {
  namespace hierarchical {
    template class h_vertex_t<octree::elem_base2d_type>;
    template class h_vertex_t<octree::elem_base3d_type>;
    // cannot instantiate this, because CellChildIterator is not defined.
    // template class h_cell_t  <octree::elem_base2d_type>;
    template class h_incidence_iterator_t<octree::elem_base2d_type, 0, 2>;
    template class h_incidence_iterator_t<octree::elem_base3d_type, 0, 3>;
  }
  
}


double f(GrAL::tuple<double,2> X) {
  using namespace GrAL;
  typedef point_traits<tuple<double, 2> > pt;
  double x = pt::x(X), y = pt::y(X);
  return x*x+y*y -0.211;
}

double f3d(GrAL::tuple<double,3> X) {
  using namespace GrAL;
 typedef  point_traits<tuple<double, 3> > pt;
 double x = pt::x(X), y = pt::y(X), z = pt::z(X);
 return x*x+y*y+z*z -0.211;
}

double f1(GrAL::tuple<double,2> X) {
  using namespace GrAL;
  typedef  point_traits<tuple<double, 2> > pt;
  double x = pt::x(X)-0.5, y = pt::y(X)-0.5;
 
 return x*x*x + y*y*y - 0.5*x*x - 0.5*y*y + 0.1333;
}

double f2(GrAL::tuple<double,2> X) {
  using namespace GrAL;
  typedef  point_traits<tuple<double, 2> > pt;
 double x = 0.8*(pt::x(X)-0.5), y = 0.8*(pt::y(X)-0.5);
 
 return (1.0 + 0.3*sin(5.0*atan2(x,y)))*(x*x + y*y) - 0.1001;
}



int main() {
  using namespace GrAL;
   
  using namespace std; 
  {
    /*
    namespace cart = cartesian2d;
    typedef cart::CartesianGrid2D               cart_grid_type;
    typedef tuple<double,2>                     coord_type;
    */
    namespace cart = cartesiannd; // cartesian3d;
    //typedef cart::CartesianGrid3D               cart_grid_type;
    typedef cartesiannd::grid<3>                cart_grid_type;
    typedef tuple<double,3>                     coord_type;

    typedef grid_types<cart_grid_type>          cgt;
    //    typedef stdext::identity<coord_type>        mapping_type;
    typedef matrix<3,3,0> matrix_type;
    typedef affine_mapping<matrix_type, coord_type> mapping_type;
    typedef cart::mapped_geometry<cart_grid_type, mapping_type> cart_geom_type;
   

    typedef octree::Octree<cart_grid_type>               octree_type;
    typedef octree::non_conforming_leafgrid<octree_type> leafgrid_type;
    typedef octree_type::hier_grid_type     hier_grid_type;
    typedef hier_grid_type                  hgt;
    typedef hierarchical::hier_geometry<hier_grid_type, cart_geom_type> hier_geom_type;

    typedef grid_types<leafgrid_type> lgt;
  
    typedef lgt::Vertex               LeafVertex;
    typedef lgt::Cell                 LeafCell;
    typedef lgt::VertexIterator       LeafVertexIterator;
    typedef lgt::VertexOnCellIterator LeafVertexOnCellIterator;
    
    /*    
    cart_grid_type root(4,4);    // 1x1 cells
    cart_grid_type pattern(3,3); // 2x2 cells;
    */

    cart_grid_type root(6,6,6);    // 1x1 cells
    cart_grid_type pattern(3,3,3); // 2x2 cells;
    cart_geom_type root_geom(root, mapping_type::identity());
    
    octree_type    Oct  (root, pattern);
    hier_geom_type Geom (*Oct.TheHierGrid(), root_geom);

    print_state(Oct, cout);

    int maxlevels = 2;
    hgt::level_handle coarse = Oct.TheHierGrid()->coarsest_level();
    Oct.activate(Oct.coarsest_level());


    for(hgt::level_handle lev = coarse; lev <= coarse + maxlevels; ++lev) {
      cout << "Level " << lev << endl;
      for(cgt::CellIterator c(* Oct.TheHierGrid()->FlatGrid(lev)); ! c.IsDone(); ++c) {
	hgt::Cell hc(* Oct.TheHierGrid(), *c, lev);
	unsigned cnt_plus = 0;

	for(cgt::VertexOnCellIterator vc(*c); ! vc.IsDone(); ++vc) {
	  if( f3d(Geom.coord(hgt::Vertex(* Oct.TheHierGrid(),*vc, lev))) >= 0) 
	    ++cnt_plus;
	}
	
	
	if( 0 < cnt_plus && cnt_plus < hc.NumOfVertices()) {
	  if(Oct.isLeaf(hc)) {
	    cout << "Splitting cell " << hc.Flat().index() << "\n";
	    Oct.split_cell(hc);
	  }
	}
      }
      print_state(Oct, cout);
    }

    ref_ptr<octree_type> op(Oct);
    leafgrid_type L(op);

    test_archetypes(L, cout);

    /*    
    OstreamOFF2DFmt Out("leaf.off");
    ConstructGrid(Out, L, Geom);
  
    OstreamGMV2DFmt GMVOut("leaf.gmv");
    ConstructGrid(GMVOut,L,Geom);
    */

    /*    
    OstreamGMV3DFmt GMVOut("leaf3d.gmv");
    ConstructGrid(GMVOut,L,Geom);
    */
  }
  
}
