#include "Gral/Grids/Cartesian2D/general-index-map.h"


void general_indexmap2D::pretty_print(std::ostream& out) {
  out << "v1 = " << v1 << "   v2 = " << v2 << '\n'
      << "major_idx = " << major_idx  << ";  minor_idx = " << minor_idx << '\n'
      << "major_incr = " << major_incr  << ";  minor_incr = " << minor_incr << '\n'
      << "n0 = " << n0() << '\n'
      << "ll = " << ll() << '\n'
      << "ur = " << ur() << '\n'
      << "size = " << range_size() << '\n'
      << "nx = " << np.x << ", ny = " << np.y << '\n';
}
