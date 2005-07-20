#ifndef GRAL_GB_GEOMETRY_FIND_NEAREST
#define GRAL_GB_GEOMETRY_FIND_NEAREST

// $LICENSE_NEC_2004

#include "Gral/Base/common-grid-basics.h"
#include "Geometry/algebraic-primitives.h"

#include <boost/limits.hpp>

namespace GrAL {


/*! \brief Find cell of grid \c g which is (approximately) the nearest cell to point \c p 

    \ingroup geometricsearching
   
    The algorithm performs a linear search over all cells of \c g.
    If called several times, it may be better to set up a search structure.

    \note This algorithm just checks the distance of \c p to the first vertex of each cell,
    so the result may not be the cell which is really nearest to \c p. In particular,
    if p is inside a cell \c c, there is no guarantee that \c c is actually returned.
*/
template<class GRID, class GEOM, class COORD, class GT> 
typename GT::Cell find_nearest_cell_approx(GRID const& g, GEOM const& geom, COORD pp, GT)
{
  typedef typename GEOM::coord_type         coord_type;
  typedef algebraic_primitives<coord_type>  ap;
  typedef typename ap::component_type        real_type;

  coord_type p; assign_point(p, pp);

  typename GT::Cell res;
  real_type distmin = std::numeric_limits<real_type>::max();

  for(typename GT::CellIterator c(g); !c.IsDone(); ++c) {
    double dist = ap::distance(p, geom.coord(typename GT::Vertex(* (*c).FirstVertex())));
    if(dist < distmin) {
      res = *c;
      distmin = dist;
    }
  }
  // either g is empty (distmin remains unchanged) or we must have found something.
  ENSURE_ALWAYS( (distmin ==  std::numeric_limits<real_type>::max()) || res.valid(), 
		 "did not find a valid cell!", 1);
  return res;
}


/*! \brief Convenience wrapper
 */
template<class GRID, class GEOM, class COORD>
typename grid_types<GRID>::Cell  find_nearest_cell_approx(GRID const& g, GEOM const& geom, COORD pp)
{ return find_nearest_cell_approx(g,geom,pp, grid_types<GRID>()); }

} // namespace GrAL

#endif
