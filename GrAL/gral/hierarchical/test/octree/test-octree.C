#include "Gral/Hierarchical/octree.h"

#include "Gral/Grids/Cartesian2D/all.h"
#include "Gral/Grids/Cartesian3D/all.h"

#include "Gral/Subranges/enumerated-subrange.h"
#include "Gral/IO/complex2d-format-output.h"

#include <iostream>
#include "Container/functions.h" // identity<>

#include "Geometry/point-traits.h"

template<class T, unsigned N>
struct point_traits<tuple<T,N> > : public point_traits_fixed_size_array<tuple<T,N>, T, N> {};


namespace octree {
  template class Octree<cartesian2d::CartesianGrid2D>;
  template class Octree<cartesian3d::CartesianGrid3D>;
}

int main() {
  using namespace std;

  {
    typedef cartesian2d::CartesianGrid2D cart_grid_type;
    typedef grid_types<cart_grid_type> cgt;
    typedef octree::Octree<cartesian2d::CartesianGrid2D> octree_type;
    typedef octree::Octree<cartesian2d::CartesianGrid2D> octgt;
    typedef octree_type::OctCell              OctCell;
    typedef octree_type::OctCellChildIterator OctCellChildIterator ;

    cart_grid_type root(3,3);
    cart_grid_type ref_pattern(3,3); // 2x1 cells!
    octree::Octree<cartesian2d::CartesianGrid2D>  oct(root,ref_pattern);
    hierarchical::hier_partial_grid_function<OctCell, int> material(* (oct.TheHGrid()), 0);

    cout << "Number of levels: " << oct.num_of_levels() << "\n";
    int lev = oct.finest_level();
    for(cgt::CellIterator c(* oct.LevelGrid(lev)); ! c.IsDone(); ++c) {
      OctCell oc(* (oct.TheHGrid()), *c, lev);
      if(oct.isLeaf(oc)) {
	cout << "Splitting cell " << (*c).index() << " => ";
	oct.split_cell(oc);
	material.update(); // add missing level(s)
	for(OctCellChildIterator ch(oc); ! ch.IsDone(); ++ch)
	  material[*ch] = material(oc);
	cout << "active range on level " << lev+1 << " is: ";
	for(octgt::ActiveRangeCellIterator ac= oct.ActiveRange(lev+1)->FirstCell(); ! ac.IsDone(); ++ac)
	  cout << (*ac).index() << ", ";
	cout << "\n";
      }
    }
    cout << "Number of levels: " << oct.num_of_levels() << "\n";
    lev = oct.finest_level() -1;
    for(cgt::CellIterator c(* oct.LevelGrid(lev)); ! c.IsDone(); ++c) {
      OctCell oc(* (oct.TheHGrid()), *c, lev);
      if(oct.isBranch(oc)) {
	cout << "Joining cell " << (*c).index() << " => ";
	oct.join_cells(oc); 
	cout << "active range on level " << lev+1 << " is: ";
	if(oct.valid(lev+1)) {
	  for(octgt::ActiveRangeCellIterator ac= oct.ActiveRange(lev+1)->FirstCell(); ! ac.IsDone(); ++ac)
	    cout << (*ac).index() << ", ";
	}
	cout << "\n";
      }
    }
    cout << "Number of levels: " << oct.num_of_levels() << "\n";


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
      OctCell oc(* (oct.TheHGrid()), *c, lev);
      if(oct.isBranch(oc)) {
	cout << "Joining cell " << (*c).index() << " => ";
	oct.join_cells(oc); 
	cout << "active range on level " << lev+1 << " is: ";
	if(oct.valid(lev+1)) {
	  for(octgt::ActiveRangeCellIterator ac= oct.ActiveRange(lev+1)->FirstCell(); ! ac.IsDone(); ++ac)
	    cout << (*ac).index() << ", ";
	}
	cout << "\n";
      }
    }
    cout << "Number of levels: " << oct.num_of_levels() << "\n";
   

    // problems: 
    // (a) must manually add layers to material,
    // (b) must ensure that material  are propagated towards children.
    // solutions:
    // (a) notifier/observer pattern for octree/grid function
    // (b) give hier. gf an extra parameter which initializes finer levels from coarser ones
    //     (or vice versa)
  }
}
