#ifndef GRAL_GB_TRIANG3D_STORED_GEOMETRY_H
#define GRAL_GB_TRIANG3D_STORED_GEOMETRY_H

// $LICENSE_NEC_2003

#include "Gral/Grids/Triang3D/triang3d.h"
#include "Geometry/point-traits.h"
#include "Utility/pre-post-conditions.h"

#include <iostream>
#include <algorithm>
#include <cmath>

/*! \file
 */

/*! \brief Geometry wrapper class for Triang3D
   
    \ingroup triang3dmodule
 
   Model of $GrAL VertexGridGeometry.

   \see Test in \ref test-triang3d-geometry.C
   \todo It could be parameterised by scalar type of coordinates, 
    and by space dimension.
 */
class stored_geometry_triang3d {
public:
  typedef Triang3D              grid_type; 
  typedef grid_types<grid_type> gt;
  typedef gt::Vertex            Vertex;
  typedef gt::Edge              Edge;
  typedef gt::Facet             Facet;
  typedef gt::Cell              Cell;
  typedef gt::VertexOnCellIterator VertexOnCellIterator;
private:
  grid_type const* g;
  double         * xyz;
  bool             owned;
public:
  //! Empty geometry
  stored_geometry_triang3d() : g(0), xyz(0), owned(false) {}
  //! initialize with value semantics (coords are owned)
  stored_geometry_triang3d(grid_type const& g_)
    : g(&g_)
    { 
      init_xyz();
    }
  //! initialize with reference semantics (coords are referenced)
  stored_geometry_triang3d(grid_type const& g_, double* xyz_)
    : g(&g_), xyz(xyz_), owned(false) {}
  //! Destructor
  ~stored_geometry_triang3d() { clear();}

  //! Dimension of embedding space
  unsigned space_dimension() const { return 3;}

  //! change to value semantics
  void set_grid(grid_type const& g_) {
    clear();
    g = & g_;
    init_xyz();
  }

private:
  void clear() { if (owned) delete [] xyz; xyz = 0;}
  void init_xyz() {
    if(g->NumOfVertices() > 0) {
      xyz = new double[3*g->NumOfVertices()];
      owned = true;
    }
    else {
      xyz = 0;
      owned = false;
    }
  }

public:
  //! Anchor grid
  grid_type const& TheGrid() const { return *g;}

  class coord_type;
  class coord_proxy {
    double* xyz_v; // start address of xyz values of a vertex
    friend class coord_type;
  public:
    coord_proxy(double* xyz_v_) : xyz_v(xyz_v_) {}
    inline void operator=(coord_type const& coo);
    template<class COORD>
    void operator=(COORD const& coo);
   
    double  operator[](int i) const { cr(i); return xyz_v[i]; }
    double& operator[](int i)       { cr(i); return xyz_v[i]; }

  private:
    void cr(int i) const {      
      REQUIRE( 0 <= i && i <  3, "i = " << i << " out of range!\n",1);
    }
  };

  //! \brief coordinate representation
  class coord_type : public array_operators<coord_type, double, 3> {
    double xyz[3];
  public:
    coord_type() {}
    coord_type(coord_proxy p)               { init(p.xyz_v);}
    explicit coord_type(double const* xyz_) { init(xyz_);}
    explicit coord_type(double d) { xyz[0] = xyz[1] =xyz[2] = d;}
    coord_type(double x, double y, double z) { xyz[0] = x; xyz[1] = y; xyz[2] = z;}
    coord_type& operator=(coord_proxy p) { init(p.xyz_v); return *this;}


    double  operator[](int i) const { cr(i); return xyz[i]; }
    double& operator[](int i)       { cr(i); return xyz[i]; }

  private:
    void cr(int i) const {      
      REQUIRE( 0 <= i && i <  3, "i = " << i << " out of range!\n",1);
    }
    void init(double const* p)
      { xyz[0] = p[0]; xyz[1] = p[1]; xyz[2] = p[2]; }
  };
  //! A type for real numbers
  typedef double scalar_type;

  //! coordinate of Vertex (read/write access)
  coord_proxy coord(Vertex const& v) 
    { return coord_proxy(xyz + 3*v.handle());}
  //! coordinate of Vertex (read access)
  coord_type  coord(Vertex const& v) const 
    { return coord_type (xyz + 3*v.handle());}

  //! Length of an edge
  inline scalar_type length(Edge const& e) const;

  //! area of a facet
  inline scalar_type area(Facet const& f)  const;

  //! 1D-volume of edge
  inline scalar_type volume(Edge const& e) const { return length(e);}
  //! 2D-volume of facet
  inline scalar_type volume(Facet const& f)  const { return area(f);}
  //! 3D-volume of cell
  inline scalar_type volume(Cell  const& c) const;


  //! Center of inertia of \c c
  coord_type center(Cell const& c) const 
    { return (coord(c.V(0)) + coord(c.V(1)) + coord(c.V(2)) + coord(c.V(3)))/4.0;}

  //! Barycenter/Centroid (average of vertices) of cell \c c
  coord_type barycenter(Cell const& c) const { return center(c);}

  //! solid angle of the wedge of vertex \c vc, in steradians (3D)   
  scalar_type solid_angle(VertexOnCellIterator const& vc) const;

  /*! \brief ratio of solid angle of wedge \c vc to complete solid angle

      The ratios of the wedges of an internal regular vertex sum up to 1.
   */
  scalar_type solid_angle_ratio(VertexOnCellIterator const& vc) const { return solid_angle(vc)/(4*M_PI);}

};


inline std::ostream&
operator<<(std::ostream& out, stored_geometry_triang3d::coord_type const& p)
{ return (out << p[0] << ' ' << p[1] << ' ' << p[2]);}

inline std::istream&
operator>>(std::istream&  in, stored_geometry_triang3d::coord_type & p)
{ return (in >> p[0] >> p[1] >> p[2]);}


inline void
stored_geometry_triang3d::coord_proxy::operator=
(stored_geometry_triang3d::coord_type const& p)
{  xyz_v[0] = p[0]; xyz_v[1] = p[1]; xyz_v[2] = p[2];  }
template<class COORD>

inline void
stored_geometry_triang3d::coord_proxy::operator=
(COORD const& p)
{ 
  typedef point_traits<COORD> pt;
  xyz_v[0] = pt::x(p);
  xyz_v[1] = pt::y(p);
  xyz_v[2] = pt::z(p);
}

template<>
struct point_traits<stored_geometry_triang3d::coord_type> 
  : public point_traits_fixed_size_array
        <stored_geometry_triang3d::coord_type,double,3> {};

template<>
struct point_traits<stored_geometry_triang3d::coord_proxy> 
  : public point_traits_fixed_size_array
        <stored_geometry_triang3d::coord_proxy,double,3> 
{
  typedef Ptype reference_type;
  static void ConstructWithDim(unsigned d, Ptype)
    { REQUIRE( d == 3, "d = " << d, 1); }
};

template<class Q>
inline void assign_point(stored_geometry_triang3d::coord_proxy p,
			 Q const& q) 
{
 typedef point_traits<stored_geometry_triang3d::coord_proxy> ptP;
 typedef point_traits<Q> ptQ;

 REQUIRE( (ptP::Dim(p) == ptQ::Dim(q)), 
	  "Cannot assign points of different dimension!" 
	  << "(dim(p) = " << ptP::Dim(p)  << ", "
	  << "(dim(q) = " << ptQ::Dim(q), 1);
 
 int iq = ptQ::LowerIndex(q); 
 int ip = ptP::LowerIndex(p);  
 for(; iq <= ptQ::UpperIndex(q); ++ip, ++iq)
   p[ip] = q[iq]; 
}  


inline stored_geometry_triang3d::scalar_type 
stored_geometry_triang3d::length(stored_geometry_triang3d::Edge const& e) const 
{ 
  typedef algebraic_primitives<coord_type> ap;
  return ap::distance(coord(e.V1()), coord(e.V2()));
}

inline stored_geometry_triang3d::scalar_type 
stored_geometry_triang3d::area(stored_geometry_triang3d::Facet const& f) const
{
  typedef algebraic_primitives<coord_type> ap;
  gt::VertexOnFacetIterator vf(f);
  coord_type p0 = coord(*vf); ++vf;
  coord_type p1 = coord(*vf); ++vf;
  coord_type p2 = coord(*vf);
  return ap::triangle_area(p0,p1,p2);
}

inline stored_geometry_triang3d::scalar_type 
stored_geometry_triang3d::volume(stored_geometry_triang3d::Cell  const& c) const
{ 
  typedef algebraic_primitives<coord_type> ap;
  coord_type v0 = coord(c.V(0));
  return fabs(1.0/6.0 * ap::det3(coord(c.V(1))-v0, coord(c.V(2))-v0, coord(c.V(3))-v0));
}

inline stored_geometry_triang3d::scalar_type 
stored_geometry_triang3d::solid_angle
(stored_geometry_triang3d::VertexOnCellIterator const& vc) const
{
  typedef algebraic_primitives<coord_type> ap;
  Cell c(vc.TheCell());
  int v[4] = {0,1,2,3};
  std::swap(v[0], v[vc.local_handle()]);

  coord_type d1 = ap::normalization(coord(c.V(v[1])) - coord(c.V(v[0])));
  coord_type d2 = ap::normalization(coord(c.V(v[2])) - coord(c.V(v[0])));
  coord_type d3 = ap::normalization(coord(c.V(v[3])) - coord(c.V(v[0])));

  // Euler-Eriksson-Formula, see R\o{a}de/Westergren, BETA Mathematics Handbook for Science and 
  // Engineering, Studentlitteratur
  return 2*atan( fabs(ap::det3(d1,d2,d3)) / (1 + ap::dot(d1,d2) + ap::dot(d2,d3) + ap::dot(d1,d3)));
}

#endif
