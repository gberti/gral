#ifndef GRAL_GB_BASE_SIMPLE_GEOMETRY_H
#define GRAL_GB_BASE_SIMPLE_GEOMETRY_H

// $LICENSE


#include "Gral/Base/common-grid-basics.h"
#include "Utility/pre-post-conditions.h"
#include "Geometry/point-traits.h"
#include "Geometry/algebraic-primitives.h"
#include "Gral/Geometries/geometric-types-2d.h"

namespace GrAL {

/*! \brief Simple geometry, just storing vertex coordinates

    \ingroup  gridgeometries
    \see \ref gridgeometries module
    \see Tested in \ref test-simple-geometry.C
 */
template<class GRID, class COORD> 
class simple_geometry : public grid_types<GRID> {
  typedef simple_geometry<GRID, COORD> self;
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
  /*!  \brief Initialize with grid \c g.
     The geometrie (vertex coordinates) still need to be initialized.
   */
  simple_geometry(GRID const& g) : coords(g) {}

  /*! \brief Construct with coordinate sequence
    
      The sequence <tt> [begin_coord, end_coord[ </tt> corresponds to the 
      vertex sequence <tt> [g.FirstVertex(), g.EndVertex()[ </tt>
   */
  template<class It>
  simple_geometry(GRID const& g, It begin_coord, It end_coord) : coords(g)
  { 
    copy(begin_coord, end_coord);
  }

  /*! \brief Initialize grid and coordinates
    
      The sequence <tt> [begin_coord, end_coord[ </tt> corresponds to the 
      vertex sequence <tt> [g.FirstVertex(), g.EndVertex()[ </tt>

      The code sequence 
      \code
      simple_geometry<grid_type, coord_type> geom;
      geom.init(grid, coords.begin(), coords.end());
      \endcode
      is equivalent to
      \code
      simple_geometry<grid_type, coord_type> geom(grid, coords.begin(), coords.end())
      \endcode
   */
  template<class It>
  void init(GRID const& g, It begin_coord, It end_coord) 
  {
    coords.set_grid(g);
    copy(begin_coord, end_coord);
  }
  /*! \brief Initialize (or overwrite) coordinates.

      \pre The object is bound to a grid.

      The sequence <tt> [begin_coord, end_coord[ </tt> corresponds to the 
      vertex sequence <tt> [TheGrid().FirstVertex(), TheGrid().EndVertex()[ </tt>

      The code sequence 
      \code
      simple_geometry<grid_type, coord_type> geom(grid);
      geom.copy(grid, coords.begin(), coords.end());
      \endcode
      is equivalent to
      \code
      simple_geometry<grid_type, coord_type> geom(grid, coords.begin(), coords.end())
      \endcode
   */
  template<class It>
  void copy(It begin_coord, It end_coord) {
    for(typename gt::VertexIterator v(TheGrid()); ! v.IsDone(); ++v, ++begin_coord)
      coords[*v] = convert_point<coord_type>(*begin_coord);
  }

  grid_type  const& TheGrid() const { cb(); return coords.TheGrid();}
  void set_grid(grid_type const& g) { coords.set_grid(g);}
  void rebind  (grid_type const& g) { coords.rebind(g);}

  bool empty() const { cb(); return TheGrid().NumOfVertices() == 0;}

  /*! \brief Dimension of the embedding space
      \todo This does not work for an empty grid - use compile-time dimension if possible,
      or pass an extra parameter for the dimension if \c coord_type has a run-time dimension only.
  */
  int space_dimension() const { cb(); return pt::Dim(coords(* TheGrid().FirstVertex()));}

  //! Coordinate of vertex \c v
  coord_type const& coord(Vertex const& v) const { cb(); return coords(v);}
  //! Coordinate of vertex \c v (read/write access)
  coord_type      & coord(Vertex const& v)       { cb(); return coords[v];}

  //! \brief Euclidean lenght of edge \c e
  scalar_type length(typename gt::Edge const& e) const { cb(); return ap::distance(coord(e.V1()), coord(e.V2()));}
  //! \brief Euclidean length of edge \c e
  scalar_type volume(typename gt::Edge const& e) const {       return length(e);}

  //! Area of the face \c f
  scalar_type area(typename gt::Face const& f) const { 
    cb();
    Polygon2d<typename gt::Face,self> pf(f, *this);
    return pf.convex_area();
  }
  //! \brief Area of the face \c f
  scalar_type volume(typename gt::Face const& f) const { return area(f);}
 

  //! \brief Barycenter of the vertices of cell \c c 
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
