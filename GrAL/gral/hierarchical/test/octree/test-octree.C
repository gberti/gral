/*! \file Test harness for octree::Octree

*/
#include "Gral/Hierarchical/octree.h"
#include "Gral/Hierarchical/refine-octree-cell-towards.C"
#include "Gral/Hierarchical/balance-octree.C"

#include "Gral/Grids/Cartesian2D/all.h"
#include "Gral/Grids/Cartesian3D/all.h"

#include "Gral/Subranges/enumerated-subrange.h"
#include "Gral/IO/complex2d-format-output.h"

#include <iostream>
#include "Container/functions.h" // identity<>

#include "Geometry/point-traits.h"

template<class T, unsigned N>
struct point_traits<tuple<T,N> > : public point_traits_fixed_size_array<tuple<T,N>, T, N> {};


template<class OCTREE>
void print_state(OCTREE const& oct, std::ostream& out) 
{
  out << "Octree state: Number of levels: " << oct.num_of_levels()  
      << " ( " << oct.coarsest_level() << " .. " << oct.finest_level() << " )" << "\n"
      << "              Number of cells:  " << oct.NumOfCells()     << "\n"
      << "              Number of leafs:  " << oct.NumOfLeafCells() << "\n";
}

namespace octree {
  template class Octree<cartesian2d::CartesianGrid2D>;
  // disabled: no grid cartesian_subrange_type
  //  template class Octree<cartesian3d::CartesianGrid3D>;
}

int main() {
  using namespace std;

  {
    typedef cartesian2d::CartesianGrid2D cart_grid_type;
    typedef grid_types<cart_grid_type> cgt;
    typedef octree::Octree<cartesian2d::CartesianGrid2D> octree_type;
    typedef octree::Octree<cartesian2d::CartesianGrid2D> octgt;
    typedef octree_type::oct_cell_type              oct_cell_type;
    typedef octree_type::OctCellChildIterator OctCellChildIterator ;

    cart_grid_type root(3,3);
    cart_grid_type ref_pattern(3,3); // 2x2 cells!
    octree_type    oct(root,ref_pattern);
    oct.activate(oct.coarsest_level());
    hierarchical::hier_partial_grid_function<oct_cell_type, int> material(* (oct.TheHierGrid()), 0);

    print_state(oct, cout);

    int lev = oct.finest_level();
    for(cgt::CellIterator c(* oct.LevelGrid(lev)); ! c.IsDone(); ++c) {
      oct_cell_type oc(* (oct.TheHierGrid()), *c, lev);
      if(oct.isLeaf(oc)) {
	cout << "Splitting cell " << (*c).index() << " => ";
	oct.split_cell(oc);
	material.update(); // add missing level(s)
	for(OctCellChildIterator ch(oc); ! ch.IsDone(); ++ch)
	  material[*ch] = material(oc);
	cout << "active range on level " << lev+1 << " is: ";
	for(octgt::ActiveLevelCellIterator ac= oct.ActiveRange(lev+1)->FirstCell(); ! ac.IsDone(); ++ac)
	  cout << (*ac).index() << ", ";
	cout << "\n";
      }
    }

    print_state(oct, cout);

    lev = oct.finest_level() -1;
    for(cgt::CellIterator c(* oct.LevelGrid(lev)); ! c.IsDone(); ++c) {
      oct_cell_type oc(* (oct.TheHierGrid()), *c, lev);
      if(oct.isBranch(oc)) {
	cout << "Joining cell " << (*c).index() << " => ";
	oct.join_cells(oc); 
	cout << "active range on level " << lev+1 << " is: ";
	if(oct.valid(lev+1)) {
	  for(octgt::ActiveLevelCellIterator ac= oct.ActiveRange(lev+1)->FirstCell(); ! ac.IsDone(); ++ac)
	    cout << (*ac).index() << ", ";
	}
	cout << "\n";
      }
    }
    print_state(oct, cout);

    enumerated_subrange<cart_grid_type> FL(*oct.LevelGrid(oct.finest_level()), 
					   *oct.ActiveRange(oct.finest_level()));
    typedef tuple<double,2> coord_type;
    typedef stdext::identity<coord_type>        mapping_type;
    cartesian2d::mapped_geometry<mapping_type> GeomFL(*oct.LevelGrid(oct.finest_level()));
    OstreamComplex2DFmt GOut("oct.complex2d");
    ConstructGrid(GOut, GOut, FL, GeomFL);

    oct.add_coarser_level();
    cout << "Number of levels: " << oct.num_of_levels() << "\n";

    lev = oct.coarsest_level();
    for(cgt::CellIterator c(* oct.LevelGrid(lev)); ! c.IsDone(); ++c) {
      oct_cell_type oc(* (oct.TheHierGrid()), *c, lev);
      if(oct.isBranch(oc)) {
	cout << "Joining cell " << (*c).index() << " => ";
	oct.join_cells(oc); 
	cout << "active range on level " << lev+1 << " is: ";
	if(oct.valid(lev+1)) {
	  for(octgt::ActiveLevelCellIterator ac= oct.ActiveRange(lev+1)->FirstCell(); ! ac.IsDone(); ++ac)
	    cout << (*ac).index() << ", ";
	}
	cout << "\n";
      }
    }
    print_state(oct, cout);

   
    typedef octree_type::LeafCellIterator LeafCellIterator;
    // gcc2.96 does not get this
    // LeafCellIterator  lc(ref_ptr<const octree_type>(oct));
    LeafCellIterator  lc = ref_ptr<const octree_type>(oct);
    cout << "Leafs:\n";
    for(; !lc.IsDone(); ++lc)
      cout << lc.Flat().index() << "\n";


    // problems: 
    // (a) must manually add layers to material,
    // (b) must ensure that material  are propagated towards children.
    // solutions:
    // (a) notifier/observer pattern for octree/grid function
    // (b) give hier. gf an extra parameter which initializes finer levels from coarser ones
    //     (or vice versa)
  }
  
  {
    typedef cartesian2d::CartesianGrid2D cart_grid_type;
    typedef grid_types<cart_grid_type>   cgt;
    typedef octree::Octree<cartesian2d::CartesianGrid2D> octree_type;
    typedef octree::Octree<cartesian2d::CartesianGrid2D> octgt;
    typedef octree_type::oct_cell_type                   oct_cell_type;
    typedef octree_type::hier_grid_type                  hgt;

    cart_grid_type root(2,2);        // 1x1 cells
    cart_grid_type ref_pattern(3,3); // 2x2 cells!
    octree_type    oct(root,ref_pattern);
    oct.activate(oct.coarsest_level());

    cout << "refine towards:\n";
    oct.add_finer_level();
    octgt::level_handle coarse = oct.coarsest_level();
    octgt::level_handle fine   = oct.finest_level();
    
    print_state(oct, cout);
    hgt::hier_cell_type c_finest(oct.TheHierGrid(), * oct.TheHierGrid()->FlatGrid(fine)->FirstCell(), fine);
    hgt::hier_cell_type c_root   = oct.leaf_ancestor(c_finest);
    refine_towards(oct, c_root, c_finest, 1);

    print_state(oct, cout);
    for(octgt::level_handle lev = oct.coarsest_level(); lev <= oct.finest_level(); ++lev) {
      cout << "Level " << lev << ":\n";
      for(cgt::CellIterator c(oct.LevelGrid(lev)->FirstCell()); ! c.IsDone(); ++c)
	cout << "Cell " << (*c).index() << (oct.isLeaf(hgt::hier_cell_type(oct.TheHierGrid(), *c, lev)) ? " (L)" : "" ) << "\n";
    }

    oct.add_finer_level();
    oct.add_finer_level();
    fine = oct.finest_level();
    c_finest = hgt::hier_cell_type(oct.TheHierGrid(), oct.TheHierGrid()->FlatGrid(fine)->cell(cgt::index_type(4,4)), fine);
    c_root   = oct.leaf_ancestor(c_finest);
    refine_towards(oct, c_root, c_finest, 2);								

    print_state(oct, cout);
    for(octgt::level_handle lev = oct.coarsest_level(); lev <= oct.finest_level(); ++lev) {
      cout << "Level " << lev << ":\n";
      for(cgt::CellIterator c(oct.LevelGrid(lev)->FirstCell()); ! c.IsDone(); ++c) {
	hgt::hier_cell_type hc(oct.TheHierGrid(), *c, lev);
	if(oct.isActive(hc))
	  cout << "Cell " << (*c).index() << (oct.isLeaf(hc) ? " (L)" : "" ) << "\n";
      }
    }

    cout << "balancing:\n";
    balance_octree(oct, 1);
    print_state(oct, cout);
    for(octgt::level_handle lev = oct.coarsest_level(); lev <= oct.finest_level(); ++lev) {
      cout << "Level " << lev << ":\n";
      for(cgt::CellIterator c(oct.LevelGrid(lev)->FirstCell()); ! c.IsDone(); ++c) {
	hgt::hier_cell_type hc(oct.TheHierGrid(), *c, lev);
	if(oct.isActive(hc))
	  cout << "Cell " << (*c).index() << (oct.isLeaf(hc) ? " (L)" : "" ) << "\n";
      }
    }

  }
    

}
