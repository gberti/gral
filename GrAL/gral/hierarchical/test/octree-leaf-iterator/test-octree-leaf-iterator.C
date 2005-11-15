/*! \file Test harness for octree::Octree

*/

#include "Gral/Hierarchical/octree.h"
#include "Gral/Grids/CartesianND/all.h"

#include "Utility/as-string.h"

template<class OCTREE>
void print_state(OCTREE const& oct, std::ostream& out) 
{
  out << "Octree state: Number of levels: " << oct.num_of_levels()  
      << " ( " << oct.coarsest_level() << " .. " << oct.finest_level() << " )" << "\n"
      << "              Number of cells:  " << oct.NumOfCells()     << "\n"
      << "              Number of leafs:  " << oct.NumOfLeafCells() << "\n"
      << "Active vertices:\n";
  
  for(int lev = oct.coarsest_level(); lev <= oct.finest_level(); ++lev) {
    out << "Level " << lev << ": " << oct.ActiveVertexRange(lev)->NumOfVertices() << " vertices:  ";
    for(typename OCTREE::ActiveLevelVertexIterator v(oct.ActiveVertexRange(lev)->FirstVertex()); !v.IsDone(); ++v)
      out << (*v).index() << ", ";
    out << std::endl;
  }
  
}

template<class E>
std::string print_edge(E const& e) { 
  //  return GrAL::as_string(e.Flat().index()) + "," + GrAL::as_string(e.level());
  typedef GrAL::element_traits<typename E::flat_element_type> et;
  typedef GrAL::grid_types<typename et::grid_type>            gt;

  std::string res = "L" + GrAL::as_string(e.level()) + " [";
  typename gt::VertexOnEdgeIterator ve(e.Flat());
  res += GrAL::as_string((*ve).index());
  res += " -> ";
  ++ve;
  res += GrAL::as_string((*ve).index());
  res += "]";
  return res;
}

int main() {
  using namespace GrAL;
  using namespace std;
 
  typedef cartesiannd::grid<2>                    cart_grid_type;
  typedef grid_types<cart_grid_type>              cgt;
  typedef octree::Octree<cart_grid_type>          octree_type;
  typedef octree::Octree<cart_grid_type>          octgt;
  typedef octree_type::oct_cell_type              oct_cell_type;
  typedef octree_type::oct_edge_type              oct_edge_type;

  cart_grid_type root(3,3);
  cart_grid_type ref_pattern(3,3); // 2x2 cells!
  octree_type    oct(root,ref_pattern);
  oct.activate(oct.coarsest_level());

  int lev = oct.finest_level();
  for(cgt::EdgeIterator e(* oct.LevelGrid(lev)); ! e.IsDone(); ++e) {
    oct_edge_type oe(* (oct.TheHierGrid()), *e, lev);
    octree::leaf_elem_iterator<octree_type, oct_edge_type> el(oct, oe);
    cout << "Edge leaf iterator:\n";
    while(! el.IsDone()) {
      cout <<  print_edge(*el) << "\n";
      ++el;
    }
  }



  for(int i = 0; i < 3; ++i) {
    int flev = oct.finest_level();
    oct_cell_type oc(* (oct.TheHierGrid()),* oct.LevelGrid(flev)->FirstCell(), flev);
    oct.split_cell(oc);
    cout << "Refining cell " << oc.Flat().index() << "," << oc.level() << "\n";
    print_state(oct,cout);
    
    int clev = oct.coarsest_level();
    oct_cell_type oc0(* (oct.TheHierGrid()),* oct.LevelGrid(clev)->FirstCell(), clev);
    for(cgt::EdgeOnCellIterator ec(oc0.Flat()); !ec.IsDone(); ++ec) {
      oct_edge_type oe(* (oct.TheHierGrid()), *ec, oc0.level());
      octree::leaf_elem_iterator<octree_type, oct_edge_type> el(oct, oe);
      cout << "Edge leaf iterator on e=" << oe.Flat().index() << ", " << oe.level() <<  ":\n";
      while(! el.IsDone()) {
	cout <<  print_edge(*el) << "\n";
	++el;
      }
    }
  }

  lev = oct.coarsest_level();
  for(cgt::EdgeIterator e(* oct.LevelGrid(lev)); ! e.IsDone(); ++e) {
    oct_edge_type oe(* (oct.TheHierGrid()), *e, lev);
    octree::leaf_elem_iterator<octree_type, oct_edge_type> el(oct, oe);
    cout << "Edge leaf iterator on " << oe.Flat().index() << ", " << oe.level() <<":\n";
    while(! el.IsDone()) {
       cout <<  print_edge(*el) << "\n";
      ++el;
    }
  }

}
