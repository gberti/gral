/*! \file

*/

#include "Gral/Hierarchical/conforming-leaf-grid.h"
#include "Gral/Grids/CartesianND/all.h"

#include <iostream>

namespace GrAL { namespace octree { namespace conforming_leaf_grid {
  typedef cartesiannd::grid<2>    grid2d_type;
  typedef grid_types<grid2d_type> gt2;
  typedef Octree<grid2d_type>     octree2d_type;
  typedef octree2d_type::hier_grid_type hier_grid2d_type;

  template class grid     <octree2d_type>;
  template class vertex_t <octree2d_type>;

  template class element_t<octree2d_type, hier_grid2d_type::hier_cell_type>;
  template class element_t<octree2d_type, hier_grid2d_type::hier_facet_type>;
  typedef element_t<octree2d_type, hier_grid2d_type::hier_cell_type>   cell2d_type;
  typedef element_t<octree2d_type, hier_grid2d_type::hier_facet_type>  facet2d_type;

  //template class grid_types<grid<octree2d_type> >;
  //template class element_traits<cell2d_type>;
  //  template class element_traits<facet2d_type>;
  typedef element_traits<facet2d_type>::element_type  facet_type;

  typedef grid_types<grid     <octree2d_type> > confgt2;
  //  template class side_iterator_t<confgt2::Cell, confgt2::Facet, gt2::FacetOnCellIterator>;

  typedef cartesiannd::grid<3>    grid3d_type;
  typedef grid_types<grid3d_type> gt3;
  typedef Octree<grid3d_type>     octree3d_type;
  typedef octree3d_type::hier_grid_type hier_grid3d_type;

  template class grid     <octree3d_type>;
  template class vertex_t <octree3d_type>;

  template class element_t<octree3d_type, hier_grid3d_type::hier_cell_type>;
  template class element_t<octree3d_type, hier_grid3d_type::hier_facet_type>;

  typedef grid_types<grid     <octree3d_type> > confgt3;

}}}



template<class LG>
void test_leaf_grid2d(LG const& L) {
  using namespace std;

  typedef GrAL::grid_types<LG> lgt;

  cout << "Unrefined:\n";
  for(typename lgt::CellIterator c(L); !c.IsDone(); ++c) {
    cout << "Cell " << "L" << (*c).level() << "," << (*c).Base().Flat().index()  << ", "
	 << "Facets:\n";
    for(typename lgt::FacetOnCellIterator fc(*c); !fc.IsDone(); ++fc) {
      typename lgt::Facet f(*fc);
      cout << "L"  << f.level() 
	   << " [" << f.Base().Flat().low_vertex_index() 
	   << ","  << f.Base().Flat().high_vertex_index() << "]\n";
    }
  }

}



template<class LG>
void test_leaf_grid3d(LG const& L) {
  using namespace std;

  typedef GrAL::grid_types<LG> lgt;

  cout << "Unrefined:\n";
  for(typename lgt::CellIterator c(L); !c.IsDone(); ++c) {
    cout << "Cell " << "L" << (*c).level() << "," << (*c).Base().Flat().index()  << ", "
	 << "Facets:\n";
    for(typename lgt::FacetOnCellIterator fc(*c); !fc.IsDone(); ++fc) {
      typename lgt::Facet f(*fc);
      cout << "  F"  << f.level() 
	   << " [" << f.Base().Flat().low_vertex_index() 
	   << ","  << f.Base().Flat().high_vertex_index() << "]  ";
      cout << "  Edges: ";
      for(typename lgt::EdgeOnFacetIterator efc(*fc); !efc.IsDone(); ++efc) {
	typename lgt::Edge e(*efc);
	cout << " E" << e.level()
	     << "[" << e.Base().Flat().low_vertex_index() 
	     << "," << e.Base().Flat().high_vertex_index() << "] ";
      }
      cout << "\n";
    }
  }
}

int main() {
  using namespace std;

  namespace clg = GrAL::octree::conforming_leaf_grid;
  
  {
    clg::grid2d_type   root(3,3);
    clg::grid2d_type   ref_pattern(3,3);
    clg::octree2d_type oct(root, ref_pattern);
    oct.activate(oct.coarsest_level());
    
    typedef clg::grid<clg::octree2d_type>          leaf_grid_type;
    typedef GrAL::grid_types<leaf_grid_type>  lgt;
    
    leaf_grid_type L(oct);
    
    test_leaf_grid2d(L);
    
    lgt::Cell c0 = *L.FirstCell();
    cout << "Splitting cell " << c0.level() << "," << c0.Flat().index() << "\n";
    oct.split_cell(c0.Base());
    
    test_leaf_grid2d(L);

    //   GrAL::grid_function<lgt::Vertex, lgt::vertex_handle> gf(L);
    GrAL::grid_function<lgt::Cell, lgt::cell_handle> gfc(L);
    for(lgt::CellIterator c(L); !c.IsDone(); ++c)
      gfc[*c] = c.handle();
    for(lgt::CellIterator c(L); !c.IsDone(); ++c)
      REQUIRE_ALWAYS(gfc(*c) == c.handle(), "", 1);
   
    GrAL::grid_function<lgt::Facet, lgt::facet_handle> gff(L);
    for(lgt::FacetIterator f(L); !f.IsDone(); ++f)
       gff[*f] = f.handle();
    for(lgt::FacetIterator f(L); !f.IsDone(); ++f)
      REQUIRE_ALWAYS(gff(*f) == f.handle(), "", 1);

    
 }

  // 3D
 {
   clg::grid3d_type   root(3,3,3);
   clg::grid3d_type   ref_pattern(3,3,3);
   clg::octree3d_type oct(root, ref_pattern);
   oct.activate(oct.coarsest_level());
   
   typedef clg::grid<clg::octree3d_type>          leaf_grid_type;
   typedef GrAL::grid_types<leaf_grid_type>  lgt;
   
   leaf_grid_type L(oct);
    
   test_leaf_grid3d(L);
    
   lgt::Cell c0 = *L.FirstCell();
   cout << "Splitting cell " << c0.level() << "," << c0.Flat().index() << "\n";
   oct.split_cell(c0.Base());
   
   test_leaf_grid3d(L);

   GrAL::grid_function<lgt::Cell, lgt::cell_handle> gfc(L);
   for(lgt::CellIterator c(L); !c.IsDone(); ++c)
     gfc[*c] = c.handle();
   for(lgt::CellIterator c(L); !c.IsDone(); ++c)
      REQUIRE_ALWAYS(gfc(*c) == c.handle(), "", 1);
    
    GrAL::grid_function<lgt::Facet, lgt::facet_handle> gff(L);
    for(lgt::FacetIterator f(L); !f.IsDone(); ++f)
       gff[*f] = f.handle();
    for(lgt::FacetIterator f(L); !f.IsDone(); ++f)
      REQUIRE_ALWAYS(gff(*f) == f.handle(), "", 1);

    GrAL::grid_function<lgt::Edge, lgt::edge_handle> gfe(L);
    for(lgt::EdgeIterator e(L); !e.IsDone(); ++e)
      gfe[*e] = e.handle();
    for(lgt::EdgeIterator e(L); !e.IsDone(); ++e)
      REQUIRE_ALWAYS(gfe(*e) == e.handle(), "", 1);

  }
}
