#ifndef GRAL_BASE_GB_GEOMETRIC_TEMPLATE_TYPES_FOR_GRIDS
#define GRAL_BASE_GB_GEOMETRIC_TEMPLATE_TYPES_FOR_GRIDS



// $LICENSE

#include "Config/compiler-config.h"
#include "Geometry/algebraic-primitives.h"

//----------------------------------------------------------------
//
/*! \defgroup geometrictypes Geometric types
    \ingroup gridgeometries

    Generic geometric types for combinatorial grid elements.

   \b Contents:
    - template<class Edge, class geom> class Segment;
    - template<class Edge, class geom> class Polygon2d;

   These geometric types assume a linear geometry, segments
   are straight lines.

   The classes can be specialized to concrete grid elements, 
   in order to remove layers of indirection or use  additional
   knowledge. <BR>
   They MUST be specialized if the underlying assumptions (linearity)
   are violated.

   \see Module \ref geometries
   \see Module \ref geometricfunctors
*/
//----------------------------------------------------------------


/*! \brief  Geometric segment corresponding to combinatorial edge
   \ingroup geometrictypes
    
   \templateparams
    - Edge: $GrAL GridEdge
    - geom: $GrAL VertexGridGeometry

   \see Module \ref geometrictypes
*/
template<class Edge, class geom>
class Segment {
  typedef typename geom::coord_type coord_type;
  typedef algebraic_primitives<coord_type> algebra;
public:
  Segment(const Edge& e, const geom& gg) : _e(e), g(gg) {}
  coord_type Start()  const { return g.coord(_e.V1());}
  coord_type End()    const { return g.coord(_e.V2());}
  coord_type center() const { return (0.5*(Start()+End()));}
  double     length() const { return( algebra::norm_2(Start()-End()));} 

  coord_type operator()(double t) const { return ((1-t)*Start() + t*End());}
private:
  Edge _e;
  const geom& g;
};



template<class Face, class geom> class Polygon2d;

/*! \brief Iterator over vertices of Polygon2d
   \ingroup geometrictypes

   \templateparams
   (same as for Polygon2d<Face,geom>)
   - Face $GrAL GridFace
   - geom $GrAL VertexGridGeometry

   \see Polygon2d
   \see Module \ref geometrictypes
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
   \ingroup geometrictypes

   \templateparams
   (same as for vertex_iterator_Polygon2d<Face,geom>)
   - Face $GrAL GridFace
   - geom $GrAL VertexGridGeometry
   \see vertex_iterator_Polygon2d<Face,geom>
   \see Module \ref geometrictypes
 */
template<class Face, class geom>
class Polygon2d {
public:
  typedef typename geom::coord_type        coord_type;
  typedef algebraic_primitives<coord_type> algebra;

  typedef point_traits<coord_type> pt;

  typedef typename geom::grid_type grid_type;
  typedef grid_types<grid_type> gt;
  typedef typename gt::Vertex      Vtx;
public:

  //--------------------- construction -------------------------------

  Polygon2d(const Face& f, const geom& gg) : _f(f), g(gg) {}
  const geom& TheGeometry() const { return g;}


  //------------------- iteration ----------- ------------------------

  typedef vertex_iterator_Polygon2d<Face,geom>  vertex_iterator;
  vertex_iterator FirstVertex() const { return vertex_iterator(1,*this);}
  vertex_iterator EndVertex()   const { return vertex_iterator(NumOfVertices()+1,*this);}

  int               NumOfVertices() const {return _f.NumOfVertices();}
  coord_type /*const&*/ V(int i) const {
    Vtx v = _f.V(i);
    coord_type vv = TheGeometry().coord(v);
    return vv;
  }
  coord_type /*const&*/ Vertex(int i) const {return V(i);} 


  //--------------------- geometric functions ------------------------

  //! mass center of the vertices
  coord_type barycenter() const 
    { 
      coord_type c(pt::Origin(pt::Dim(V(1)))); //TheGeometry().space_dimension()));
      for(int i =1; i<= NumOfVertices(); i++) 
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

  //! Area calcultation works for general simple polygons
  double    area() const {
    double a = 0.0;
    for(int i = 3; i<= NumOfVertices(); ++i) {
      coord_type v1(V(1));
      coord_type vi_1(V(i-1));
      coord_type vi(V(i));
      a += algebra::signed_triangle_area(v1,vi_1,vi);
    }
    return (a > 0 ? a : -a);
  }

private:
  Face _f;
  const geom& g;
};

template<class Face, class geom>
inline
vertex_iterator_Polygon2d<Face,geom>::value_type 
vertex_iterator_Polygon2d<Face,geom>::operator*() const { return p->V(i);}

/*! \ingroup geometrictypes
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

template<class Face, class geom>
inline 
bool operator!=  (vertex_iterator_Polygon2d<Face,geom> const& lhs, 
		  vertex_iterator_Polygon2d<Face,geom> const& rhs)
{ return !(lhs == rhs);}

/*! \ingroup geometrictypes
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

/*
template<class gt, class geom> 
struct geometric_types {
  
  typedef Segment<typename gt::Edge,geom> segment;
  typedef Polygon<typename gt::Face,geom> polygon;
};
*/

#endif
