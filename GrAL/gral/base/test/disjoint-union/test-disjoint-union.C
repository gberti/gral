

/* ------------------------------------------------------------

    Copyright (C) 2001 - 2009 Guntram Berti, 
    Contact: guntram AT users DOT berlios DOT de

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


/*! \file
 
*/

#include "Gral/Views/disjoint-union-view.h"


#include "Gral/Grids/Complex2D/all.h"
#include "Gral/Grids/Cartesian2D/all.h"

#include "Gral/IO/complex2d-format-output.h"

#include "Gral/Test/all.h"


#include "Container/functions.h" // std extensions: identity

#include <iostream>

int main() {
  using namespace GrAL;
  using namespace std;
  namespace c2d = cartesian2d;

  // create Cartesian grid & geometry
  c2d::CartesianGrid2D R(3,3);
  typedef coordN<2> coord_type;
  typedef stdext::identity<coord_type>       mapping_type;
  typedef c2d::mapped_geometry<mapping_type> reg_geom_type;
  mapping_type id; 
  reg_geom_type GeomR(R, id);


  // create unstructured grid & geometry
  Complex2D G;
  typedef stored_geometry_complex2D  us_geom_type;
  us_geom_type GeomG(G);
  ConstructGrid(G,GeomG, R, GeomR);

  // create disjoint union view of above grids and geometries
  typedef   disjoint_union_view::grid_view<Complex2D, c2d::CartesianGrid2D> union_type;
  typedef grid_types<union_type> ugt;

  union_type U(G,R);
  typedef disjoint_union_view::geom_view<us_geom_type, reg_geom_type> union_geom_type;
  union_geom_type GeomU(U,GeomG, GeomR);

  // test functionality of union view
  test_vertex_iterator        (U,cout);
  test_cell_iterator          (U,cout);
  test_vertex_on_cell_iterator(U,cout);

  test_sequence_iterator<ugt::Vertex>(U, cout);
  test_sequence_iterator<ugt::Cell  >(U, cout);
  test_incidence_iterator<ugt::Vertex,ugt::Cell>(U, cout);

  typedef ugt::vertex_handle u_vertex_handle;


  // copy union into unstructured grid
  Complex2D UG;
  stored_geometry_complex2D GeomUG(UG);
  ConstructGrid(UG, GeomUG, U, GeomU);

  // output union to files
  {
    OstreamComplex2DFmt Out("union.complex2d.out");
    ConstructGrid(Out, UG, GeomUG);
  }
  {
    OstreamComplex2DFmt Out2("union2.complex2d.out");
    ConstructGrid(Out2, U, GeomU);
  }
}
