/*! \file
    Tests for \c hierarchical::hgrid_cartesian<>

*/

#include "Gral/Hierarchical/hierarchical-grid.h"
#include "Gral/Grids/CartesianND/all.h"
#include "Gral/Grids/Cartesian3D/all.h"
#include "Gral/Grids/Cartesian2D/all.h"

#include <iostream>

template<class GRID>
void test_hier_grid(GRID const& root,
		    GRID const& pattern,
		    std::ostream & out)
{
  typedef GRID cart_grid_type;
  typedef grid_types<cart_grid_type> cgt;
  typedef hierarchical::hgrid_cartesian<cart_grid_type> hier_grid_type;
  typedef hier_grid_type                                hgt;
  typedef typename hgt::hier_cell_type                  hier_cell_type;
  typedef typename hgt::hier_vertex_type                hier_vertex_type;
  typedef typename hgt::element_base_type               element_base_type;
  typedef hierarchical::h_vertex_on_cell_iterator_t<element_base_type> HierVertexOnCellIterator;
  typedef hgt::index_type index_type;

  hier_grid_type H(root,pattern);
  H.add_finer_level();
  H.add_coarser_level();
    
  for(typename hgt::level_handle lev = H.coarsest_level(); lev <= H.finest_level(); lev = H.next_finer_level(lev)) {
    out << "Level " << lev << ": " << H.FlatGrid(lev)->cell_size() << " cells" << std::endl;
    for(typename cgt::CellIterator c = H.FlatGrid(lev)->FirstCell(); !c.IsDone(); ++c) {
      hgt::cart_subrange_type R = H.descendants(hier_cell_type(H,*c,lev), H.finest_level());
      out << "Descendant of cell " << (*c).index() << ": " 
	  << R.NumOfCells()    << " cells: "    << "[" << R.low_cell_index()   << "," << R.high_cell_index()   << "]"
	  << "  "
	  << R.NumOfVertices() << " vertices: " << "[" << R.low_vertex_index() << "," << R.high_vertex_index() << "]"
	  << "\n";
      REQUIRE_ALWAYS( R.high_cell_index()   + index_type(1)  ==  R.beyond_cell_index(),   "", 1);
      REQUIRE_ALWAYS( R.high_vertex_index() + index_type(1)  ==  R.beyond_vertex_index(), "", 1);
    }
  }
  
  for(typename cgt::CellIterator c = H.FlatGrid(H.finest_level())->FirstCell(); !c.IsDone(); ++c) {
    hier_cell_type h(H,*c,H.finest_level());
    hier_cell_type p = H.Parent(h);
    hier_cell_type pp = H.Parent(p);
    
    out << "child: " << h.Flat().index()  << "; "
	<< "parent: " << p.Flat().index() << "; (children: ";
    for(typename hgt::CellChildIterator ch(p); ! ch.IsDone(); ++ch) {
      REQUIRE_ALWAYS(p == ch.Parent(), "", 1);
      out << ch.Flat().index() << ", ";
    }
    out << "\b\b) "
	<< "grandp: " << pp.Flat().index() << std::endl;

    for(HierVertexOnCellIterator vc(h); !vc.IsDone(); ++vc) {
      hier_vertex_type v = *vc;
      hier_cell_type   c = vc.TheCell();
      REQUIRE_ALWAYS(c == h, "", 1);
    }
  }
  out << "\n"
      << "Vertices & Coarsest Parents:" << std::endl;
  for(typename cgt::VertexIterator v = H.FlatGrid(H.finest_level())->FirstVertex(); !v.IsDone(); ++v) {
    hier_vertex_type hv(H,*v, H.finest_level());
    hier_vertex_type cv = H.CoarsestParent(hv);
    out << hv.Flat().index() << " -> " << cv.Flat().index() << ", level " << cv.level() << std::endl; 
  }
  
  H.remove_coarsest_level();
  out << "# levels: " << H.num_of_levels() << std::endl;
  H.remove_coarsest_level();
  out << "# levels: " << H.num_of_levels() << std::endl;
  H.remove_coarsest_level();
  out << "# levels: " << H.num_of_levels() << std::endl;
  try{
    H.remove_coarsest_level();
  }
  catch(...) {
    out << " Continuing. " << std::endl; 
  }
  out << "# levels: " << H.num_of_levels() << std::endl;
}

// explicit instantiation to make sure all members are compilable
namespace hierarchical { 
  // Temporarily disabled
  //  template class hgrid_cartesian<cartesian3d::CartesianGrid3D>; 
  template class hgrid_cartesian<cartesian2d::CartesianGrid2D>; 

  
  typedef  hgrid_cartesian<cartesian2d::CartesianGrid2D> hier_grid_2d;
  template h_element_base_t<hier_grid_2d>;
  typedef  h_element_base_t<hier_grid_2d> element_base_type_2d;
  template h_vertex_t<element_base_type_2d>;
  template h_cell_t  <element_base_type_2d>;
  template h_vertex_on_cell_iterator_t<element_base_type_2d>;
  
  typedef  hgrid_cartesian<cartesiannd::grid<3> > hier_grid_3d;
  template h_element_base_t<hier_grid_3d>;
  typedef  h_element_base_t<hier_grid_3d> element_base_type_3d;
  template h_vertex_t<element_base_type_3d>;
  template h_cell_t  <element_base_type_3d>;
  template h_vertex_on_cell_iterator_t<element_base_type_3d>;

}

int main() {
  using namespace std;
  namespace hier = hierarchical;

  {
    namespace cart = cartesiannd;
    typedef cart::grid<3> cart_grid_type;
    typedef grid_types<cart_grid_type> gt;
    typedef gt::index_type it;

    cart_grid_type root(it(3,3,3));
    cart_grid_type ref_pattern(it(3,3,3)); // 2x2x2 cells!
    test_hier_grid(root, ref_pattern, cout);     
  }

  cout << "----------------------------" << std::endl;
  {
    namespace cart = cartesian2d;
    typedef cart::CartesianGrid2D cart_grid_type;

    // CartesianGrid2D can be have lower index != (0,0)!
    // Then hierarchical_grid does not work any more!
    cart_grid_type root(4,4);
    cart_grid_type ref_pattern(4,4); // 3x3 cells!
    test_hier_grid(root, ref_pattern, cout);   
  }
  
  cout << "----------------------------" << std::endl;
  {
    namespace cart = cartesian2d;
    typedef cart::CartesianGrid2D cart_grid_type;

    // CartesianGrid2D can be have lower index != (0,0)!
    // Then hierarchical_grid does not work any more!
    cart_grid_type root(3,3);
    cart_grid_type ref_pattern(3,3); // 2x2 cells!
    test_hier_grid(root, ref_pattern, cout);   
  }
  
}
