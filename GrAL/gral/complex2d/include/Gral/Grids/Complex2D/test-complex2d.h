#include <iostream.h>

#include "IO/control-device.h"

#include "Gral/Grids/Complex2D/complex2d.h"
#include "Gral/Grids/Complex2D/stored-geometry.h"
#include "Gral/Grids/Complex2D/grid-functions.h"


class TestComplex2D  : public controlable {
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
  TestComplex2D();
  virtual void register_at(ControlDevice& Ctrl, const string& prefix);
  virtual void register_at(ControlDevice& Ctrl) { register_at(Ctrl,"");}
  void test_complex2d_iterators(const Complex2D& CC,
				const stored_geometry_complex2D& geom,
				ostream& out);
};
