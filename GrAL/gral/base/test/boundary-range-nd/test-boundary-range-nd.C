/*! \file

    Test harness for \c GrAL::boundary_range_nd<>
*/

#include "Gral/Subranges/boundary-range-nd.h"
//#include "Gral/Grids/CartesianND/cartesiannd.h"
#include "Gral/Grids/Cartesian2D/all.h"
#include "Gral/Test/test-facet-iterator.h"

#include <iostream>

namespace GrAL {
  //  template boundary_range_nd<cartesiannd::grid<2> >;
  template boundary_range_nd<cartesian2d::CartesianGrid2D>;
}

int main() {
  using namespace GrAL;
  using namespace std;

  {
    cartesian2d::CartesianGrid2D R(2,2);
    boundary_range_nd<cartesian2d::CartesianGrid2D> bd(R);
    typedef grid_types<boundary_range_nd<cartesian2d::CartesianGrid2D> > bgt;
    // test_facet_iterator(bd, cout);
    cout << "bd: " << bd.NumOfFacets() << " facets" << endl;
    for(bgt::FacetIterator f(bd.FirstFacet()); !f.IsDone(); ++f) {
    //for(bgt::FacetIterator f(bd); !f.IsDone(); ++f) {
      cout << "f = [";
      for(bgt::VertexOnFacetIterator vf(*f); !vf.IsDone(); ++vf)
	cout << vf.handle() << " ";
      cout <<"]\n";
    }
    cout << "Vertices: ";
    //for(bgt::VertexIterator v(bd.FirstVertex()); !v.IsDone(); ++v)
    for(bgt::VertexIterator v(bd); !v.IsDone(); ++v)
      cout << (*v).handle() << " ";
    cout << endl;
  }

  {
    cartesian2d::CartesianGrid2D R(3,3);
    boundary_range_nd<cartesian2d::CartesianGrid2D> bd(R);
    typedef grid_types<boundary_range_nd<cartesian2d::CartesianGrid2D> > bgt;
    // test_facet_iterator(bd, cout);
    cout << "bd: " << bd.NumOfFacets() << " facets" << endl;
    for(bgt::FacetIterator f(bd.FirstFacet()); !f.IsDone(); ++f) {
      cout << "f = [";
      for(bgt::VertexOnFacetIterator vf(*f); !vf.IsDone(); ++vf)
	cout << vf.handle() << " ";
      cout <<"]\n";
    }
    cout << "Vertices: ";
    for(bgt::VertexIterator v(bd.FirstVertex()); !v.IsDone(); ++v)
      cout << (*v).handle() << " ";
    cout << endl;
  }
}

