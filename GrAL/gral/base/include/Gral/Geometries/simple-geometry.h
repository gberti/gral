#ifndef GRAL_GB_BASE_SIMPLE_GEOMETRY_H
#define GRAL_GB_BASE_SIMPLE_GEOMETRY_H

// $LICENSE


#include "Gral/Base/common-grid-basics.h"
#include "Gral/Base/flag.h"

#include "Utility/pre-post-conditions.h"
#include "Geometry/point-traits.h"
#include "Geometry/algebraic-primitives.h"
#include "Gral/Geometries/geometric-types-2d.h"


namespace GrAL {

  template<class GEOM, class GRID, class COORD,unsigned DIM>
  class solid_angle_plugin_dim { };

  template<class GEOM, class GRID, class COORD, unsigned DIM> // GRID, class COORD, unsigned DIM>
  class solid_angle_plugin : public solid_angle_plugin_dim<GEOM, GRID, COORD, DIM> // GEOM::dim> 
  {
  public:
    // GEOM derives from this
    typedef GEOM geom_type;
    geom_type const& TheGeom() const { return static_cast<geom_type const&>(*this);}
  };


  /*! \brief Extension of simple_geometry for 3D meshes

      \see simple_geometry
      \todo Move this into separate header
  */
  template<class GEOM, class GRID, class COORD>
  class solid_angle_plugin_dim<GEOM,GRID, COORD, 2> {
    typedef GEOM                     geom_type;
    typedef solid_angle_plugin<GEOM, GRID,  COORD, 2> derived;
    geom_type const& TheGeom() const { return static_cast<derived const&>(*this).TheGeom();}
    
    //    typedef typename geom_type::grid_type   grid_type;
    typedef GRID grid_type;
    //    typedef typename geom_type::coord_type  coord_type;
    typedef COORD coord_type;
    typedef point_traits<coord_type>        pt;
    typedef typename pt::component_type scalar_type;
    typedef grid_types<grid_type>            gt;
    typedef typename gt::VertexOnCellIterator VertexOnCellIterator;

  public:
    
    //! solid angle of the wedge of vertex \c vc, in radians (2D)   
    scalar_type solid_angle      (VertexOnCellIterator vc) const;
    /*! \brief ratio of solid angle of wedge \c vc to complete solid angle
      
      The ratios of the wedges of an internal regular vertex sum up to 1 (for a flat grid).
      This is useful for dimension-independent algorithms.
   */
    scalar_type solid_angle_ratio(VertexOnCellIterator vc) const { return solid_angle(vc) / (2* M_PI);}
  };

  template<class GEOM, class GRID, class COORD>
  inline
  typename solid_angle_plugin_dim<GEOM, GRID, COORD, 2>::scalar_type
  solid_angle_plugin_dim<GEOM,GRID, COORD, 2>::solid_angle(typename solid_angle_plugin_dim<GEOM, GRID, COORD, 2>::VertexOnCellIterator vc) const
  {
    typedef algebraic_primitives<coord_type> ap;
    typename gt::Edge e1;
    typename gt::Vertex v = *vc;
    for(typename gt::EdgeOnCellIterator ec(vc.TheCell()); !ec.IsDone(); ++ec)
      if(v == (*ec).V1() || v == (*ec).V2()) {
	e1 = *ec;
	break;
      }
    typename gt::Edge e2 = vc.TheGrid().switched_edge(v,e1, vc.TheCell());
    coord_type dir_e1 = (TheGeom().coord(vc.TheGrid().switched_vertex(v,e1)) - TheGeom().coord(v));
    coord_type dir_e2 = (TheGeom().coord(vc.TheGrid().switched_vertex(v,e2)) - TheGeom().coord(v));
    return ap::angle(dir_e1, dir_e2);
  }


  /*! \brief Extension of simple_geometry for 3D meshes

      \see simple_geometry
  */
  template<class GEOM, class GRID, class COORD>
  class solid_angle_plugin_dim<GEOM,GRID, COORD, 3> {
    typedef GEOM                     geom_type;
    typedef solid_angle_plugin<GEOM, GRID,  COORD, 3> derived;
    geom_type const& TheGeom() const { return static_cast<derived const&>(*this).TheGeom();}

    typedef GRID                        grid_type;
    typedef COORD                       coord_type;
    typedef point_traits<COORD>         pt;
    typedef typename pt::component_type scalar_type;
    typedef grid_types<GRID>            gt;
    typedef typename gt::VertexOnCellIterator VertexOnCellIterator;

  public:

    //! solid angle of the wedge of vertex \c vc, in steradians (3D)   
    scalar_type solid_angle(typename gt::VertexOnCellIterator vc) const;

    /*! \brief ratio of solid angle of wedge \c vc to complete solid angle
      
      The ratios of the wedges of an internal regular vertex sum up to 1.
      This is useful for dimension-independent algorithms.
   */
    scalar_type solid_angle_ratio(VertexOnCellIterator vc) const { return solid_angle(vc) / (4 * M_PI);}
  };

  template<class GEOM, class GRID, class COORD>
  inline
  typename solid_angle_plugin_dim<GEOM, GRID, COORD, 3>::scalar_type
  solid_angle_plugin_dim<GEOM, GRID, COORD, 3>::solid_angle
  (typename solid_angle_plugin_dim<GEOM, GRID, COORD, 3>::VertexOnCellIterator vc) const
  {
    typedef algebraic_primitives<coord_type> ap;
    typename gt::Cell c(vc.TheCell());
    typename gt::Vertex v0(*vc);
    flag<typename gt::grid_type> F(v0, c);
    
    typename gt::Vertex v[4];
    v[0] = v0;
    v[1] = F.switched_vertex();
    F.switch_edge();
    F.switch_facet();
    v[2] = F.switched_vertex();
    F.switch_edge();
    F.switch_facet();
    v[3] = F.switched_vertex();

    typename gt::Vertex v1 = v[1];

    scalar_type res(0.0);
    while(v[3] != v1) {
      coord_type d[3];
      for(int i = 0; i < 3; ++i)
	d[i] = ap::normalization(TheGeom().coord(v[i+1]) - TheGeom().coord(v[0]));
      // Euler-Eriksson-Formula, see R\o{a}de/Westergren, BETA Mathematics Handbook for Science and 
      // Engineering, Studentlitteratur
      // This should work even for non-convex vertex figures (spherical polygons)
      res += 2*atan( ap::det3(d[0],d[1],d[2]) / ( 1 + ap::dot(d[0],d[1]) + ap::dot(d[1],d[2]) + ap::dot(d[0],d[2])));
      v[1] = v[2];
      v[2] = v[3];
      F.switch_edge();
      F.switch_facet();
      v[3] = F.switched_vertex();
    }
    return fabs(res);
  }



/*! \brief Simple geometry, just storing vertex coordinates

    \ingroup  gridgeometries
    \see \ref gridgeometries module
    \see Tested in \ref test-simple-geometry.C

      \todo Move plugin stuff like solid_angle to extra header, and make it optional.

 */
template<class GRID, class COORD> 
class simple_geometry : 
    public grid_types<GRID>,
    public solid_angle_plugin<simple_geometry<GRID, COORD>, GRID, COORD, GRID::dim > 
{
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

  enum { dim = grid_type::dim };
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
