#ifndef NMWR_GB_STORED_FOR_CARTESIAN2D_H
#define NMWR_GB_STORED_FOR_CARTESIAN2D_H

// $LICENSE

#include "Geometry/point-traits.h"
#include "Geometry/algebraic-primitives.h"
#include "Gral/Base/common-grid-basics.h"
#include "Gral/Geometries/geometric-types.h"

#include "Gral/Grids/Cartesian2D/cartesian-grid2d.h"
#include "Gral/Grids/Cartesian2D/grid-functions.h"


namespace cartesian2d {

template<class Coord2D>
class stored_geometry_reg2d_base : public grid_types<RegGrid2D>,
                                   public algebraic_primitives<Coord2D>
{
public:
  typedef Coord2D   coord_type;
  typedef RegGrid2D grid_type;

  stored_geometry_reg2d_base() :  g(0) {} 

  stored_geometry_reg2d_base(const RegGrid2D& gg) 
    : g(&gg), coords_(gg) {}

  void set_grid(const RegGrid2D& gg) { g = &gg; coords_.set_grid(gg);}
  void rebind  (const RegGrid2D& gg) { g = &gg; coords_.rebind  (gg);}
  void bound() const { return (g != 0);}

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

  This is a straight-line geometric embedding for the grid class
  RegGrid2D that simply stores the vertex coordinates. The type
  Coord2D of the coordinates is a template parameter.
  This uses a grid_function<Vertex,Coord2D> to store the coordinates,
  so you (yet) have to provide a specialization to these parameters.

  There is a problem with the provided functionality:
  If we have a 2D-embedding (which is assumed the normal case)
  we may provide normal directions to Edges, in the 3D-case 
  this does not make sense. So there are 2 possibilities:
   - specialize to 2D case
   - provide compile-time branching over static dimension of
     Coord2D parameter.

  \todo Use the same mechanism for dimension dependent functionality
        as in cartesian2d::mapped_geometry
*/

template<class Coord2D>
class stored_geometry 
  : public  stored_geometry_reg2d_base<Coord2D>
{
  
public:
  typedef stored_geometry_reg2d_base<Coord2D>  base;
  typedef stored_geometry<Coord2D>             self;

  typedef point_traits<Coord2D> pt;
  typedef algebraic_primitives<Coord2D> ap;

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


  stored_geometry() {}
  stored_geometry(const RegGrid2D& gg) 
    : base(gg) {}

  friend std::istream& operator>>(std::istream& in, self& rs) { rs.read(in); return in;}


  typedef Segment<Edge,base>   segment_type;
  typedef Polygon2d<Face,base> polygon_type;

  segment_type segment(const Edge& e) const { return segment_type(e,basic_geom());}
  polygon_type polygon(const Face& f) const { return polygon_type(f,basic_geom());}

  double volume(const Edge& e) const  { return (segment_type(e,basic_geom()).length());}
  double length(const Edge& e) const  { return (segment_type(e,basic_geom()).length());}
  // double volume(const EdgeIterator& e) const {return volume(*e);}
  double volume(const Cell& c) const { return (polygon_type(c,basic_geom()).area());}
  
  coord_type center(const Edge& e) const {return(segment_type(e,basic_geom()).center());}
  
  // center of inertia
  coord_type center(const Cell& c) const {return(polygon_type(c,basic_geom()).center());}

  coord_type outer_area_normal(/*const CellIterator& c,*/
			       const CellOnCellIterator& nb)
    { // this could be done more efficiently.
      //coord_type ctr(center(*c));
      coord_type ctr(center(nb.TheCell()));
      Edge e(nb);
      segment_type S(e,basic_geom());
      coord_type n(ap::normal_with_same_length(S.End() - S.Start()));
      return ( ap::dot(n,(ctr - S.Start())) < 0 ? n : coord_type(-n));
    }
  coord_type outer_area_normal(/*const CellIterator& c,*/
			       const FacetOnCellIterator& f)
    { // this could be done more efficiently.
      //coord_type ctr(center(*c));
      coord_type ctr(center(f.TheCell()));
      Edge e(*f);
      segment_type S(e,basic_geom());
      coord_type n(ap::normal_with_same_length(S.End() - S.Start()));
      return ( ap::dot(n,(ctr - S.Start())) < 0 ? n : coord_type(-n));
    }
 
  /*
  coord_type outer_area_normal(const Cell& c,
			       const NeighbourCellIterator& nb)
    { // this could be done more efficiently.
      coord_type ctr(center(c));
      Edge e(c,nb);
      segment_type S(e,basic_geom());
      coord_type n(ap::normal_with_same_length(S.End() - S.Start()));
      return ( ap::dot(n,(ctr - S.Start())) < 0 ? n : coord_type(-n));
    }
  */

  coord_type normal_dir(const Edge& e) const {
    coord_type v1 = coord(e.V1());
    coord_type v2 = coord(e.V2());
    return coord_type(pt::y(v2)-pt::y(v1),pt::x(v1)-pt::x(v2));
  }

  coord_type normed_outer_normal(const Edge& e, const Cell& c) const {
    coord_type v1 = coord(e.V1());
    coord_type v2 = coord(e.V2());
    coord_type dir(ap::normed_normal(v2-v1));
    coord_type ctr = center(c);
    return (ap::dot(dir,v1-ctr) > 0 ? dir : coord_type(-dir));
  }

  /*  only useful if dim(coord_type) == 2 !
      hint: use normed_normal(segment(facet(nb)));

  coord_type normal(const CellIterator& , 
		   const NeighbourCellIterator& nb) const // |.| == 1
    {
      Edge e(nb);
      segment_type S(e,basic_geom());
      return normed_normal(S.End()-S.Start());
    }
  
  coord_type area_normal(const CellIterator& ,
			const NeighbourCellIterator& nb) const 
    {
      Edge e(nb);
      segment_type S(e,basic_geom());
      return normal_with_same_length(S.End()-S.Start());
    }
  
  coord_type area_normal(const Edge& e) const 
  {
    segment_type S(e,basic_geom());
    return normal_with_same_length(S.End()-S.Start());
  }
  
  coord_type normal(const Edge& e) const 
  {
    segment_type S(e,basic_geom());
    return normed_normal(S.End()-S.Start());
  }
  */
private:
  const base& basic_geom() const { return *this;}  
};

} // namespace cartesian2d


#endif
