/*! \file
    Tests for \c hierarchical::hgrid_cartesian<>

*/

#include "Gral/Hierarchical/hierarchical-grid.h"
#include "Gral/Grids/CartesianND/all.h"
#include "Gral/Grids/Cartesian3D/all.h"
#include "Gral/Grids/Cartesian2D/all.h"

#include <iostream>

using namespace GrAL;

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
  typedef typename hgt::hier_facet_type                 hier_facet_type;
  typedef typename hgt::hier_vertex_type                hier_vertex_type;
  typedef typename hgt::element_base_type               element_base_type;
  typedef typename hierarchical::h_incidence_iterator_t<element_base_type, vertex_type_tag, cell_type_tag> 
                                                        HierVertexOnCellIterator;
  typedef typename hgt::index_type                      index_type;
  typedef hierarchical::h_element_t<hierarchical::h_element_base_t<hier_grid_type>, typename cgt::Facet >  hier_facet1_type;

  hier_grid_type H(root,pattern);
  H.add_finer_level();
  H.add_coarser_level();
    
  for(typename hgt::level_handle lev = H.coarsest_level(); lev <= H.finest_level(); lev = H.next_finer_level(lev)) {
    out << "Level " << lev << ": " << H.FlatGrid(lev)->cell_size() << " cells" << std::endl;
    for(typename cgt::CellIterator c = H.FlatGrid(lev)->FirstCell(); !c.IsDone(); ++c) {
      typename hgt::cart_subrange_type R = H.descendants(hier_cell_type(H,*c,lev), H.finest_level());
      out << "Descendant of cell " << (*c).index() << ": " 
	  << R.NumOfCells()    << " cells: "    << "[" << R.low_cell_index()   << "," << R.high_cell_index()   << "]"
	  << "  "
	  << R.NumOfVertices() << " vertices: " << "[" << R.low_vertex_index() << "," << R.high_vertex_index() << "]"
	  << "\n";
      REQUIRE_ALWAYS( R.high_cell_index()   + index_type(1)  ==  R.beyond_cell_index(),   "", 1);
      REQUIRE_ALWAYS( R.high_vertex_index() + index_type(1)  ==  R.beyond_vertex_index(), "", 1);
      for(typename cgt::FacetOnCellIterator fc(*c); ! fc.IsDone(); ++fc) {
	hier_facet_type hf(H, *fc, lev);
      }
    }
    for(typename cgt::FacetIterator f = H.FlatGrid(lev)->FirstFacet(); !f.IsDone(); ++f) {
      hier_facet1_type hf(H,*f,lev);
      typename hgt::cart_subrange_type R = H.descendants(hf, H.finest_level());
      
      out << "All descendants of facet " << (*f).index() << ": " 
	  << R.NumOfFacets()    << " facets: "    << "[" << R.low_cell_index()   << "," << R.high_cell_index()   << "]"
	  << "  "
	  << R.NumOfVertices() << " vertices: " << "[" << R.low_vertex_index() << "," << R.high_vertex_index() << "]"
	  << std::endl;
      if(lev < H.finest_level()) {
	out << "Children of facet " << (*f).index() << ": " << std::flush;
	typename hgt::cart_subrange_type Ch = H.children(hf);
	out << Ch.NumOfFacets()    << " facets: "    << "[" << Ch.low_cell_index()   << "," << Ch.high_cell_index()   << "]"
	    << "  " << std::flush
	    << Ch.NumOfVertices() << " vertices: " << "[" << Ch.low_vertex_index() << "," << Ch.high_vertex_index() << "]"
	    << std::endl;
	for(typename hier_facet1_type::ChildIterator chf = hf.FirstChild(); chf != hf.EndChild(); ++chf)
	  out << (*chf).level() << "," << (*chf).Flat().index() << ";  " << std::flush;
	out << std::endl;
      }
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
      hier_cell_type   c = vc.TheAnchor();
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
namespace GrAL {
namespace hierarchical { 
  // Temporarily disabled
  //  template class hgrid_cartesian<cartesian3d::CartesianGrid3D>; 
  template class hgrid_cartesian<cartesian2d::CartesianGrid2D>; 

  
  //  typedef  hgrid_cartesian<cartesian2d::CartesianGrid2D> hier_grid_2d;
  typedef  hgrid_cartesian<cartesiannd::grid<2> >   hier_grid_2d;
  template class h_element_base_t<hier_grid_2d>;
  typedef  h_element_base_t<hier_grid_2d> element_base_type_2d;
  template class h_vertex_t<element_base_type_2d>;
  template class h_cell_t  <element_base_type_2d>;
  
  typedef  hgrid_cartesian<cartesiannd::grid<3> > hier_grid_3d;
  template class h_element_base_t<hier_grid_3d>;
  typedef  h_element_base_t<hier_grid_3d> element_base_type_3d;
  template class h_vertex_t<element_base_type_3d>;
  template class h_cell_t  <element_base_type_3d>;
}}

int main() {
  using namespace GrAL;
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
    typedef cartesiannd::grid<2> cart_grid_type;
    typedef grid_types<cart_grid_type> gt;
    typedef gt::index_type             it;
    // CartesianGrid2D can be have lower index != (0,0)!
    // Then hierarchical_grid does not work any more!
    cart_grid_type root(4,4);
    cart_grid_type ref_pattern(4,4); // 3x3 cells!
    test_hier_grid(root, ref_pattern, cout);   
  }
  
  cout << "----------------------------" << std::endl;
  {
    typedef cartesiannd::grid<2> cart_grid_type;
    typedef grid_types<cart_grid_type> gt;
    typedef gt::index_type             it;

    // CartesianGrid2D can be have lower index != (0,0)!
    // Then hierarchical_grid does not work any more!
    cart_grid_type root(it(3,3));
    cart_grid_type ref_pattern(it(3,3)); // 2x2 cells!
    test_hier_grid(root, ref_pattern, cout);   
  }
  
}
