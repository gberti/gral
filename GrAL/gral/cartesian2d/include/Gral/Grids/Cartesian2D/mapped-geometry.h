#ifndef NMWR_GB_GEOM_PRIMITIVES_FOR_CARTESIAN2D_H
#define NMWR_GB_GEOM_PRIMITIVES_FOR_CARTESIAN2D_H


// $LICENSE

#include "Config/compiler-config.h"
#include "Geometry/algebraic-primitives.h"

#include "Gral/Base/common-grid-basics.h"
#include "Gral/Geometries/geometric-types.h"
#include "Gral/Grids/Cartesian2D/cartesian-grid2d.h"

/*!  \brief  geometry for RegGrid2D with explicit mapping.

      This class implements a straight-line geometry for the
      grid class RegGrid2D. The coordinates of the vertices
      are given by a mapping from the unit-square [0,1]x[0,1]
      to R^2 (or R^3, in principle. There are some problems,
      however. See below).

      To this end, the given underlying grid G of type RegGrid2D
      is thought to be mapped onto the unit square, by default
      G.ll() -> (0.0,0.0)  and G.ur() -> (1.0,1.0).
      A rectangle other than G.ll(),G.ur() may be specified that is mapped
      to [0,1]^2. This is useful e.g. for grids with overlaps or that
      are components of a larger grid, occupying only a part of the logical
      entire grid.

       \templateparams
        + CoordMap:
         - typedef coord_type ( type of result)
         - typedef argument_type 
         - coord_type operator()(argument_type, 2D)
         - int dim_of_image()  (dim. of coord_type, 2D or 3D)
*/

namespace cartesian2d {

template<class CoordMap>
class mapped_geometry_reg2d_base 
  : public grid_types<RegGrid2D>
{
public:
  //  typedef typename CoordMap::coord_type  coord_type;
  typedef typename CoordMap::result_type  coord_type;
  typedef RegGrid2D                      grid_type;
  typedef typename RegGrid2D::index_type index_type;

  mapped_geometry_reg2d_base() :  g(0) {} 

  //@{ @name Constructor
  mapped_geometry_reg2d_base(const CoordMap& ff, const RegGrid2D& gg) 
    : f(ff), g(&gg), ll(gg.ll()), ur(gg.ur())
    { 
      dx = (ur.x() - ll.x() != 0 ? 1.0 / (ur.x() -ll.x()) 
	    : 0.0);
      dy = (ur.y() - ll.y() != 0 ? 1.0 / (ur.y() -ll.y()) 
	    : 0.0);
    }
  mapped_geometry_reg2d_base(const CoordMap& ff, const RegGrid2D& gg,
				 const index_type& LL, const index_type& UR) 
    : f(ff), g(&gg), ll(LL), ur(UR) {
      dx = (ur.x() - ll.x() != 0 ? 1.0 / (ur.x() -ll.x()) 
	    : 0.0);
      dy = (ur.y() - ll.y() != 0 ? 1.0 / (ur.y() -ll.y()) 
	    : 0.0);
  }
  //@}

  const grid_type& TheGrid()    const { 
    REQUIRE((g != 0) , "no grid!\n",1);
    return *g;
  }
  const CoordMap&  TheMapping() const { return f;}
  
  // is this always possible?
  unsigned space_dimension() const { return f.dim_of_image();}

  coord_type coord(const Vertex& v) const  { 
 
    /*   return f(unit_xcoord(v), 
	 unit_ycoord(v));*/ // not always feasible
   typedef typename CoordMap::argument_type a_type;
   return f(a_type(unit_xcoord(v), 
		   unit_ycoord(v)));
  }

private:
  // canonical embedding into the unit 2-cube [0,1]x[0,1]
  double unit_xcoord(const Vertex& v)  const { return (v.x() -ll.x()) *dx;}
  double unit_ycoord(const Vertex& v)  const { return (v.y() -ll.y()) *dy;}

  double dx,dy;
  CoordMap f;  // should this be a reference?
  const grid_type* g; // reference
  index_type ll, ur;  // rect. [ll,ur] is mapped to [0,1]^2
};

//--------------------- dimension dependend geometry parts --------------

template<class CM, class GEOM, class DIM>
class dd_mapped_geom_reg2d 
  : public mapped_geometry_reg2d_base<CM> 
{
  typedef mapped_geometry_reg2d_base<CM> base;
public:
  typedef typename base::index_type index_type;

  dd_mapped_geom_reg2d() {} 

  dd_mapped_geom_reg2d(const CM& ff, const RegGrid2D& gg) 
    : mapped_geometry_reg2d_base<CM>(ff,gg) {}

  dd_mapped_geom_reg2d(const CM& ff, const RegGrid2D& gg,
		       const index_type& LL, const index_type& UR) 
    : mapped_geometry_reg2d_base<CM>(ff,gg,LL,UR) {}
};


template<class CM, class GEOM>
class dd_mapped_geom_reg2d<CM, GEOM, tag2D>
  : public mapped_geometry_reg2d_base<CM> { 
  //   only useful if dim(coord_type) == 2 !

public:
  typedef  mapped_geometry_reg2d_base<CM> base;
  typedef typename base::Vertex Vertex;
  typedef typename base::Edge   Edge;
  typedef typename base::Facet  Facet;
  typedef typename base::Cell   Cell;

  typedef typename base::CellIterator        CellIterator;
  typedef typename base::CellOnCellIterator  CellOnCellIterator;
  typedef typename base::FacetOnCellIterator FacetOnCellIterator;

  typedef typename base::coord_type coord_type;
  typedef typename base::index_type index_type;

protected:
  GEOM      * _sub()       { return static_cast<GEOM      *>(this);}
  GEOM const* _sub() const { return static_cast<GEOM const*>(this);}

  base       & basic_geom()       { return *this;}
  base  const& basic_geom() const { return *this;}
public:
  dd_mapped_geom_reg2d() {} 

  dd_mapped_geom_reg2d(const CM& ff, const RegGrid2D& gg) 
    : base(ff,gg) {}

  dd_mapped_geom_reg2d(const CM& ff, const RegGrid2D& gg,
		       const index_type& LL, const index_type& UR) 
    : base(ff,gg,LL,UR) {}


  // coord_type from base
  typedef algebraic_primitives<coord_type>  ap;

  coord_type 
  normal(CellIterator const& ,
	 CellOnCellIterator const& nb) const  // |.| == 1
  {
    Edge e(nb);
    typename GEOM::segment_type S(e, basic_geom());
    return ap::normed_normal(S.End()-S.Start());
  }
  
  coord_type 
  area_normal(CellIterator const& ,
	      CellOnCellIterator const& nb) const 
  {
    Edge e(nb);
    typename GEOM::segment_type S(e, basic_geom());
    return ap::normal_with_same_length(S.End()-S.Start());
  }
  
  coord_type 
  area_normal(Edge const& e) const 
  {
    typename GEOM::segment_type S(e,basic_geom());
    return ap::normal_with_same_length(S.End()-S.Start());
  }
  
  coord_type 
  normal(Edge const& e) const 
  {
    typename GEOM::segment_type S(e, basic_geom());
    return ap::normed_normal(S.End()-S.Start());
  }
  
  
  coord_type outer_area_normal(FacetOnCellIterator const& f) const 
  {
    return ap::normal_with_same_length(coord(f.V1()) - coord(f.V2()));
  }

  
};

//----------- general geometry : include dimension dependend parts ----------

template<class CM>
class mapped_geometry
  : 
  public  dd_mapped_geom_reg2d<CM, mapped_geometry<CM>,
                               typename point_traits<typename CM::result_type>::dimension_tag>
{
  
public:
  typedef dd_mapped_geom_reg2d<CM, mapped_geometry<CM>,
                               typename point_traits<typename CM::result_type>::dimension_tag>
  base;

  typedef typename base::Vertex Vertex;
  typedef typename base::Edge   Edge;
  typedef typename base::Face   Face;
  typedef typename base::Facet  Facet;
  typedef typename base::Cell   Cell;

  typedef typename base::CellIterator        CellIterator;
  typedef typename base::CellOnCellIterator  CellOnCellIterator;
  typedef typename base::FacetOnCellIterator FacetOnCellIterator;

  typedef typename base::coord_type coord_type;
  typedef typename base::index_type index_type;

  typedef CM mapping_type;

  typedef point_traits<coord_type>                         pt;
  typedef mapped_geometry_reg2d_base<CM>                   geom_base;
 
  mapped_geometry() {}

  mapped_geometry(const RegGrid2D& gg, const CM& ff)
    : base(ff,gg) {}

  mapped_geometry( const RegGrid2D& gg,  const CM& ff,
		   const index_type& LL, const index_type& UR) 
    : base(ff,gg,LL,UR) {}

  typedef Segment<Edge,geom_base>   segment_type;
  typedef Polygon2d<Face,geom_base> polygon_type;

  segment_type segment(const Edge& e) const { return segment_type(e,basic_geom());}
  polygon_type polygon(const Face& f) const { return polygon_type(f,basic_geom());}

  double volume(const Edge& e) const  { return (segment_type(e,basic_geom()).length());}
  double length(const Edge& e) const  { return (segment_type(e,basic_geom()).length());}
  // double volume(const EdgeIterator& e) const {return volume(*e);}
  double volume(const Cell& c) const { return (polygon_type(c,basic_geom()).area());}
  
  coord_type center(const Edge& e) const {return(segment_type(e,basic_geom()).center());}
  
  // center of inertia 
  coord_type center(const Cell& c) const {return(polygon_type(c,basic_geom()).center());}

};

} // namespace cartesian2d


#endif
