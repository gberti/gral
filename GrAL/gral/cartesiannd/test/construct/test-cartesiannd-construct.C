/*! \file
    Test harness for cartesiannd::grid<DIM>
*/

#include "Gral/Grids/CartesianND/cartesiannd.h"
#include "Gral/Test/all.h"
#include <iostream>



namespace GrAL { namespace cartesiannd {
  template class grid<1>;
  template class grid<2>;
  template class grid<3>;
}}


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
    typedef typename gt::template sequence_iterator_d<1>::type ElementIterator;
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
    typedef typename gt::template sequence_iterator_d<2> s2;
    typedef typename gt::template sequence_iterator_d<2>::type ElementIterator;
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
    typedef typename gt::template sequence_iterator_d<3>::type ElementIterator;
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

  /*
  cartesiannd::delta_map<2>::init();
  cout << "delta_map<2>::dirs: \n";
  cartesiannd::delta_map<2>::print(cout);
  cartesiannd::delta_map<2>::selfcheck();
  */

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

    test_vertex_on_cell_iterator(R, cout);
    test_edge_on_cell_iterator  (R, cout);
    test_vertex_on_edge_iterator(R, cout);

    test_archetypes(R,cout);

    cartesiannd::grid<2>::archetype_type A = R.TheArchetype();
    typedef grid_types<cartesiannd::grid<2>::archetype_type> agt;
    agt::Cell   c(*A.FirstCell());
    agt::Vertex v(*A.FirstVertex());
    agt::Vertex v_first = v;

    cout << "Testing switch:\n";

    cout << "c=[" << c.direction() << "," << c.index() << "] ";
    cout << "v=[" << v.index() << "] ";

    do {
      A.switch_vertex(v,c);
      cout  << " -> ";
      cout << "c=[" << c.direction() << "," << c.index() << "] ";
      cout << "v=[" << v.index() << "] ";
      A.switch_cell(v,c);
      cout  << " -> ";
      cout << "c=[" << c.direction() << "," << c.index() << "] ";
      cout << "v=[" << v.index() << "] ";
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
    cout << "Grid 2x2x2x2x2 vertices:\n";
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
    cartesiannd::subrange<2> RR(R, R.low_vertex_index(), R.beyond_vertex_index());
    cout << "Subrange 2x2 vertices:\n";
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
