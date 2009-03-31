/*! \file

*/

/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


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
