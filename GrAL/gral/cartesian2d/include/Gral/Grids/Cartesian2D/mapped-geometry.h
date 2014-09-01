#ifndef NMWR_GB_GEOM_PRIMITIVES_FOR_CARTESIAN2D_H
#define NMWR_GB_GEOM_PRIMITIVES_FOR_CARTESIAN2D_H



/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


/*! \file
 */

#include "Config/compiler-config.h"
#include "Geometry/algebraic-primitives.h"

#include "Gral/Base/common-grid-basics.h"
#include "Gral/Geometries/geometric-types-2d.h"
#include "Gral/Grids/Cartesian2D/cartesian-grid2d.h"
#include "Gral/Grids/Cartesian2D/geometry-extensions2d.h"

#include "Utility/pre-post-conditions.h"

namespace GrAL {

namespace cartesian2d {




template<class CoordMap>
class mapped_geometry_base 
  : public grid_types<RegGrid2D>
{
public:
  //  typedef typename CoordMap::coord_type  coord_type;
  typedef grid_geometry_category              category;

  typedef CoordMap                            mapping_type;
  typedef typename mapping_type::result_type  coord_type;
  typedef RegGrid2D                           grid_type;
  typedef typename RegGrid2D::index_type      index_type;
  typedef point_traits<coord_type>            pt;


  //@{
  // @name Constructors
  mapped_geometry_base() :  g(0) {} 
  mapped_geometry_base(const CoordMap& ff, const RegGrid2D& gg) 
    : f(ff), g(&gg), ll(gg.ll()), ur(gg.ur())
  { init();}
  mapped_geometry_base(const CoordMap& ff, const RegGrid2D& gg,
			     const index_type& LL, const index_type& UR) 
    : f(ff), g(&gg), ll(LL), ur(UR) 
  { init();}
  //@}
  
  void init(RegGrid2D  const& gg, CoordMap const& ff,
	    index_type const& LL, index_type const& UR) 
  {
    g = &gg;
    f = ff;
    ll = LL;
    ur = UR;
    init();
  }
  void init() {
      dx = (ur.x() - ll.x() != 0 ? 1.0 / (ur.x() -ll.x()) 
	    : 0.0);
      dy = (ur.y() - ll.y() != 0 ? 1.0 / (ur.y() -ll.y()) 
	    : 0.0);
  }

  const grid_type& TheGrid()    const { cb(); return *g;}
  bool bound() const { return (g != 0); }
  void cb()    const { REQUIRE(bound(), "",1);}
  void rebind(grid_type    const& gg) { g = &gg; ll = g->ll(); ur = g->ur(); init();}
  void rebind(grid_type    const& gg,
	      mapping_type const& ff) { f = ff; rebind(gg);}

  const mapping_type&  TheMapping() const { return f;}
  
  // is this always possible?
  // unsigned space_dimension() const { return f.dim_of_image();}
  unsigned space_dimension() const { return pt::Dim(coord(* (TheGrid().FirstVertex())));}

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

template<class CM, class GEOM, unsigned DIM>
class dd_mapped_geom_reg2d 
  : public mapped_geometry_base<CM> 
{
  typedef mapped_geometry_base<CM> base;
public:
  typedef typename base::index_type index_type;

  dd_mapped_geom_reg2d() {} 

  dd_mapped_geom_reg2d(const CM& ff, const RegGrid2D& gg) 
    : mapped_geometry_base<CM>(ff,gg) {}

  dd_mapped_geom_reg2d(const CM& ff, const RegGrid2D& gg,
		       const index_type& LL, const index_type& UR) 
    : mapped_geometry_base<CM>(ff,gg,LL,UR) {}
};

/*! \brief Dimension specific functionality for 2D
    \internal
 */
template<class CM, class GEOM>
class dd_mapped_geom_reg2d<CM, GEOM, 2>
  : public mapped_geometry_base<CM> { 
  //   only useful if dim(coord_type) == 2 !

public:
  typedef  mapped_geometry_base<CM> base;
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

  /*! \brief Normed normal of facet \c nb
       
      \return
        Normal \f$n\f$  to facet \c nb
       -  \f$n\f$  is pointing away from \c nb.TheCell() 
       - \f$\|n\| = 1\f$ 
  */
  coord_type 
  outer_normal(FacetOnCellIterator const& nb) const 
  {
    return ap::normed_normal(coord(nb.V1()) - coord(nb.V2()));
  }
  
  /*! \brief Normal of facet \c nb
       
      \return
        Normal \f$n\f$  to facet \c nb
       -  \f$n\f$  is pointing away from \c nb.TheCell() 
       - The Euclidean norm  \f$\|n\|\f$ is equal to the volume (i.e. length) of the facet.
   */
  coord_type 
  outer_area_normal(FacetOnCellIterator const& nb) const 
  {
    return ap::normal_with_same_length(coord(nb.V1()) - coord(nb.V2()));
  }

  
  /*! \brief Normal of facet \c f
       
      \return
        Normal \f$n\f$  to facet \c f.
        The Euclidean norm  \f$\|n\|\f$ is equal to the volume (i.e. length) of the facet.
   */
  coord_type 
  area_normal(Facet const& f) const 
  {
    typename GEOM::segment_type S(f,basic_geom());
    return ap::normal_with_same_length(S.End()-S.Start());
  }
  
  /*! \brief Normed normal of facet \c f
       
      \return
        Normal \f$n\f$  to facet \c f with \f$\|n\|=1\f$.
   */
  coord_type 
  normal(Edge const& e) const 
  {
    typename GEOM::segment_type S(e, basic_geom());
    return ap::normed_normal(S.End()-S.Start());
  }
};

//----------- general geometry : include dimension dependend parts ----------

/*!  \brief  geometry for RegGrid2D with explicit mapping.
     \ingroup cartesian2dgeom

      This class implements a straight-line geometry for the
      grid class RegGrid2D. The coordinates of the vertices
      are given by a mapping from the unit-square [0,1]x[0,1]
      to R^2 (or R^3, in principle. There are some problems,
      however. See below).

      To this end, the given underlying grid G of type \c RegGrid2D
      is thought to be mapped onto the unit square, by default
      <tt> G.ll() -> (0.0,0.0) </tt>  and <tt> G.ur() -> (1.0,1.0)</tt>.
      A rectangle other than <tt>[G.ll(),G.ur()</tt> may be specified that is mapped
      to \f$[0,1]^2\f$. This is useful e.g. for grids with overlaps or that
      are components of a larger grid, occupying only a part of the logical
      entire grid.

       \templateparams
        + CoordMap:
         - typedef coord_type ( type of result)
         - typedef argument_type 
         - coord_type operator()(argument_type, 2D)
         - int dim_of_image()  (dim. of coord_type, 2D or 3D)

   \see \c dimension_dependend_extensions2d<BASE, GEOM, 2>  for additional functionality
        if the space dimension is 2.
   \see Test in \ref test-mapped-geometry.C
*/

template<class CM>
class mapped_geometry
  :   public dimension_dependend_extensions2d<mapped_geometry_base<CM>, 
					      mapped_geometry<CM>, 
					      point_traits<typename CM::result_type>::dimension>
{
  
public:
  /*
  typedef dd_mapped_geom_reg2d<CM, mapped_geometry<CM>,
                               point_traits<typename CM::result_type>::dimension>
  */
  typedef dimension_dependend_extensions2d<mapped_geometry_base<CM>, mapped_geometry<CM>, 
					  point_traits<typename CM::result_type>::dimension>
  base;
  typedef mapped_geometry_base<CM>  geom_base_type;
  using base::basic_geom;

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
  typedef mapped_geometry_base<CM>                   geom_base;

  //@{ 
  // @name Constructors
  //! Empty geometry
  mapped_geometry() {}
  //! Standard geometry
  mapped_geometry(const RegGrid2D& gg, const CM& ff = CM())
    { this->init(gg,ff, gg.ll(), gg.ur()); }
  //! Map \c [LL, UR] to \c ff(\f$[0,1]^2\f$\c)
  mapped_geometry( const RegGrid2D& gg,  const CM& ff,
		   const index_type& LL, const index_type& UR) 
    { this->init(gg,ff,LL,UR); }
  //@}

  /*! \brief geometric representation of edges

     \todo For non-linear mappings, this must be something more sophisticated.
   */
  typedef Segment<Edge,geom_base>   segment_type;
  /*! \brief geometric representation of faces (cells)

     \todo For non-linear mappings, this must be something more sophisticated.
   */
  typedef Polygon2d<Face,geom_base> polygon_type;

  //! get geometric representation for an edge
  segment_type segment(const Edge& e) const { return segment_type(e,basic_geom());}
  //! get geometric representation for a face (cell)
  polygon_type polygon(const Face& f) const { return polygon_type(f,basic_geom());}

  //! 1-dimensional volume of an edge
  double volume(const Edge& e) const  { return (segment_type(e,basic_geom()).length());}
  //! length of a (straight) edge
  double length(const Edge& e) const  { return (segment_type(e,basic_geom()).length());}

  // double volume(const EdgeIterator& e) const {return volume(*e);}
  //! 2-dimensional volume of a cell
  double area  (const Cell& c) const { return volume(c);}
  //! 2-dimensional volume of a cell
  double volume(const Cell& c) const { return (polygon_type(c,basic_geom()).area());}

  
  //! Midpoint of an edge
  coord_type center(const Edge& e) const {return(segment_type(e,basic_geom()).center());}
  
  //! Center of inertia of a cell 
  coord_type center(const Cell& c) const {return(polygon_type(c,basic_geom()).center());}

};

} // namespace cartesian2d

} // namespace GrAL 

#endif
