/*! \file
    Test harness for cartesiannd::grid<DIM>
*/

#include "Gral/Grids/CartesianND/cartesiannd.h"
#include "Gral/Test/all.h"
#include <iostream>

namespace cartesiannd {
  template grid<1>;
  template grid<2>;
  template grid<3>;
}



template<class G>
void print_grid(G const& g, std::ostream& out)
{
  for(int k = 0; k <= G::dimension; ++k)
    out << g.NumOfElements(k) << " " << k << "-elements\n";
} 

int main() {
  using namespace std;

  // test binomial coeff.
  cout << "Testing binomial coefficient:\n";
  unsigned N = 4;
  for(unsigned n = 0; n <= N; ++n) {
    cout << n << "   ";
    for(unsigned i = 0; i < N-n; ++i) cout << ' ';
    for(unsigned k = 0; k <= n; ++k)
      cout << binomial_coeff(n,k) << " ";
    cout << "\n";
  }

  cartesiannd::delta_map<2>::init_dirs();
  cout << "delta_map<2>::dirs: \n";
  cartesiannd::delta_map<2>::print(cout);
  cartesiannd::delta_map<2>::selfcheck();

  cartesiannd::delta_map<3>::init_dirs();
  cout << "delta_map<3>::dirs: \n";
  cartesiannd::delta_map<3>::print(cout);
  cartesiannd::delta_map<3>::selfcheck();

  cartesiannd::delta_map<4>::init_dirs();
  cout << "delta_map<4>::dirs: \n";
  cartesiannd::delta_map<4>::print(cout);
  cartesiannd::delta_map<4>::selfcheck();

  cartesiannd::delta_map<5>::init_dirs();
  cout << "delta_map<5>::dirs: \n";
  cartesiannd::delta_map<5>::print(cout);
  cartesiannd::delta_map<5>::selfcheck();

  cartesiannd::delta_map<6>::init_dirs();
  cout << "delta_map<6>::dirs: \n";
  cartesiannd::delta_map<6>::print(cout);
  cartesiannd::delta_map<6>::selfcheck();


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
    cout << "Grid 2x2 vertices:\n";
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
    cartesiannd::grid<3> R(it3(2,2,2));
    cout << "Grid 2x2x2 vertices:\n";
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


  
}
