#ifndef GRAL_GB_GRIDS_TEST_TRIANG2D_H
#define GRAL_GB_GRIDS_TEST_TRIANG2D_H


/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


/*! \file
*/

#include <iostream>

#include "IO/control-device.h"

#include "Gral/Grids/Triang2D/triang2d.h"
#include "Gral/Grids/Triang2D/grid-functions.h"

namespace GrAL {

class TestTriang2D  : public controlable {
private:
  bool test_vertices;
  bool test_cells;
  bool test_edges;
  bool test_boundary_facets;
  bool test_boundary_range;

  bool test_cell_vertices;
  bool test_cell_edges;
  bool test_cell_neighbours;
  bool test_cell_nb_edges;

  bool test_cell_nb_iter;
  bool test_cell_edge_flip;

public:
  TestTriang2D();
  virtual void register_at(ControlDevice& Ctrl, std::string const& prefix);
  virtual void register_at(ControlDevice& Ctrl) { register_at(Ctrl,"");}
  void test_iterators(Triang2D const& CC,
		      std::ostream&   out);
};


} // namespace GrAL 

#endif
