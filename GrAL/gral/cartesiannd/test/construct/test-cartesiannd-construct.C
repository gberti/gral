/*! \file
    Test harness for cartesiannd::grid<DIM>
*/


/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti,
    Contact: guntram AT users DOT berlios DOT de

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include "Gral/Grids/CartesianND/all.h"

#include "Gral/Test/all.h"
#include <iostream>

#include <boost/static_assert.hpp>

namespace GrAL { namespace cartesiannd {
  template class grid<1>;
  template class grid<2>;
  template class grid<3>;
}}


namespace GrAL { namespace grid_types_detail {
  template class get_grid_categories<GrAL::cartesiannd::grid_types_base<GrAL::cartesiannd::grid<2>, 2> >;

}
  template<class G> class grid_types_test {};
  
  template<>
  class grid_types_test<GrAL::cartesiannd::grid<2> >
    : public grid_types_detail::get_grid_categories<GrAL::cartesiannd::grid_types_base<GrAL::cartesiannd::grid<2>, 2> > {};

  typedef grid_types_test<GrAL::cartesiannd::grid<2> >::Vertex testVertex;
}

using namespace GrAL;

template<class G>
void print_grid(G const& g, std::ostream& out)
{
  for(unsigned k = 0; k <= g.dimension(); ++k)
    out << g.NumOfElements(k) << " " << k << "-elements\n";
} 




template<class G, int DIM>
struct local_vertex_access_tester {
  static void act(G const& g, std::ostream& out) {}
};

template<class G>
struct local_vertex_access_tester<G,1>  {
  static void act(G const& g, std::ostream& out) 
  {
    typedef grid_types<G> gt;
    typedef typename sequence_iterator_d<gt,1>::type ElementIterator;
    out << "Dim=1:" << std::endl;
    for(ElementIterator e(g); ! e.IsDone(); ++e) {
      typedef typename ElementIterator::local_index_type lit;
      out << "Element " <<  (*e).low_vertex_index() << "," << (*e).high_vertex_index() << "  ";
      for(int i = 0; i <= 1; ++i)
	  out << "V(" << i << ")=" << (*e).Vl(lit(i)).index() << "  " << std::flush;
      out << std::endl; 
   }
  }
};


template<class G>
struct local_vertex_access_tester<G,2>  {
  static void act(G const& g, std::ostream& out) 
  {
    local_vertex_access_tester<G,1>::act(g,out);
    typedef grid_types<G> gt;
    typedef typename sequence_iterator_d<gt,2>::type ElementIterator;
    out << "Dim=2:" << std::endl;
    for(ElementIterator e(g); ! e.IsDone(); ++e) {
      typedef typename ElementIterator::local_index_type lit;
      out << "Element " <<  (*e).low_vertex_index() << "," << (*e).high_vertex_index() << "  ";
      for(int i = 0; i <= 1; ++i)
	for(int j = 0; j <= 1; ++j)
	  out << "V(" << i << "," << j << ")=" << (*e).Vl(lit(i,j)).index() << "  " << std::flush;
      out << std::endl;
    }
  }
};

template<class G>
struct local_vertex_access_tester<G,3>  {
 static void act(G const& g, std::ostream& out) 
  {
    local_vertex_access_tester<G,2>::act(g,out);
    typedef grid_types<G> gt;
    typedef typename sequence_iterator_d<gt,3>::type ElementIterator;
    out << "Dim=3:" << std::endl;
    for(ElementIterator e(g); ! e.IsDone(); ++e) {
      typedef typename ElementIterator::local_index_type lit;
      out << "Element " <<  (*e).low_vertex_index() << "," << (*e).high_vertex_index() << "  ";
      for(int i = 0; i <= 1; ++i)
	for(int j = 0; j <= 1; ++j)
	  for(int k = 0; k <= 1; ++k)
	    out << "V(" << i << "," << j << "," << k << ")=" << (*e).Vl(lit(i,j,k)).index() << "  " << std::flush;
      out << std::endl;
    }
  }
};


template<class G>
void test_local_vertex_access(G const& g, std::ostream& out) 
{ local_vertex_access_tester<G, G::dim>::act(g,out);}


int main() {
  using namespace std;

  
  cartesiannd::delta_map<2>::init();
  cout << "delta_map<2>::dirs: \n";
  cartesiannd::delta_map<2>::print(cout);
  cartesiannd::delta_map<2>::selfcheck();
 

  cartesiannd::delta_map<3>::init();
  cout << "delta_map<3>::dirs: \n";
  cartesiannd::delta_map<3>::print(cout);
  cartesiannd::delta_map<3>::selfcheck();
  
  cartesiannd::delta_map<4>::init();
  cout << "delta_map<4>::dirs: \n";
  cartesiannd::delta_map<4>::print(cout);
  cartesiannd::delta_map<4>::selfcheck();

  cartesiannd::delta_map<5>::init();
  cout << "delta_map<5>::dirs: \n";
  cartesiannd::delta_map<5>::print_maps(cout);
  cartesiannd::delta_map<5>::selfcheck();

  cartesiannd::delta_map<6>::init();
  cout << "delta_map<6>::dirs: \n";
  cartesiannd::delta_map<6>::print_maps(cout);
  cartesiannd::delta_map<6>::selfcheck();


  
  

  typedef grid_types<cartesiannd::grid<1> > gt1;
  typedef gt1::index_type it1;

  typedef grid_types<cartesiannd::grid<2> > gt2;
  typedef gt2::index_type it2;

  typedef cartesiannd::delta_map<2> dm2;
  typedef gt2::index_type           idx2;

  idx2 i0(0,0);
  idx2 i1(0,0);
  idx2 i2(0,0);
  unsigned m0 = 0, m1 = 0, m2 = 0;
  unsigned dim = 1;

  cout << "i0=" << i0 << " i1=" << i1 << " i2=" << i2 << endl;
  dm2::switch_index(i0,m0, i1,m1, i2, m2, dim);
  cout << "i1: " << i1 << " m1: " << m1 <<  endl;
  dm2::switch_index(i0,m0, i1,m1, i2, m2, dim);
  cout << "i1: " << i1 << " m1: " << m1 <<  endl;
  cout << endl;

  i0 = idx2(1,1);
  i1 = idx2(1,0);
  m1 = 1;
  cout << "i0=" << i0 << " i1=" << i1 << " i2=" << i2 << endl;
  cout << "i1: " << i1 << " m1: " << m1 <<  endl;
  dm2::switch_index(i0,m0, i1,m1, i2, m2, dim);
  cout << "i1: " << i1 << " m1: " << m1 <<  endl;
  dm2::switch_index(i0,m0, i1,m1, i2, m2, dim);
  cout << "i1: " << i1 << " m1: " << m1 <<  endl;
  cout << endl;
  
  i0 = idx2(1,0);
  i1 = idx2(1,0);
  m1 = 1;
  cout << "i0=" << i0 << " i1=" << i1 << " i2=" << i2 << endl;
  cout << "i1: " << i1 << " m1: " << m1 <<  endl;
  dm2::switch_index(i0,m0, i1,m1, i2, m2, dim);
  cout << "i1: " << i1 << " m1: " << m1 <<  endl;
  dm2::switch_index(i0,m0, i1,m1, i2, m2, dim);
  cout << "i1: " << i1 << " m1: " << m1 <<  endl;
  cout << endl;
    
  i0 = idx2(0,1);
  i1 = idx2(0,1);
  m1 = 0;

  cout << "i0=" << i0 << " i1=" << i1 << " i2=" << i2 << endl;
  cout << "i1: " << i1 << " m1: " << m1 <<  endl;
  dm2::switch_index(i0,m0, i1,m1, i2, m2, dim);
  cout << "i1: " << i1 << " m1: " << m1 <<  endl;
  dm2::switch_index(i0,m0, i1,m1, i2, m2, dim);
  cout << "i1: " << i1 << " m1: " << m1 <<  endl;
  cout << endl;

  i2 = idx2(0,1);
  m1 = 1;
  cout << "i0=" << i0 << " i1=" << i1 << " i2=" << i2 << endl;
  cout << "i1: " << i1 << " m1: " << m1 <<  endl;
  dm2::switch_index(i0,m0, i1,m1, i2, m2, dim);
  cout << "i1: " << i1 << " m1: " << m1 <<  endl;
  dm2::switch_index(i0,m0, i1,m1, i2, m2, dim);
  cout << "i1: " << i1 << " m1: " << m1 <<  endl;
  cout << endl;


  BOOST_STATIC_ASSERT((1 == GrAL::is_base_or_same<GrAL::grid_range_category,
		                                  gt2::categories<gt2::grid_type>::type>
		       ::value));

  typedef grid_types<cartesiannd::subrange<2> > rgt2;
  BOOST_STATIC_ASSERT((1 == GrAL::is_base_or_same<GrAL::grid_range_category,
		                                  rgt2::categories<rgt2::grid_type>::type>
		       ::value));
  BOOST_STATIC_ASSERT((1 == GrAL::is_base_or_same<GrAL::grid_range_category,
		                                  rgt2::categories<cartesiannd::subrange<2> >::type>
		       ::value));

  typedef grid_types<cartesiannd::grid<3> > gt3;
  typedef gt3::index_type it3;

  typedef grid_types<cartesiannd::grid<4> > gt4;
  typedef gt4::index_type it4;

  typedef grid_types<cartesiannd::grid<5> > gt5;
  typedef gt5::index_type it5;

  typedef grid_types<cartesiannd::grid<6> > gt6;
  typedef gt6::index_type it6;

  
 
  {
    cartesiannd::grid<2> R(it2(0,0));
    cout << "Grid 0x0 vertices:\n";
    R.print(cout);
    print_grid(R, cout);
    cout << endl;

  }
  {
    cartesiannd::grid<2> R(it2(1,1));
    cout << "Grid 1x1 vertices:\n";
    R.print(cout);
    print_grid(R, cout);
    cout << endl;
  }

  {
    cartesiannd::grid<2> R(it2(2,1));
    cout << "Grid 2x1 vertices:\n";
    R.print(cout);
    print_grid(R, cout);
    cout << endl;

  }

  {
    cartesiannd::grid<2> R(it2(2,2));
    typedef grid_types<cartesiannd::grid<2> > gt;
    cout << "Grid 2x2 vertices:\n";
    R.print(cout);
    print_grid(R, cout);
    cout << endl;

    test_vertex_iterator(R, cout);
    test_edge_iterator  (R, cout);
    // test_face_iterator  (R, cout);
    test_facet_iterator (R, cout);
    test_cell_iterator  (R, cout);

    test_vertex_on_vertex_iterator(R, cout);
    //test_edge_on_vertex_iterator  (R, cout);
    test_cell_on_vertex_iterator  (R, cout);

    test_vertex_on_edge_iterator(R, cout);
    //test_cell_on_edge_iterator  (R, cout);

    test_vertex_on_cell_iterator(R, cout);
    test_edge_on_cell_iterator  (R, cout);
    test_vertex_on_edge_iterator(R, cout);

    test_archetypes(R,cout);
    {
      gt::Cell c = *R.FirstCell();
      gt::Edge e = *c.FirstEdge();
      gt::Vertex v = *e.FirstVertex();
      cout << "v=" << v.index() << " e=" << e.low_vertex_index() << " -> " << e.high_vertex_index() 
	   << " c= " << c.index() << "\n";
      R.switch_element(v,e,c);
      cout << "v=" << v.index() << " e=" << e.low_vertex_index() << " -> " << e.high_vertex_index() 
	   << " c= " << c.index() << "\n";
      R.switch_element(v,e);
      cout << "v=" << v.index() << " e=" << e.low_vertex_index() << " -> " << e.high_vertex_index() 
	   << " c= " << c.index() << "\n";
      R.switch_element(v,e,c);
      cout << "v=" << v.index() << " e=" << e.low_vertex_index() << " -> " << e.high_vertex_index() 
	   << " c= " << c.index() << "\n";
      R.switch_element(v,e);
      cout << "v=" << v.index() << " e=" << e.low_vertex_index() << " -> " << e.high_vertex_index() 
	   << " c= " << c.index() << "\n";
      R.switch_element(v,e,c);
      cout << "v=" << v.index() << " e=" << e.low_vertex_index() << " -> " << e.high_vertex_index() 
	   << " c= " << c.index() << "\n";
      R.switch_element(v,e);
      cout << "v=" << v.index() << " e=" << e.low_vertex_index() << " -> " << e.high_vertex_index() 
	   << " c= " << c.index() << "\n";

    }

    cartesiannd::grid<2>::archetype_type A = R.TheArchetype();
    typedef grid_types<cartesiannd::grid<2>::archetype_type> agt;
    agt::Cell   c(*A.FirstCell());
    agt::Vertex v(*A.FirstVertex());
    agt::Vertex v_first = v;

    test_sequence_iterator<agt::Vertex>(A, cout);
    test_sequence_iterator<agt::Cell  >(A, cout);
    test_incidence_iterator<agt::Vertex,agt::Cell  >(A, cout);

    cout << "Testing switch:\n";

    cout << "c=[" << c.Base().direction() << "," << c.Base().index() << "] ";
    cout << "v=[" << v.Base().index() << "] ";

    do {
      A.switch_vertex(v,c);
      cout  << " -> ";
      cout << "c=[" << c.Base().direction() << "," << c.Base().index() << "] ";
      cout << "v=[" << v.Base().index() << "] ";
      A.switch_cell(v,c);
      // A.switch_edge(v,c);
      cout  << " -> ";
      cout << "c=[" << c.Base().direction() << "," << c.Base().index() << "] ";
      cout << "v=[" << v.Base().index() << "] ";
    } while ( v != v_first);
    cout << endl;

    cout << "Testing low_vertex_index/high: " << endl;
    cout << "Cells: " << endl;
    for(gt::CellIterator c(R); ! c.IsDone(); ++c)
      cout << (*c).low_vertex_index() << "; " << (*c).high_vertex_index() << endl; 
    cout << "Edges: " << endl;
    for(gt::EdgeIterator c(R); ! c.IsDone(); ++c)
      cout << (*c).low_vertex_index() << "; " << (*c).high_vertex_index() << endl; 
    cout << "Vertices: " << endl;
    for(gt::VertexIterator c(R); ! c.IsDone(); ++c)
      cout << (*c).low_vertex_index() << "; " << (*c).high_vertex_index() << endl; 


    {
      typedef grid_types< cartesiannd::grid<2> > gt;
      //  typedef grid_types_detail::get_grid_categories<gt> catgt;
      typedef gt catgt;
      typedef catgt::categories<gt::VertexIterator>::type cat_VertexIterator;
      //int i = cat_VertexIterator();
      BOOST_STATIC_ASSERT((1 == GrAL::is_base_or_same<GrAL::grid_sequence_iterator_category,
			   catgt::categories<gt::VertexIterator>::type>
			   ::value));
      BOOST_STATIC_ASSERT((1 == GrAL::is_base_or_same<GrAL::grid_incidence_iterator_category,
			   catgt::categories<gt::VertexOnCellIterator>::type>
			   ::value));
      BOOST_STATIC_ASSERT((1 == GrAL::is_base_or_same<GrAL::grid_range_category,
			   catgt::categories<gt::grid_type>::type>
			   ::value));
      
      
      //    int i =  catgt::categories<gt::VertexIterator>::type();
      //  int j =  catgt::categories<gt::VertexOnCellIterator>::type();
      
      gt::VertexIterator v = begin<gt::Vertex>(R);
      gt::CellIterator   c = begin<gt::Cell  >(R);
      gt::VertexOnCellIterator vc = begin<gt::Vertex>(*c);


      typedef element<gt,vertex_type_tag>::type Vertex;
      typedef element<gt,edge_type_tag>  ::type Edge;
      typedef element<gt,cell_type_tag>  ::type Cell;
      typedef sequence_iterator<gt,vertex_type_tag> VertexIterator;
      typedef sequence_iterator<gt,edge_type_tag>   EdgeIterator;
      typedef sequence_iterator<gt,cell_type_tag>   CellIterator;
      typedef incidence_iterator<gt,vertex_type_tag, edge_type_tag>::type VertexOnEdgeIterator;
      typedef incidence_iterator<gt,vertex_type_tag, cell_type_tag>::type VertexOnCellIterator;
      typedef incidence_iterator<gt,edge_type_tag,   cell_type_tag>::type EdgeOnCellIterator;
    }

  }

  {
    cartesiannd::grid<3> R(it3(2,2,2));
    typedef grid_types<cartesiannd::grid<3> > gt;
    cout << "Grid 2x2x2 vertices:\n";
    R.print(cout);
    print_grid(R, cout);
    cout << endl;
    test_vertex_iterator(R, cout);
    test_edge_iterator  (R, cout);
    // test_face_iterator  (R, cout);
    test_facet_iterator (R, cout);
    test_cell_iterator  (R, cout);
    
    test_vertex_on_cell_iterator(R, cout);
    test_edge_on_cell_iterator  (R, cout);
    test_facet_on_cell_iterator (R, cout);
    test_vertex_on_edge_iterator(R, cout);
    // test_facet_on_edge_iterator(R, cout);
    // test_cell_on_edge_iterator(R, cout);

    test_vertex_on_facet_iterator(R,cout);
    test_edge_on_facet_iterator(R,cout);
    // test_cell_on_facet_iterator(R,cout);

    test_vertex_on_cell_iterator(R,cout);
    test_edge_on_cell_iterator(R,cout);
    test_facet_on_cell_iterator(R,cout);

    test_cell_on_cell_iterator(R,cout);

    cout << "Testing low/high_vertex_index: " << endl;
    cout << "Cells: " << endl;
    for(gt::CellIterator c(R); ! c.IsDone(); ++c)
      cout << (*c).low_vertex_index() << "; " << (*c).high_vertex_index() << endl; 
    cout << "Facets: " << endl;
    for(gt::FacetIterator c(R); ! c.IsDone(); ++c)
      cout << (*c).low_vertex_index() << "; " << (*c).high_vertex_index() << endl; 
    cout << "Edges: " << endl;
    for(gt::EdgeIterator c(R); ! c.IsDone(); ++c)
      cout << (*c).low_vertex_index() << "; " << (*c).high_vertex_index() << endl; 
    cout << "Vertices: " << endl;
    for(gt::VertexIterator c(R); ! c.IsDone(); ++c)
      cout << (*c).low_vertex_index() << "; " << (*c).high_vertex_index() << endl; 


    {
	
      gt::VertexIterator v     = GrAL::begin<gt::Vertex>(R);
      gt::VertexIterator v_end = GrAL::end  <gt::Vertex>(R);
      int                sz_v  = GrAL::size <gt::Vertex>(R);
	
      gt::EdgeIterator   e     = GrAL::begin<gt::Edge  >(R);
      gt::EdgeIterator   e_end = GrAL::end  <gt::Edge  >(R);
      int                sz_e  = GrAL::size <gt::Edge  >(R);
      
      gt::CellIterator   c     = GrAL::begin<gt::Cell  >(R);
      gt::CellIterator   c_end = GrAL::end  <gt::Cell  >(R);
      int                sz_c  = GrAL::size <gt::Cell  >(R);
      
      gt::VertexOnCellIterator vc     = GrAL::begin<gt::Vertex>(*c);
      gt::VertexOnCellIterator vc_end = GrAL::end  <gt::Vertex>(*c);
      int                      sz_vc  = GrAL::size <gt::Vertex>(*c);
      
      gt::VertexOnEdgeIterator ve     = GrAL::begin<gt::Vertex>(*e);
      gt::VertexOnEdgeIterator ve_end = GrAL::end  <gt::Vertex>(*e);
      int                      sz_ve  = GrAL::size <gt::Vertex>(*e);
	
	
      gt::VertexIterator v3 = GrAL::begin<gt::VertexIterator>(R);
      }

  }

  
  {
    cartesiannd::grid<4> R(it4(2));
    cout << "Grid 2x2x2x2 vertices:\n";
    R.print(cout);
    print_grid(R, cout);
    cout << endl;
    test_vertex_iterator(R, cout);
    test_edge_iterator  (R, cout);
    // test_face_iterator  (R, cout);
    test_facet_iterator (R, cout);
    test_cell_iterator  (R, cout);
  }

  {
    cartesiannd::grid<5> R(it5(2));
    typedef GrAL::grid_types<cartesiannd::grid<5> > gt;
    cout << "Grid 2x2x2x2x2 vertices:\n";
    R.print(cout);
    print_grid(R, cout);
    cout << endl;
    test_vertex_iterator(R, cout);
    test_edge_iterator  (R, cout);
    // test_face_iterator  (R, cout);
    test_facet_iterator (R, cout);
    test_cell_iterator  (R, cout);

    typedef GrAL::element_d          <gt,2>::type elem2;
    typedef GrAL::sequence_iterator_d<gt,2>::type elem2_iterator;
    elem2_iterator e2 = GrAL::begin<elem2>(R);

    typedef GrAL::element_d          <gt,3>::type elem3;
    typedef GrAL::sequence_iterator_d<gt,3>::type elem3_iterator;
    elem3_iterator e3 = GrAL::begin<elem3>(R);

  }

  {
    cartesiannd::grid<6> R(it6(2));
    cout << "Grid 2^6 vertices:\n";
    R.print(cout);
    print_grid(R, cout);
    cout << endl;
    test_vertex_iterator(R, cout);
    test_edge_iterator  (R, cout);
    // test_face_iterator  (R, cout);
    test_facet_iterator (R, cout);
    test_cell_iterator  (R, cout);
  }

 {
    cartesiannd::grid<2> R(it2(2,2));
    cartesiannd::subrange<2> RR (R,  R .low_vertex_index(), R .beyond_vertex_index());
    cartesiannd::subrange<2> RRR(RR, RR.low_vertex_index(), RR.beyond_vertex_index());
    
    cout << "Subrange 2x2 vertices:\n";
    RR.print(cout);
    print_grid(RR, cout);
    cout << endl;

    test_vertex_iterator(RR, cout);
    test_edge_iterator  (RR, cout);
    // test_face_iterator  (R, cout);
    test_facet_iterator (RR, cout);
    test_cell_iterator  (RR, cout);


    test_local_vertex_access(R , cout);
    test_local_vertex_access(RR, cout);
    
    typedef grid_types<cartesiannd::subrange<2> > rgt;
    rgt::VertexIterator v = begin<rgt::VertexIterator>(RR);
    // all these calls are equivalent
    rgt::CellIterator   c0  = GrAL::begin_x(RR);
    rgt::CellIterator   c1  = GrAL::begin<rgt::Cell>(RR);
    rgt::CellIterator   c2  = GrAL::begin<rgt::CellIterator>(RR);
    rgt::VertexOnCellIterator vc = GrAL::begin_x(*c0);
 }

 {
   cartesiannd::grid<2> R(it2(4,4)); // 3x3 cells
   cartesiannd::subrange<2> RR(R, it2(1,1), it2(3,3));
   typedef grid_types<cartesiannd::grid    <2> > gt;
   typedef grid_types<cartesiannd::subrange<2> > rgt;

   // test construction of grid<> elements with subrange<> elements 
   rgt::CellIterator c_sg = GrAL::begin_x(RR);
   gt ::CellIterator c_g  = c_sg;
   cout << "c_g .index()= " << c_g .index() << " c_g .handle()= " << c_g .handle() << "\n"
	<< "c_sg.index()= " << c_sg.index() << " c_sg.handle()= " << c_sg.handle() << "\n";

   // test indexing of grid_function over grid<> with subrange<> elements
   grid_function<gt::Cell,int> gf(R,0);
   gf[c_sg] = 1;

   
   rgt::VertexOnCellIterator vc_sg(c_sg);
   gt ::VertexOnCellIterator vc_g (c_g);
   for( ; ! vc_sg.IsDone(); ++vc_sg, ++vc_g)
     REQUIRE_ALWAYS(vc_sg.index() == vc_g.index(), "", 1);

   rgt::CellOnCellIterator cc_sg(c_sg);
   gt ::CellOnCellIterator cc_g (c_g);
   for( ; ! cc_sg.IsDone(); ++cc_sg, ++cc_g)
     REQUIRE_ALWAYS(cc_sg.index() == cc_g.index(), "", 1);
 }

 {
    cartesiannd::grid<2> R(it2(2,2));
    cartesiannd::subrange<2> RR(R, it2(1,1), it2(2,2));
    cout << "Subrange [1,2[x[1,2[ \n";
    RR.print(cout);
    print_grid(RR, cout);
    cout << endl;

    test_vertex_iterator(RR, cout);
    test_edge_iterator  (RR, cout);
    // test_face_iterator  (R, cout);
    test_facet_iterator (RR, cout);
    test_cell_iterator  (RR, cout);

  }

 {
    cartesiannd::grid<3> R(it3(2,2,2));
    cartesiannd::subrange<3> RR(R, R.low_vertex_index(), R.beyond_vertex_index());
    cout << "Subrange 2x2x2 vertices:\n";
    RR.print(cout);
    print_grid(RR, cout);
    cout << endl;

    test_vertex_iterator(RR, cout);
    test_edge_iterator  (RR, cout);
    // test_face_iterator  (R, cout);
    test_facet_iterator (RR, cout);
    test_cell_iterator  (RR, cout);

    test_local_vertex_access(RR, cout);
  }

 {
    cartesiannd::grid<3> R(it3(3,3,3));
    typedef grid_types<cartesiannd::grid<3> > gt; 
    cartesiannd::subrange<3> RR(R, 
				gt::Cell(R,gt::index_type(0,0,0)), 
				gt::Cell(R,gt::index_type(1,1,1)));
    cout << "Subrange 2x2x2 cells:\n";
    RR.print(cout);
    print_grid(RR, cout);
    cout << endl;

    test_vertex_iterator(RR, cout);
    test_edge_iterator  (RR, cout);
    // test_face_iterator  (R, cout);
    test_facet_iterator (RR, cout);
    test_cell_iterator  (RR, cout);

    test_local_vertex_access(RR, cout);

 }  
}
