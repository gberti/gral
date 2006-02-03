#ifndef NMWR_GB_GEOM_PRIMITIVES_FOR_COMPLEX_H
#define NMWR_GB_GEOM_PRIMITIVES_FOR_COMPLEX_H


// $LICENSE

#include "Config/compiler-config.h"

#include <algorithm>
#include "Geometry/algebraic-primitives.h"

#include "Gral/Base/common-grid-basics.h"
#include "Gral/Geometries/geometric-types-2d.h" 
#include "Gral/Grids/Complex2D/complex2d.h"

namespace GrAL {

struct stored_geometry_complex2D_base
  : public grid_types<Complex2D> {
public:
  typedef Complex2D grid_type;
protected:
public:
  grid_type *  g;
public:
  typedef grid_geometry_category    category;
  typedef grid_type::CoordType      coord_type;
  typedef point_traits<coord_type>          pt;
  typedef algebraic_primitives<coord_type>  ap;

  typedef  pt::component_type       real_type;

  stored_geometry_complex2D_base() : g(0) {}

  // we cast const away, because non-const reference is only needed for
  // write-access to coordinates, and that conceptually does not belong into
  // the grids responsibility
  stored_geometry_complex2D_base(grid_type const& gg) : g((grid_type*) &gg) {}
  void set_grid(grid_type const& gg) { g = (grid_type*) (&gg);}
  void rebind  (grid_type const& gg) { g = (grid_type*) (&gg);}
  bool bound() const { return (g != 0);}

  unsigned space_dimension() const { return pt::Dim(coord(* TheGrid().FirstVertex()));}

  const Complex2D& TheGrid() const 
    { REQUIRE((g != 0), "geometry: Grid = 0!\n",1);  return *g;}
  Complex2D&       TheGrid()       
    { REQUIRE((g != 0), "geometry: Grid = 0!\n",1); return *g;}

  const coord_type& coord(const Vertex& V) const { 
    // IOMgr::Info() << "base geom g = " << g << endl;
    REQUIRE((&(V.TheGrid()) == g),
	     "coord(V): Grids don't match: V.TheGrid() = "
	     << &(V.TheGrid()) << ", this->TheGrid() : " << g << "!\n",1);
    return TheGrid().Coord(V);
  }
  coord_type&       coord(const Vertex& V) {
    // IOMgr::Info() << "base geom g = " << g << endl;
    REQUIRE((&(V.TheGrid()) == g),
	    "coord(V): Grids don't match: V.TheGrid() = "
	    << &(V.TheGrid()) << ", this->TheGrid() : " << g << "!\n",1);
    return TheGrid().Coord(V);
  }
};


/*! \brief Geometry class for Complex2D

     \ingroup complex2dmodule
 
     This is an embedding of a grid of type Complex2D into 2D Euclidean space,
     using straight edges.

     \todo Test
 */
struct stored_geometry_complex2D
 : public stored_geometry_complex2D_base
{
private:
  typedef stored_geometry_complex2D_base base;
  typedef stored_geometry_complex2D      self;
public:  
  stored_geometry_complex2D() {}
  stored_geometry_complex2D(grid_type const& gg) : base(gg)  {}

  //! Geometric representation for an edge
  typedef Segment<Edge,base>               segment_type;
  //! Geometric representation for an edge
  typedef Polygon2d<Cell,base>             polygon_type;
  typedef algebraic_primitives<coord_type> ap;

  //! Get geometric representation for an edge
  segment_type segment(const Edge& E) const { return segment_type(E,basic_geom());}
  //! Get geometric representation for a face (cell)
  polygon_type polygon(const Face& F) const { return polygon_type(F,basic_geom());}

  //! 
  double length(const Edge& e) const { return (segment_type(e,basic_geom()).length());}

  //! 1-dimensional volume of an edge (length)
  double volume(const Edge& e) const { return (segment_type(e,basic_geom()).length());}
  //! 1-dimensional volume of an edge (length)
  double volume(const EdgeIterator& e) const {return volume(*e);}
  //! 2-dimensional volume of a cell (area)
  double volume(const Cell& c) const { 
    polygon_type poly(c,basic_geom());
    return (poly.area());
  }

  /*! \brief Diameter of a cell
     
      This function returns the maximal distance of two vertices. 
  */
  double diameter(Cell const& c)  const {
    double diam2 = 0.0;
    for(VertexOnCellIterator vc(c); ! vc.IsDone(); ++vc) {
      VertexOnCellIterator w = vc; 
      ++w;
      for(; !w.IsDone(); ++w)
        diam2 = ::std::max(diam2, ap::distance2(coord(*vc),coord(*w)));
    }
    return sqrt(diam2);
  }

  //!
  coord_type center(const Edge& e) const {return(segment_type(e,basic_geom()).center());}

  //! center of inertia
  coord_type center(const Cell& c) const {return(polygon_type(c,basic_geom()).center());}

  //! \brief Barycenter of the vertices of cell \c c 
  coord_type barycenter(Cell const& c) const { 
    coord_type res(0.0);
    for(VertexOnCellIterator vc(c); !vc.IsDone(); ++vc)
      res += coord(*vc);
    res *= 1.0/c.NumOfVertices();
    return res;
  }


  /*! \brief Outward pointing normal of a facet
       The result is normalized and  points outward of <tt>nb.TheCell()</tt>.
   */
  coord_type outer_normal(/*const CellIterator& c, */
			  const FacetOnCellIterator& nb) const 
    {
      coord_type ctr(center(nb.TheCell()));
      Edge e(nb);
      segment_type S(e,basic_geom());
      coord_type n( ap::normed_normal(S.End()-S.Start()));
      return ( ap::dot(n,(ctr - S.Start())) < 0 ? n : coord_type(-n));
    }

 
  /*! \brief Outward pointing normal of a facet
       The result points outward of <tt>f.TheCell()</tt>.
       Its Euclidean norm is equal to the volume of the facet \c f (i.e. the length of \f). 
   */
  coord_type outer_area_normal(const FacetOnCellIterator& f) const 
    //    const NeighbourCellIterator& nb) const
    {
      coord_type ctr(center(f.TheCell()));
      Edge e(*f);
      segment_type S(e,basic_geom());
      coord_type n(ap::normal_with_same_length(S.End() - S.Start()));
      return ( ap::dot(n,(ctr - S.Start())) < 0 ? n : coord_type(-n));
    }

 /*! \brief Outward pointing normal towards a neighbor cell
       The result points toward  <tt>nb</tt>.
       Its Euclidean norm is equal to the volume (i.e. length) of the facet f 
       separating \c nb and <tt>nb.TheCell()</tt>.
   */
  coord_type outer_area_normal(const CellNeighbourIterator& nb) const
    {
      coord_type ctr(center(nb.TheCell()));
      Edge e(nb);
      segment_type S(e,basic_geom());
      coord_type n(ap::normal_with_same_length(S.End() - S.Start()));
      return ( ap::dot(n,(ctr - S.Start())) < 0 ? n : coord_type(-n));
    }
  
  /*! \brief  Normal to the edge \e having the same length 
   */
  coord_type area_normal(const Edge& e) const
  {
    segment_type S(e,basic_geom());
    return ap::normal_with_same_length(S.End()-S.Start());
  }
  
  /*! \brief  Normalized normal to the edge \e 
   */
  coord_type normal(const Edge& e) const
  {
    segment_type S(e,basic_geom());
    return ap::normed_normal(S.End()-S.Start());
  }
  
  //! solid angle of the wedge of vertex \c vc, in radians (2D) 
  double solid_angle(VertexOnCellIterator const& vc) const 
  {
    typedef algebraic_primitives<coord_type> ap;
    Edge e1;
    Vertex v = *vc;
    for(EdgeOnCellIterator ec(vc.TheCell()); !ec.IsDone(); ++ec)
      if(v == (*ec).V1() || v == (*ec).V2()) {
	e1 = *ec;
	break;
      }
    Edge e2 = TheGrid().switched_edge(v,e1, vc.TheCell());
    coord_type dir_e1 = (coord(TheGrid().switched_vertex(v,e1)) - coord(v));
    coord_type dir_e2 = (coord(TheGrid().switched_vertex(v,e2)) - coord(v));
    return ap::angle(dir_e1, dir_e2);
  }

   /*! ratio of solid angle of wedge \c vc to complete solid angle

      The ratios of the wedges of an internal regular vertex sum up to 1.
   */
  double solid_angle_ratio(VertexOnCellIterator const& vc) const { return solid_angle(vc)/(2*M_PI);}


private:
  base const& basic_geom() const { return *this;}
};

} // namespace GrAL 

#endif
