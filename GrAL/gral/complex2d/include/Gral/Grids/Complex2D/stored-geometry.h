#ifndef NMWR_GB_GEOM_PRIMITIVES_FOR_COMPLEX_H
#define NMWR_GB_GEOM_PRIMITIVES_FOR_COMPLEX_H


//----------------------------------------------------------------
//   (c) Guntram Berti, 1997
//   Chair for Numerical Mathematics & Scientific Computing (NMWR)
//   TU Cottbus - Germany
//   http://math-s.math.tu-cottbus.de/NMWR
//   
//----------------------------------------------------------------
#include "Config/compiler-config.h"

#include "Geometry/algebraic-primitives.h"

#include "Gral/Base/common-grid-basics.h"
#include "Gral/Geometries/geometric-types.h" 
#include "Gral/Grids/Complex2D/complex2d.h"


struct stored_geometry_complex2D_base
  : public grid_types<Complex2D> {
public:
  typedef Complex2D grid_type;
protected:
public:
  grid_type *  g;
public:
  typedef  grid_type::CoordType     coord_type;
  typedef point_traits<coord_type>          pt;
  typedef algebraic_primitives<coord_type>  ap;

  typedef  pt::component_type       real_type;

  stored_geometry_complex2D_base() : g(0) {}

  // we cast const away, because non-const reference is only needed for
  // write-access to coordinates, and that conceptually does not belong into
  // the grids responsibility
  stored_geometry_complex2D_base(grid_type const& gg) : g((grid_type*) &gg) {}
  void set_grid(grid_type const& gg) { g = (grid_type*) (&gg);}

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


struct stored_geometry_complex2D
 : public stored_geometry_complex2D_base
{
public:
  typedef stored_geometry_complex2D_base base;
  //private:
  // Complex2D* g;
public:  
  typedef stored_geometry_complex2D self;
  stored_geometry_complex2D() {}
  stored_geometry_complex2D(grid_type const& gg) : base(gg)  {}

  typedef Segment<Edge,base>               segment_type;
  typedef Polygon2d<Cell,base>             polygon_type;
  typedef algebraic_primitives<coord_type> ap;


  segment_type segment(const Edge& E) const { return segment_type(E,basic_geom());}
  polygon_type polygon(const Face& F) const { return polygon_type(F,basic_geom());}

  double length(const Edge& e) const { return (segment_type(e,basic_geom()).length());}

  double volume(const Edge& e) const { return (segment_type(e,basic_geom()).length());}
  double volume(const EdgeIterator& e) const {return volume(*e);}
  double volume(const Cell& c) const { 
    // IOMgr::Info() << "stored_geom::volume(Cell) " << endl;
    // IOMgr::Info() << "Volume(): " << (void*) &(TheGrid()) << endl;
    //    base const& bs (basic_geom());
    // IOMgr::Info() << "bs.g: " << (void*) bs.g << endl;
    polygon_type poly(c,basic_geom());
    return (poly.area());
    //  return (polygon_type(c,basic_geom()).area());
  }

  double diameter(Cell const& c)  const {
    double diam2 = 0.0;
    for(VertexOnCellIterator vc(c); ! vc.IsDone(); ++vc) {
      VertexOnCellIterator w = vc; 
      ++w;
      for(; !w.IsDone(); ++w)
        diam2 = max(diam2, ap::distance2(coord(*vc),coord(*w)));
    }
    return sqrt(diam2);
  }

  coord_type center(const Edge& e) const {return(segment_type(e,basic_geom()).center());}

  // center of inertia
  coord_type center(const Cell& c) const {return(polygon_type(c,basic_geom()).center());}


  coord_type outer_normal(/*const CellIterator& c, */
			  const FacetOnCellIterator& nb) const 
    //const NeighbourCellIterator& nb) const
    {
      coord_type ctr(center(nb.TheCell()));
      Edge e(nb);
      segment_type S(e,basic_geom());
      coord_type n( ap::normed_normal(S.End()-S.Start()));
      return ( ap::dot(n,(ctr - S.Start())) < 0 ? n : coord_type(-n));
    }

 
  coord_type outer_area_normal(const FacetOnCellIterator& f) const 
    //    const NeighbourCellIterator& nb) const
    {
      coord_type ctr(center(f.TheCell()));
      Edge e(*f);
      segment_type S(e,basic_geom());
      coord_type n(ap::normal_with_same_length(S.End() - S.Start()));
      return ( ap::dot(n,(ctr - S.Start())) < 0 ? n : coord_type(-n));
    }

  coord_type outer_area_normal(const CellNeighbourIterator& nb) const
    {
      coord_type ctr(center(nb.TheCell()));
      Edge e(nb);
      segment_type S(e,basic_geom());
      coord_type n(ap::normal_with_same_length(S.End() - S.Start()));
      return ( ap::dot(n,(ctr - S.Start())) < 0 ? n : coord_type(-n));
    }
  
  
  coord_type area_normal(const Edge& e) const
  {
    segment_type S(e,basic_geom());
    return ap::normal_with_same_length(S.End()-S.Start());
  }
  
  coord_type normal(const Edge& e) const
  {
    segment_type S(e,basic_geom());
    return ap::normed_normal(S.End()-S.Start());
  }
private:
  base const& basic_geom() const { return *this;}
};



#endif
