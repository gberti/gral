#ifndef GRAL_BG_GRIDS_CARTESIAN3D_MAPPED_GEOMETRY_H
#define GRAL_BG_GRIDS_CARTESIAN3D_MAPPED_GEOMETRY_H

// $LICENSE

#include "Gral/Grids/Cartesian3D/cartesian3d.h"

namespace GrAL {

namespace cartesian3d {

  /*! A mapped geometry for CartesianGrid3D
     \ingroup cartesian3dmodule

     \templateparams
     - \c CoordMap
        - type \c result_type (called \c coord_type below)
        - <tt> coord_type operator()(coord_type) </tt>
  */
template<class CoordMap>
class mapped_geometry {

public:
  typedef CoordMap mapping_type;
  typedef typename mapping_type::result_type coord_type;
  typedef CartesianGrid3D grid_type;
  typedef grid_types<grid_type> gt;
  typedef gt::index_type        index_type;
  typedef gt::Cell              Cell;
  typedef gt::Vertex            Vertex;
  typedef point_traits<coord_type> pt;
private:
  grid_type const* g;
  mapping_type f;
  double dx,dy,dz;

public:
  //! Empty geometry
  mapped_geometry() : g(0) {}
  //! Init with grid and mapping
  mapped_geometry(grid_type    const& gg,
		  mapping_type const& ff = mapping_type())
    : g(&gg), f(ff)
    {
      init();
    }
  /*! \brief  Delayed construction
       
      <tt> mapped_geometry<MAP> M; M.init(gg,ff); </tt>
      <br> is equivalent to
      <tt> mapped_geometry<MAP> M(gg,ff);</tt>
   */
  void init(grid_type    const& gg,
	    mapping_type const& ff = mapping_type())
    {
      g = &gg; 
      f = ff;
      init();
    }

  bool bound() const { return g != 0;}
  void cb()    const { REQUIRE(bound(), "",1);}
  void rebind(grid_type    const& gg) { g = &gg; init();}
  void rebind(grid_type    const& gg,
	      mapping_type const& ff) { init(gg,ff);}

  //! Dimension of embedding defined by the mapping
  unsigned space_dimension() const { return pt::Dim(coord(* (TheGrid().FirstVertex())));}

  //  coord_type coord(Vertex const& v) const { return f(x(v),y(v),z(v));}
  //! Coordinate of vertex
  coord_type coord(Vertex const& v) const { 
    index_type i(v.index()); 
    return f(coord_type(x(i),y(i),z(i)));
  }

  /*! Center of cell 
      \note Result is the mapping of the center of \c c 
       in unit coordinate defined in \f$[0,1]^3\f$.
       In the case of a non-linear map, this is only an approximation 
       to the real center.
  */
  coord_type center(Cell const& c) const { 
    index_type i=c.index(); 
    return f(coord_type(x(i)+0.5*dx,y(i)+0.5*dy,z(i)+0.5*dz));
  }

  //! Anchor grid
  grid_type    const& TheGrid() const { return *g;}
  //! Mapping
  mapping_type const& TheMap()  const { return f;}

private:
  double x(index_type i) const { return dx*i[0];}
  double y(index_type i) const { return dy*i[1];}
  double z(index_type i) const { return dz*i[2];}

  double x(Vertex const& v) const { return x(v.index());}
  double y(Vertex const& v) const { return y(v.index());}
  double z(Vertex const& v) const { return z(v.index());}

  void init()
    {
      dx = 1.0 / g->NumOfXCells();
      dy = 1.0 / g->NumOfYCells();
      dz = 1.0 / g->NumOfZCells();
    } 
};

} // namespace cartesian3d

} // namespace GrAL 

#endif
