#ifndef NMWR_GB_STORED_FOR_CARTESIAN2D_H
#define NMWR_GB_STORED_FOR_CARTESIAN2D_H

// $LICENSE

/*! \file
 */

#include "Geometry/point-traits.h"
#include "Geometry/algebraic-primitives.h"
#include "Gral/Base/common-grid-basics.h"
#include "Gral/Geometries/geometric-types-2d.h"

#include "Gral/Grids/Cartesian2D/cartesian-grid2d.h"
#include "Gral/Grids/Cartesian2D/grid-functions.h"
#include "Gral/Grids/Cartesian2D/geometry-extensions2d.h"

/*! \defgroup cartesian2dgeom Geometric components for Cartesian2D
    \ingroup cartesian2dmodule
 */

namespace GrAL {

namespace cartesian2d {

template<class Coord2D>
class stored_geometry_base : public grid_types<RegGrid2D>,
			     public algebraic_primitives<Coord2D>
{
public:
  typedef grid_geometry_category category;
  typedef Coord2D   coord_type;
  typedef RegGrid2D grid_type;

  stored_geometry_base() :  g(0) {} 

  stored_geometry_base(const RegGrid2D& gg) 
    : g(&gg), coords_(gg) {}
  
  void init(RegGrid2D const& gg) 
  {
    g = &gg;
    coords_.set_grid(gg);
  }

  void set_grid(const RegGrid2D& gg) { g = &gg; coords_.set_grid(gg);}
  void rebind  (const RegGrid2D& gg) { g = &gg; coords_.rebind  (gg);}
  bool bound() const { return (g != 0);}

  const grid_type& TheGrid() const { return *g;}

  // is this always possible?
  unsigned space_dimension() const { return Dim(coord(*(TheGrid().FirstVertex())));}

  const coord_type& coord(const Vertex& v) const { return coords_(v); }
        coord_type& coord(const Vertex& v)       { return coords_[v]; }

  void read(std::istream& in) {
    for(typename grid_function<Vertex,coord_type>::iterator ii = coords_.begin(); ii != coords_.end(); ++ii)
      in >> *ii;
  }

private:
  const grid_type* g; // reference
  grid_function<Vertex,coord_type> coords_;
};



/*! \brief "Classical" geometry for RegGrid2D with stored coordinate values

    \ingroup cartesian2dgeom

     This is a straight-line geometric embedding for the grid class
     \c RegGrid2D that simply stores the vertex coordinates. The type
     \c COORD of the coordinates is a template parameter.

     If \c COORD lives in a 2-dimensional space, there is additionaly the functionality
     in \c dimension_dependend_extensions2d<BASE, GEOM, 2>  available.

     \see Test in \ref test-stored-geometry.C
*/

template<class COORD>
class stored_geometry 
  :   public dimension_dependend_extensions2d<stored_geometry_base<COORD>, 
					      stored_geometry<COORD>, 
					      point_traits<COORD>::dimension>

{
  
public:
  typedef stored_geometry_base<COORD>  base;
  typedef stored_geometry<COORD>             self;

  typedef point_traits<COORD> pt;
  typedef algebraic_primitives<COORD> ap;

  typedef typename base::Vertex Vertex;
  typedef typename base::Edge   Edge;
  typedef typename base::Face   Face;
  typedef typename base::Facet  Facet;
  typedef typename base::Cell   Cell;

  typedef typename base::CellIterator        CellIterator;
  typedef typename base::CellOnCellIterator  CellOnCellIterator;
  typedef typename base::FacetOnCellIterator FacetOnCellIterator;

  typedef typename base::coord_type coord_type;
  // typedef typename base::EdgeIterator   EdgeIterator;

  //@{
  //! @name Constructors

  //! Empty geometry 
  stored_geometry() {}
  //! Storage allocated, but coordinates not initialized.
  stored_geometry(const RegGrid2D& gg) 
    { base::init(gg); }
  //@}

  friend std::istream& operator>>(std::istream& in, self& rs) { rs.read(in); return in;}

  /*! \brief geometric representation of edges
   */
  typedef Segment<Edge,base>   segment_type;

  /*! \brief geometric representation of faces (cells)
   */
  typedef Polygon2d<Face,base> polygon_type;

  //! get geometric representation for an edge
  segment_type segment(const Edge& e) const { return segment_type(e,basic_geom());}
  //! get geometric representation for a face (cell)
  polygon_type polygon(const Face& f) const { return polygon_type(f,basic_geom());}

  //! 1-dimensional volume of an edge
  double volume(const Edge& e) const  { return (segment_type(e,basic_geom()).length());}
  //! length of a (straight) edge
  double length(const Edge& e) const  { return (segment_type(e,basic_geom()).length());}

  //! 2-dimensional volume of a cell
  double volume(const Cell& c) const { return (polygon_type(c,basic_geom()).area());}
  
  //! Midpoint of an edge
  coord_type center(const Edge& e) const {return(segment_type(e,basic_geom()).center());}
  
  //! Center of inertia of a cell 
  coord_type center(const Cell& c) const {return(polygon_type(c,basic_geom()).center());}

private:
  const base& basic_geom() const { return *this;}  
};

} // namespace cartesian2d
} // namespace GrAL 

#endif
