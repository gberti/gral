#ifndef GRAL_GB_BASE_SIMPLE_GEOMETRY_H
#define GRAL_GB_BASE_SIMPLE_GEOMETRY_H


#include "Gral/Base/common-grid-basics.h"

/*! \brief Simple geometry, just storing vertices
 */
template<class GRID, class COORD> 
class simple_geometry : public grid_types<GRID> {
 public:
  typedef COORD               coord_type;
  typedef grid_types<GRID>    gt;
  typedef GRID                grid_type;
  typedef typename gt::Vertex Vertex;
 private:
  grid_function<Vertex,coord_type> coords;;

public:
  simple_geometry() {}
  simple_geometry(GRID const& g) : coords(g) {}
  grid_type  const& TheGrid() const { return coords.TheGrid();}
  void set_grid(grid_type const& g) { coords.set_grid(g);}

  coord_type const& coord(Vertex const& v) const { return coords(v);}
  coord_type      & coord(Vertex const& v)         { return coords[v];}
};

#endif
