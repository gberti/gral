
// $LICENSE

#ifndef GRAL_GB_BASE_DUMMY_GEOMETRY_H
#define GRAL_GB_BASE_DUMMY_GEOMETRY_H


#include "Gral/Base/common-grid-basics.h"

namespace GrAL {

/*! \brief Dummy geometry for places where one is formally required.
    \ingroup gridgeometries
    \see \ref    gridgeometries module
 */
template<class GRID> 
class dummy_geometry : public grid_types<GRID> {
public:
  typedef GRID     grid_type;
  typedef float    coord_type;
  typedef typename grid_types<GRID>::Vertex Vertex;
private:
  GRID const* g;
  float       x;
public:
  dummy_geometry() : g(0), x(0.0) {}
  dummy_geometry(GRID const& gg) : g(&gg), x(0.0) {}
  GRID const& TheGrid() const { return *g;}

  void set_grid(GRID const& gg) {g = &gg;}

  coord_type   coord(Vertex const&) const { return x;}
  coord_type & coord(Vertex const&)       { return x;}

  bool bound() const { return g != 0;}
  void rebind(grid_type const& gg) { g = &gg;}
};

/*! \brief Creator function for dummy_geometry.
  \ingroup  gridgeometries
   \relates dummy_geometry<GRID>
 */
template<class GRID> 
inline
dummy_geometry<GRID> 
DummyGeometry(GRID const& g) { return dummy_geometry<GRID>(g);}

} // namespace GrAL 

#endif
