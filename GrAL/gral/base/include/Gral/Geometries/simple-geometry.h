#ifndef GRAL_GB_BASE_SIMPLE_GEOMETRY_H
#define GRAL_GB_BASE_SIMPLE_GEOMETRY_H

// $LICENSE


#include "Gral/Base/common-grid-basics.h"
#include "Utility/pre-post-conditions.h"
#include "Geometry/point-traits.h"
#include "Geometry/algebraic-primitives.h"

namespace GrAL {

/*! \brief Simple geometry, just storing vertex coordinates

    \ingroup  gridgeometries
    \see \ref gridgeometries module
    \todo Test
 */
template<class GRID, class COORD> 
class simple_geometry : public grid_types<GRID> {
 public:
  typedef COORD               coord_type;
  typedef grid_types<GRID>    gt;
  typedef GRID                grid_type;
  typedef typename gt::Vertex Vertex;
  typedef typename gt::Edge   Edge;
  typedef typename gt::Cell   Cell;

  typedef point_traits<coord_type> pt;
  typedef algebraic_primitives<coord_type> ap;
  typedef typename pt::component_type scalar_type;
 private:
  grid_function<Vertex,coord_type> coords;

public:
  simple_geometry() {}
  simple_geometry(GRID const& g) : coords(g) {}

  template<class It>
  simple_geometry(GRID const& g, It begin_coord, It end_coord) : coords(g)
  { 
    copy(begin_coord, end_coord);
  }
  template<class It>
  void init(GRID const& g, It begin_coord, It end_coord) 
  {
    coords.set_grid(g);
    copy(begin_coords, end_coords);
  }

  template<class It>
  void copy(It begin_coord, It end_coord) {
    for(typename gt::VertexIterator v(TheGrid()); ! v.IsDone(); ++v, ++begin_coord)
      coords[*v] = convert_point<coord_type>(*begin_coord);
  }

  grid_type  const& TheGrid() const { cb(); return coords.TheGrid();}
  void set_grid(grid_type const& g) { coords.set_grid(g);}
  void rebind  (grid_type const& g) { coords.rebind(g);}

  bool empty() const { cb(); return TheGrid().NumOfVertices() == 0;}
  // does not work for empty grid
  int space_dimension() const { cb(); return pt::Dim(coords(* TheGrid().FirstVertex()));}

  coord_type const& coord(Vertex const& v) const { cb(); return coords(v);}
  coord_type      & coord(Vertex const& v)       { cb(); return coords[v];}

  scalar_type length(Edge const& e) const { return ap::distance(coord(e.V1()), coord(e.V2()));}
  coord_type barycenter(Cell const& c) const { 
    cb();
    coord_type res(0.0);
    for(typename gt::VertexOnCellIterator vc(c); !vc.IsDone(); ++vc)
      res += coord(*vc);
    res *= scalar_type(1.0/c.NumOfVertices());
    return res;
  }
 
  bool bound() const { return coords.bound();}
  void cb() const { REQUIRE(bound(), "", 1);}
};

} // namespace GrAL 

#endif
