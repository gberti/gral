#ifndef GRAL_BASE_GB_GEOMETRIC_TEMPLATE_TYPES_FOR_GRIDS
#define GRAL_BASE_GB_GEOMETRIC_TEMPLATE_TYPES_FOR_GRIDS



// $LICENSE

#include "Config/compiler-config.h"
#include "Geometry/algebraic-primitives.h"
#include "Gral/Base/common-grid-basics.h"
#include "Utility/ref-ptr.h"

namespace GrAL {


/*! \defgroup geometrictypes2d   Geometric types for 2D (obsolete)

    \ingroup gridgeometries
    \see \ref gridgeometries module

    Generic geometric types for combinatorial grid elements in 2D.


   These geometric types assume a linear geometry, segments
   are straight lines.

   The classes can be specialized to concrete grid elements, 
   in order to remove layers of indirection or use  additional
   knowledge. <BR>
   They \e must be specialized if the underlying assumptions (linearity)
   are violated.

   \note The classes in this module will be replaced by the \ref geometrictypes module
   \see Module \ref geometries
   \see Module \ref geometricfunctors
*/


/*! \brief  Geometric segment corresponding to combinatorial edge
   \ingroup geometrictypes2d

    
   \templateparams
    - Edge: model of $GrAL GridEdge
    - geom: model of $GrAL VertexGridGeometry

   \see Module \ref geometrictypes2d
*/
template<class Edge, class geom>
class Segment {
  typedef typename geom::coord_type        coord_type;
  typedef algebraic_primitives<coord_type> ap;
  typedef typename ap::real                real_type;
public:
  Segment(const Edge& e, const geom& gg) : _e(e), g(gg) {}
  coord_type Start()  const { return g.coord(_e.V1());}
  coord_type End()    const { return g.coord(_e.V2());}
  coord_type center() const { return (0.5*(Start()+End()));}
  double     length() const { return( ap::norm_2(Start()-End()));} 

  coord_type operator()(real_type t) const { return ((1-t)*Start() + t*End());}
private:
  Edge _e;
  const geom& g;
};

template<class Face, class geom>
class vertex_iterator_Polygon2d;

template<class Face, class geom>
inline bool operator==(vertex_iterator_Polygon2d<Face,geom> const&,
		      vertex_iterator_Polygon2d<Face,geom> const&);
template<class Face, class geom>
inline bool operator!=(vertex_iterator_Polygon2d<Face,geom> const&,
		      vertex_iterator_Polygon2d<Face,geom> const&);
template<class Face, class geom>
inline bool operator<(vertex_iterator_Polygon2d<Face,geom> const&,
		      vertex_iterator_Polygon2d<Face,geom> const&);

template<class Face, class geom> class Polygon2d;

/*! \internal
   \brief Iterator over vertices of Polygon2d
   \ingroup geometrictypes2d

   \templateparams
   (same as for Polygon2d<Face,geom>)
   - Face $GrAL GridFace
   - geom $GrAL VertexGridGeometry

   \see Polygon2d
   \see Module \ref geometrictypes2d
 */
template<class Face, class geom>
class vertex_iterator_Polygon2d {
private:
  unsigned i;
  Polygon2d<Face,geom> const* p;
  
  typedef vertex_iterator_Polygon2d<Face,geom> self;
public:
  //  typedef typename  Polygon2d<Face,geom>::coord_type value_type;
  typedef typename  geom::coord_type value_type;
  
  vertex_iterator_Polygon2d(int ii, Polygon2d<Face,geom> const& pp) : i(ii), p(&pp) {}
  
  self& operator++() { ++i; return *this;}
  value_type /*const&*/  operator*() const;
  
  friend bool operator== <> (self const& lhs, self const& rhs);
  friend bool operator!= <> (self const& lhs, self const& rhs);
  friend bool operator<  <> (self const& lhs, self const& rhs);
};


/*! \brief  Geometric polygon corresponding to geometric face
   \ingroup geometrictypes2d

   \templateparams
   (same as for vertex_iterator_Polygon2d<Face,geom>)
   - \c Face: model of $GrAL GridFace
   - \c geom: model of $GrAL VertexGridGeometry

   \see vertex_iterator_Polygon2d<Face,geom>
   \see Module \ref geometrictypes2d
 */
template<class Face, class geom>
class Polygon2d {
public:
  typedef typename geom::coord_type        coord_type;
  typedef algebraic_primitives<coord_type> ap;

  typedef point_traits<coord_type>    pt;
  typedef typename ap::real           real_type;

  typedef typename geom::grid_type grid_type;
  typedef grid_types<grid_type>    gt;
  typedef typename gt::Vertex      Vtx;
public:

  //--------------------- construction -------------------------------

  Polygon2d(const Face& f, const geom& gg) : _f(f), g(gg) {}
  const geom& TheGeometry() const { return g;}


  //------------------- iteration ----------- ------------------------

  //! \brief forward iterator over vertices
  typedef vertex_iterator_Polygon2d<Face,geom>  vertex_iterator;
  //! \brief beginning of vertex sequence
  vertex_iterator FirstVertex() const { return vertex_iterator(0,*this);}
  //! \brief past-the-end of vertex sequence
  vertex_iterator EndVertex()   const { return vertex_iterator(NumOfVertices(),*this);}

  //! number of vertices (size of vertex sequence)
  int               NumOfVertices() const {return _f.NumOfVertices();}

  //! \name Vertex coordinates
  //@{
  coord_type  V(int i)      const { return TheGeometry().coord(_f.V(i)); }
  coord_type  Vertex(int i) const { return V(i);} 
  coord_type  coord(Vtx v)  const { return TheGeometry().coord(v);}
  //@}

  //--------------------- geometric functions ------------------------

  //! mass center of the vertices
  coord_type barycenter() const 
    { 
      coord_type c(pt::Origin(pt::Dim(V(0)))); //TheGeometry().space_dimension()));
      for(int i =0; i< NumOfVertices(); i++) 
	c+= V(i);
      return (c/(double)NumOfVertices());
    }

  //! center of inertia
  coord_type center() const
    {
     coord_type c;
     double     a;
     get_polygon2d_center_and_area(FirstVertex(),EndVertex(),c,a);
     return c;
    }

  /*! \brief polygon area 
   
     Works for general simple polygons, in space dimension 2.
  */
  real_type    area() const {
    real_type a = 0.0;
    for(int i = 2; i< NumOfVertices(); ++i) {
      coord_type v1(V(0));
      coord_type vi_1(V(i-1));
      coord_type vi(V(i));
      a += ap::signed_triangle_area(v1,vi_1,vi);
    }
    return (a > 0 ? a : -a);
  }

  /*! \brief  polygon area

      Works for convex polygons in any space dimension
  */
  real_type    convex_area() const {
    real_type a = 0.0;
    for(int i = 2; i< NumOfVertices(); ++i) {
      coord_type v1(V(0));
      coord_type vi_1(V(i-1));
      coord_type vi(V(i));
      a += ap::triangle_area(v1,vi_1,vi);
    }
    return a;
  }

  /*! \brief Angle at vertex \c v
   */
  real_type solid_angle(Vtx v) const {
    typename gt::Edge e1;
    for(typename gt::EdgeOnFaceIterator e(_f); !e.IsDone(); ++e) {
      if(v == (*e).V1() || v == (*e).V2()) {
	e1 = *e;
	break;
      }
    }
    typename gt::Edge e2 = _f.TheGrid().switched_edge(v, e1, _f);
    coord_type dir_e1 = (coord( _f.TheGrid().switched_vertex(v,e1)) - coord(v));
    coord_type dir_e2 = (coord( _f.TheGrid().switched_vertex(v,e2)) - coord(v));
    return ap::angle(dir_e1, dir_e2);
  }

private:
  Face _f;
  const geom& g;
};

template<class Face, class geom>
inline
typename 
vertex_iterator_Polygon2d<Face,geom>::value_type 
vertex_iterator_Polygon2d<Face,geom>::operator*() const { return p->V(i);}

/*! \ingroup geometrictypes2d
    \relates  vertex_iterator_Polygon2d
 */
template<class Face, class geom>
inline 
bool operator==  (vertex_iterator_Polygon2d<Face,geom> const& lhs, 
		  vertex_iterator_Polygon2d<Face,geom> const& rhs)
{ 
  REQUIRE((lhs.p == rhs.p), "vertex_iterator: polygon differs!\n",1);
  return (lhs.i == rhs.i);
}

/*! \ingroup geometrictypes2d
    \relates  vertex_iterator_Polygon2d
 */
template<class Face, class geom>
inline 
bool operator!=  (vertex_iterator_Polygon2d<Face,geom> const& lhs, 
		  vertex_iterator_Polygon2d<Face,geom> const& rhs)
{ return !(lhs == rhs);}

/*! \ingroup geometrictypes2d
    \relates  vertex_iterator_Polygon2d
 */
template<class Face, class geom>
inline 
bool  operator< (vertex_iterator_Polygon2d<Face,geom> const& lhs, 
		 vertex_iterator_Polygon2d<Face,geom> const& rhs)
{ 
  REQUIRE((lhs.p == rhs.p), "vertex_iterator: polygon differs!\n",1);
  return (lhs.i < rhs.i);
}

/*
template<class geom>
class Polyhedron {};
*/

} // namespace GrAL 

#endif
