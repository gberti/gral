#ifndef GRAL_GB_BASE_SIMPLE_GEOMETRY_H
#define GRAL_GB_BASE_SIMPLE_GEOMETRY_H

// $LICENSE


#include "Gral/Base/common-grid-basics.h"
#include "Utility/pre-post-conditions.h"
#include "Geometry/point-traits.h"

/*! \brief Simple geometry, just storing vertex coordinates

    \ingroup  gridgeometries
    \see \ref gridgeometries
 */
template<class GRID, class COORD> 
class simple_geometry : public grid_types<GRID> {
 public:
  typedef COORD               coord_type;
  typedef grid_types<GRID>    gt;
  typedef GRID                grid_type;
  typedef typename gt::Vertex Vertex;

  typedef point_traits<coord_type> pt;
 private:
  grid_function<Vertex,coord_type> coords;

public:
  simple_geometry() {}
  simple_geometry(GRID const& g) : coords(g) {}
  grid_type  const& TheGrid() const { cb(); return coords.TheGrid();}
  void set_grid(grid_type const& g) { coords.set_grid(g);}
  void rebind  (grid_type const& g) { coords.rebind(g);}

  bool empty() const { cb(); return TheGrid().NumOfVertices() == 0;}
  // does not work for empty grid
  int space_dimension() const { cb(); return pt::Dim(coords(* TheGrid().FirstVertex()));}

  coord_type const& coord(Vertex const& v) const { cb(); return coords(v);}
  coord_type      & coord(Vertex const& v)       { cb(); return coords[v];}
 
  bool bound() const { return coords.bound();}
  void cb() const { REQUIRE(bound(), "", 1);}
};

#endif
