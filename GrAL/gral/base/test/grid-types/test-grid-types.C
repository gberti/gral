/*! \file

*/

#include "Gral/Base/grid-types.h"

template<int D>
struct grid_types_test {
  typedef int Cell;
  typedef int cell_handle;
  typedef int CellIterator;
  typedef int Facet;
  typedef int VertexOnCellIterator;
  typedef int EdgeOnCellIterator;

  typedef GrAL::grid_dim_tag<D> dimension_tag;
};




int main() 
{
  using namespace std;
  
  cout << "checking grid_types_test<1>:\n";
  GrAL::checkgt_terse<grid_types_test<1> >(cout);

  typedef GrAL::dim_dep_defs<grid_types_test<1> > gt_d1;

  cout << "checking dim_dep_defs<grid_types_test<1> >\n";
  GrAL::checkgt_terse<gt_d1>(cout);

  cout << "\n\n";

  cout << "checking grid_types_test<2>:\n";
  GrAL::checkgt_terse<grid_types_test<2> >(cout);

  typedef GrAL::dim_dep_defs<grid_types_test<2> > gt_d2;

  cout << "checking dim_dep_defs<grid_types_test<2> >\n";
  GrAL::checkgt_terse<gt_d2>(cout);

  cout << "\n\n";


  cout << "checking grid_types_test<3>:\n";
  GrAL::checkgt_terse<grid_types_test<3> >(cout);

  typedef GrAL::dim_dep_defs<grid_types_test<3> > gt_d3;

  cout << "checking dim_dep_defs<grid_types_test<3> >\n";
  GrAL::checkgt_terse<gt_d3>(cout);


}
