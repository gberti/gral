#include "Gral/Subranges/boundary-grid.h"

#include "Gral/Grids/Cartesian2D/all.h"

#include <iostream>

int main() {

  using namespace cartesian2d;

  typedef grid_types<CartesianGrid2D> gt;

  CartesianGrid2D R(2,2);

  typedef boundary_grid<gt::Cell> cell_bd_t;
  typedef grid_types<cell_bd_t>   bdcgt;
  boundary_grid<gt::Cell> bd(*(R.FirstCell()));
  

  for(bdcgt::CellIterator c(bd); ! c.IsDone(); ++c) {
    std::cout << c.handle() << " [facets: ";
    for(bdcgt::FacetOnCellIterator fc(*c); ! fc.IsDone(); ++fc) {
      std::cout << fc.handle() << " ";
    }
    std::cout << "]\n"; 
  }
}
