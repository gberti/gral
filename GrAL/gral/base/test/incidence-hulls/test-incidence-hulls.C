

/* ------------------------------------------------------------

    Copyright (C) 2001 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


/*! \file 

    Test cases for the algorithms in the incidence_hull module.

 */


#include "Gral/Algorithms/incidence-hulls.h"
#include "Gral/Algorithms/incidence-stencil.h"
#include "Gral/Grids/Cartesian2D/all.h"
#include "Gral/Base/partial-multi-grid-functions.h"
#include "Gral/Subranges/layered-subrange.h"
#include "Gral/Subranges/enumerated-subrange.h"


#include "Container/function-adapter.h"


#include <iostream>


template<class HULL>
void test_hull(HULL const& Hull)
{
  using namespace std;

  typedef typename HULL::LayerCellIterator   LayerCellIterator;
  typedef typename HULL::LayerVertexIterator LayerVertexIterator;

  cout <<  Hull.cells().NumOfLayers() << " cell layers:" << endl;
  for(int i = 1; i <= (int)Hull.cells().NumOfLayers(); ++i) {
    cout << "Cell layer " << i  << ": ";
    for(LayerCellIterator c(Hull.cells(i)->FirstCell()); ! c.IsDone(); ++c) {
      cout << "[" << (*c).index() << "]" << " ";
      REQUIRE_ALWAYS( Hull.layer(*c) == i, "Hull.layer(*c)=" <<  Hull.layer(*c) << " i=" << i,1);
    }
    cout << endl;
  }
  cout <<  Hull.vertices().NumOfLayers() << " vertex layers:" << endl;
  for(int i = 1; i <= (int)Hull.vertices().NumOfLayers(); ++i) {
    cout << "Vertex layer " << i  << ": ";
    for(LayerVertexIterator v(Hull.vertices(i)->FirstVertex()); ! v.IsDone(); ++v) {
      cout << "[" << (*v).index() << "]" << " ";
      REQUIRE_ALWAYS(Hull.layer(*v) == i, "Hull.layer(*v)=" << Hull.layer(*v) << " i=" << i, 1);
    }
    cout << endl;
  }
}

int main() {
  using namespace GrAL;
  using namespace std;
  
  incidence_stencil s("VC");
  {
    incidence_stencil s1("VC",2);
    incidence_stencil s2("VCVC");
    REQUIRE_ALWAYS( s1 == s2, "", 1);
    incidence_stencil s3("CVC", 3);
    incidence_stencil s4("CVCVCVC");
    REQUIRE_ALWAYS(s3 == s4, "", 1);
    REQUIRE_ALWAYS(s3 != s1, "", 1);
  }

  typedef cartesian2d::CartesianGrid2D grid_type;
  typedef grid_types<grid_type>        gt;

  grid_type R(4,4);
  typedef  layered_vertex_range<grid_type>  vertex_range;
  typedef  layered_cell_range<grid_type>    cell_range;
  typedef  vertex_range::range_type_ref     vrt;
  typedef  cell_range::range_type_ref       crt;
  typedef  vrt::VertexIterator              rgeVertexIterator;
  typedef  crt::CellIterator                rgeCellIterator;
  

  vertex_range v_layers(R);
  cell_range   c_layers(R);

  partial_multi_grid_function<grid_type, int> visited(R,0);

  gt::Cell seed_c = * R.FirstCell();
  visited[seed_c] = 1;
  c_layers.append_layer();
  c_layers.append(seed_c);

  int level = 1;
  bool dummy = false;
  mark_on_cells<gt>(c_layers.LastLayer().FirstCell(),
		    v_layers, c_layers,
		    visited,
		    s,
		    level,
		    constant<gt::Cell, bool>(true),
		    dummy);

  cout << "Seed cell: " << seed_c.index() << endl;
  for(unsigned i = 1; i <= c_layers.NumOfLayers(); ++i) {
    cout << "Cell layer " << i  << ": ";
    for(rgeCellIterator c(c_layers.Layer(i).FirstCell()); ! c.IsDone(); ++c)
      cout << "[" << (*c).index() << "]" << " ";
    cout << endl;
  }
  for(unsigned i = 1; i <= v_layers.NumOfLayers(); ++i) {
    cout << "Vertex layer " << i  << ": ";
    for(rgeVertexIterator v(v_layers.Layer(i).FirstVertex()); ! v.IsDone(); ++v)
      cout << "[" << (*v).index() << "]" << " ";
    cout << endl;
  }
	

	
  enumerated_subrange<grid_type> seed_range(R);
  seed_range.append_cell(seed_c);
  incidence_stencil s2("CVCVC");
  //  typedef incidence_hull<enumerated_subrange<grid_type>, incidence_stencil, gt> hull_type;
  typedef incidence_hull<incidence_hull_cfg<grid_type> > hull_type;
  typedef hull_type::LayerVertexIterator LayerVertexIterator;
  typedef hull_type::LayerCellIterator   LayerCellIterator;
  hull_type Hull(seed_range, s2);
  hull_type Hull2(seed_range, s2, hull_type::periodic);

  cout << "Single seed " << seed_c.index() << ", periodic:\n";
  test_hull(Hull2);

  cout << "Single seed " << seed_c.index() << ", not periodic:\n";
  test_hull(Hull);


  cout << "Re-adding seed range, non-periodic:\n";
  Hull.add_seed_range(seed_range);
  test_hull(Hull);

  gt::Cell seed_c2(R,2);
  cout << "Adding cell " << seed_c2.index() << " to seed range:\n";
  enumerated_subrange<grid_type> seed_range2(R);
  seed_range2.append_cell(seed_c2);
  Hull.add_seed_range(seed_range2);
  test_hull(Hull);
  
  cout << "Removing " << seed_c2.index() << " from seed range:\n";
  Hull.remove_seed_range(seed_range2);
  test_hull(Hull);

  cout << "Removing " << seed_c.index() << " from seed range:\n";
  Hull.remove_seed_range(seed_range);
  test_hull(Hull);
  REQUIRE_ALWAYS(Hull.empty(), "", 1);

  cout << "Adding " << seed_c.index() << ":\n";
  Hull.add_seed_range(seed_range);
  test_hull(Hull);


  enumerated_subrange<grid_type> rangeAB(R), rangeA(R), rangeB(R);
  rangeAB.push_back(seed_c);
  rangeAB.push_back(seed_c2);
  rangeA .push_back(seed_c);
  rangeB .push_back(seed_c2);
  
  hull_type H1(rangeAB, s2);
  hull_type H2(rangeA,  s2);
  H2.add_seed_range_incr(rangeB);
  for(LayerVertexIterator v(H1.FirstVertex()); !v.IsDone(); ++v)
    REQUIRE_ALWAYS( H1(*v) == H2(*v), "v=" << (*v).index() << " H1(v)=" << H1(*v) << " H2(v)=" << H2(*v),1);
  for(LayerCellIterator   v(H1.FirstCell  ()); !v.IsDone(); ++v)
    REQUIRE_ALWAYS( H1(*v) == H2(*v), "v=" << (*v).index() << " H1(v)=" << H1(*v) << " H2(v)=" << H2(*v),1);

  cout << "Testing large stencil:\n";
  incidence_stencil s3("CVC", 7);
  hull_type Hull3(seed_range, s3);
  test_hull(Hull);
}

