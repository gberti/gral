#include "Gral/Grids/CartesianND/all.h"
#include "Gral/Grids/ComplexND/all.h"
#include "Gral/Grids/Complex2D/all.h"
#include "Gral/Grids/Complex3D/all.h"

#include "Gral/Base/grid-morphism.h"
#include "Gral/Base/partial-grid-morphism.h"

#include "Gral/Test/all.h"

#include "Gral/IO/gmv-format-output2d.h"

#include <iostream>
#include <vector>

template<class GRID>
void print_grid(GRID const& G, std::ostream& out) 
{
  out << "# Vertices: " << G.NumOfVertices () 
      << "  "
    /*     << "# Edges: "    << G.NumOfEdges()
      << "  "
    */
      << "# Cells: "    << G.NumOfCells()
      << std::endl;
}

int main() {
  using namespace std;

  {
    cout << "Copy cartesiannd::grid<2>:" << endl;
    typedef cartesiannd::grid<2> cart_grid_type;
    typedef grid_types<cart_grid_type> gt;
    typedef gt::vertex_index_type      it;
    typedef cartesiannd::default_coord<cart_grid_type>::type coord_type;
    typedef stdext::identity<coord_type>                     mapping_type;
    typedef cartesiannd::mapped_geometry<cart_grid_type, mapping_type>  geometry_type;
    cart_grid_type R(it(2,2));
    geometry_type  GeomR(R);
    //  complexnd  ::ComplexND<2> G;
    typedef grid_types<Complex2D> gtc;
    Complex2D G;
    stored_geometry_complex2D GeomG(G);
    std::vector<gtc::vertex_handle> v_corr(R.NumOfVertices());
    std::vector<gtc::cell_handle>   c_corr(R.NumOfCells());
    // FIXME: this will assume wrong archetype!
    //    ConstructGrid0(G, R, v_corr, c_corr);
    ConstructGrid(G,GeomG, R, GeomR);
    cout << "Orig. grid:  ";  print_grid(R, cout);
    cout << "Copied grid: ";  print_grid(G, cout);

    OstreamGMV2DFmt Out("2x2.out");
    ConstructGrid(Out,G,GeomG);
  }


  {
    cout << "Copy cartesiannd::grid<3>:" << endl;
    typedef grid_types<cartesiannd::grid<3> > gt;;
    typedef gt::vertex_index_type it;
    cartesiannd::grid<3> R(it(2,2,2));
    typedef grid_types<Complex3D> gtc;
    Complex3D G;
    vertex_morphism<cartesiannd::grid<3>,   Complex3D> v_corr(R,G);
    cell_morphism  <cartesiannd::grid<3>,   Complex3D> c_corr(R,G);
    test_archetypes(R,cout);

    cout << "Grid:\n";
    R.print(cout);
    cout << "Archetype:\n";
    cartesiannd::grid<3>::TheArchetype().print(cout);

    ConstructGrid0(G, R, v_corr, c_corr);


    cout << "Orig. grid:  ";  print_grid(R, cout);
    cout << "Copied grid: ";  print_grid(G, cout);

  }

  {
    cout << "Copy cartesiannd::grid<3>:" << endl;

    typedef grid_types<cartesiannd::grid<3> > gt;;
    typedef gt::vertex_index_type it;
    cartesiannd::grid<3> R(it(2,2,2));
    typedef complexnd::ComplexND<3>   dst_grid_type;
    typedef grid_types<dst_grid_type> dgt;

    test_archetypes(R,cout);

    cout << "Grid:\n";
    R.print(cout);
    cout << "Archetype:\n";
    cartesiannd::grid<3>::TheArchetype().print(cout);

    dst_grid_type G;

    partial_grid_morphism< cartesiannd::grid<3>, dst_grid_type> Phi(R,G);
    //std::vector<dgt::vertex_handle> v_corr(R.NumOfVertices());
    //std::vector<dgt::cell_handle>   c_corr(R.NumOfCells());
    
    ConstructGrid0(G, R, Phi);
    // ConstructGrid0(G, R, v_corr, c_corr);

    cout << "Orig. grid:  ";  print_grid(R, cout);
    cout << "Copied grid: ";  print_grid(G, cout);

  }
  // does not work, partial morphism is not defined for dim > 3
  /*
  {
    cout << "Copy cartesiannd::grid<4>:" << endl;
    typedef cartesiannd::grid<4>      src_grid_type;
    typedef grid_types<src_grid_type> gt;;
    typedef gt::vertex_index_type it;
    src_grid_type  R(it(2));
    typedef complexnd::ComplexND<4>   dst_grid_type;
    typedef grid_types<dst_grid_type> dgt;

    test_archetypes(R,cout);

    cout << "Grid:\n";
    R.print(cout);
    cout << "Archetype:\n";
    src_grid_type::TheArchetype().print(cout);

    dst_grid_type G;

    partial_grid_morphism<src_grid_type, dst_grid_type> Phi(R,G);
    //std::vector<dgt::vertex_handle> v_corr(R.NumOfVertices());
    //std::vector<dgt::cell_handle>   c_corr(R.NumOfCells());
    
    complexnd::ConstructGrid0(G, R, Phi);
    // ConstructGrid0(G, R, v_corr, c_corr);

    cout << "Orig. grid:  ";  print_grid(R, cout);
    cout << "Copied grid: ";  print_grid(G, cout);

  }
  */

}
