#ifndef GRAL_GB_GEOMETRY_INTERSECTIONS_H
#define GRAL_GB_GEOMETRY_INTERSECTIONS_H

// $LICENSE_NEC

#include "Geometry/interval.h"
#include "Geometry/point-traits.h"
#include "Geometry/algebraic-primitives.h"
#include "Geometry/geometric-primitives.h"

#include <vector>

/*! \defgroup geomintersections Geometric intersections
    \brief Components for calculating intersections for geometric objects
    
*/

/*! Function class projecting a convex body to a line trough the origin
  
   \ingroup  geomintersections 
*/
template<class P>
class line_projector {
  P dir_;
  
  typedef P                           coord_type;
  typedef point_traits<coord_type>    pt;
  typedef typename pt::component_type scalar_type;
  typedef algebraic_primitives<P>     ap;
public:
  line_projector(P const& ddir) : dir_(ddir) {}

  coord_type const& dir() { return dir_;}
  /*! \brief Project the convex object \c obj to this line

      \templateparam
       - \c OBJ
          - models: \c segment<P>, \c triangle<P>
          - type \c OBJ::geom_vertex_iterator
          - type \c OBJ::coord_type
          - \c OBJ::begin_vertex(),  \c OBJ::end_vertex()
      
      \pre \c obj represents a convex, compact body (convex hull of its vertices)

   */
  template<class OBJ>
  interval<scalar_type> operator()(OBJ const& obj) const {
    interval<scalar_type> I;
    coord_type n(dir_); // in case the two types are different.
    //   for(typename OBJ:: geom_vertex_iterator v = obj.begin_vertex(); v != obj.end_vertex(); ++v)
    //  I |= ap::dot(*v,n);
    for(typename OBJ::VertexIterator v = obj.FirstVertex(); v != obj.EndVertex(); ++v)
      I |= ap::dot(obj(*v),n);
    return I;
  }
};

/*! \brief Base class for holding directions of a convex polytope of arbitrary dimension

   \ingroup  geomintersections 

   \note This is a general implementation; it is not the most efficient implementation.
    I uses vectors of vectors to hold directions.
    Specializations of the \c polytope_directions<> template can do better
*/
template<class OBJ>
class polytope_directions_base {
protected:
  typedef typename OBJ::coord_type coord_type;
  typedef std::vector<coord_type>   dir_sequence;
  typedef std::vector<dir_sequence> dimension_dirs;
  std::vector<dimension_dirs> dirs;

public:
  typedef typename dimension_dirs::const_iterator dir_sequence_iterator;
  typedef typename dir_sequence  ::const_iterator dir_iterator;

  dir_sequence_iterator begin(int k) const { return dirs[k].begin();}  
  dir_sequence_iterator end  (int k) const { return dirs[k].end  ();}  

  polytope_directions_base(OBJ const& obj) 
  {
    dirs.resize(obj.space_dimension()+1);
    // k = 0: empty dir_sequence
    dirs[0].push_back(dir_sequence());
    // to be done by derived:
    // k = 1: edge dirs
    // k = 2: face dirs if dim(obj) >= 2 && spacedim >= 3
  }
};

/*! \brief Class holding the directions of a convex polytope

   \ingroup  geomintersections 

    Primary template, to be specialized.
    The specialization is essentially driven by space dimension and polytope dimension.
 */
template<class OBJ>
class polytope_directions {};


/*! \brief Specialization for \c space_point<P>

   \ingroup  geomintersections 

*/
template<class P>
class polytope_directions<space_point<P> > : public polytope_directions_base<space_point<P> >
{
  typedef space_point<P>                        object_type;
  typedef polytope_directions_base<object_type> base;
public:
  polytope_directions(object_type const& obj) : base(obj) {}
};


/*! \brief Specialization for \c segment<P>

   \ingroup  geomintersections 

*/
template<class P>
class polytope_directions<segment<P> > : public polytope_directions_base<segment<P> >
{
  typedef  polytope_directions_base<segment<P> >  base;
  typedef  segment<P>                             object_type;
  typedef  algebraic_primitives<P>                ap;
  typedef  P                                      coord_type;
public:
  typedef typename base::dir_sequence dir_sequence;

  polytope_directions(object_type const& obj) : base(obj) 
  {
    if(obj.space_dimension() == 2) {
      // make the segment an infinitesimal rectangle
      dirs[1].push_back(dir_sequence(1, obj(1)-obj(0)));
      dirs[1].push_back(dir_sequence(1, ap::any_perp(obj(1)-obj(0))));
    }
    else if(obj.space_dimension() == 3) {
      // make segment an infinitesimal brick
      // edge directions
      coord_type edges[3];
      edges[0] =  obj(1)-obj(0);
      // get first orthogonal direction
      edges[1] = ap::any_perp(edges[0]);
      edges[2] = ap::perp(edges,edges+2);
      dirs[1].push_back(dir_sequence(1,edges[0]));
      dirs[1].push_back(dir_sequence(1,edges[1]));
      dirs[1].push_back(dir_sequence(1,edges[2]));
     
      for(int i = 0; i < 3; ++i) {
	int j = (i+1)%3;
	dir_sequence f(2);
	f[0] = dirs[1][i][0];
	f[1] = dirs[1][j][0];
	dirs[2].push_back(f);
      }
    }
    else if(obj.space_dimension() > 3) {
      REQUIRE_ALWAYS(false, "not implemented for space dimensions >= 3!",1);
    }
  }
}; 


/*! \brief Specialization for \c triangle<P>

   \ingroup  geomintersections 

*/
template<class P>
class polytope_directions<triangle<P> > : public polytope_directions_base<triangle<P> >
{
  typedef  polytope_directions_base<triangle<P> > base;
  typedef  triangle<P>                            object_type;
  typedef  algebraic_primitives<P>                ap;
  typedef  P                                      coord_type;
public:
  typedef typename base::dir_sequence dir_sequence;
  polytope_directions(object_type const& obj) : base(obj) 
  {
    // existing edges
    dirs[1].push_back(dir_sequence(1,obj(1)-obj(0)));
    dirs[1].push_back(dir_sequence(1,obj(2)-obj(1)));
    dirs[1].push_back(dir_sequence(1,obj(0)-obj(2)));
    if(obj.space_dimension() == 3) {
      // make it a 3D prism
      // additional edge: normal
      coord_type edges[2] = { obj(1)-obj(0),obj(2)-obj(0)};
      dirs[1].push_back(dir_sequence(1, ap::perp(edges, edges+2)));

      // faces
      // existing face
      dir_sequence f1(2);
      f1[0] = obj(1)-obj(0);
      f1[1] = obj(2)-obj(0);
      dirs[2].push_back(f1);
      // additional faces
      for(int i = 0; i < 3; ++i) {
	dir_sequence f(2);
	f[0]  = dirs[1][i][0];
	f[1] = dirs[1][3][0];
	dirs[2].push_back(f);
      }
    }
    else if(obj.space_dimension() > 3) {
      REQUIRE_ALWAYS(false, "not implemented for space dimensions >= 3!",1);
    }
  }
}; 

/*! \brief Specialization for \c tetrahedron<P>

   \ingroup  geomintersections 

*/

template<class P>
class polytope_directions<tetrahedron<P> > : public polytope_directions_base<tetrahedron<P> >
{
  typedef  polytope_directions_base<tetrahedron<P> > base;
  typedef  tetrahedron<P>                            object_type;
  typedef  algebraic_primitives<P>                   ap;
  typedef  P                                         coord_type;
public:
  typedef typename base::dir_sequence dir_sequence;
  polytope_directions(object_type const& obj) : base(obj) 
  {
    // edges -> dirs[1]
    for(int i = 0; i < 4; ++i)
      for(int j = i+1; j < 4; ++j)
	dirs[1].push_back(dir_sequence(1,obj(j)-obj(i)));
    // faces -> dirs[2]
    for(int i = 0; i < 4; ++i)
      for(int j = i+1; j < 4; ++j)
	for(int k = j+1; k < 4; ++k) {
	  dir_sequence face(2);
	  face[0] = obj(j)-obj(i);
	  face[1] = obj(k)-obj(i);
	  dirs[2].push_back(face);
	}
  }
};


/*! \brief Testing intersections of convex polytopes of arbitrary dimension

    This class uses the method of separating axes to implement an intersection test for
    compact convex polytopes or arbitrary dimension. 
    
    See <a href="http://www.magic-software.com/Documentation/MethodOfSeparatingAxes.pdf" TARGET=_parent>
     The Method of Separating Axes </A> by D. Eberly.

   \note If the objects are large (many vertices), this is probably not the most efficient method. 

*/
template<class OBJ1, class OBJ2>
class intersection_by_separating_axes {
public:  
  /*! \brief True if and only if \c o1 and \c o2 intersect.
       
      \pre <tt> o1.space_dimension() == o2.space_dimension() </tt>
      \post Returns \c true if and only if \c o1 and \c o2 intersect.
        They are reported to intersect also if only their boundary intersects
        (this is of interest if exact arithmetic is used by both \c OBJi::coord_type).
  */
  static bool intersects(OBJ1 const& o1,
			 OBJ2 const& o2)
  {
    REQUIRE(o1.space_dimension() == o2.space_dimension(), "",1);
    typedef typename OBJ1::coord_type coord_type;
    typedef algebraic_primitives<coord_type> ap;
    typedef point_traits<coord_type>         pt;
    int sdim = o1.space_dimension();

    coord_type e1 = pt::Origin(sdim);
    pt::x(e1) = 1; //ap::unit_vector(1);
    line_projector<coord_type> p(e1);
    if(disjoint(p(o1),p(o2)))
      return false;

    polytope_directions<OBJ1> dirs1(o1);
    polytope_directions<OBJ2> dirs2(o2);
    typedef typename polytope_directions<OBJ1>::dir_sequence_iterator dir_seq_it_1;
    typedef typename polytope_directions<OBJ2>::dir_sequence_iterator dir_seq_it_2;
    for(int k = 0; k <= sdim -1; ++k) {
      int d_1_k = sdim-1-k;
      // k directions from obj 1, and d-1-k directions from obj2
      for(dir_seq_it_1 d1 = dirs1.begin(k); d1 != dirs1.end(k); ++d1)
	for(dir_seq_it_2 d2 = dirs2.begin(d_1_k); d2 != dirs2.end(d_1_k); ++d2) {
	  std::vector<coord_type> dirs(sdim-1);
	  std::copy(d1->begin(), d1->end(), dirs.begin());
	  std::copy(d2->begin(), d2->end(), dirs.begin()+(d1->end()-d1->begin()));
	  coord_type normal = ap::perp(dirs.begin(), dirs.end()); 
	  line_projector<coord_type> p(normal);
	  if(disjoint(p(o1),p(o2)))
	    return false;
	}
    }
    return true; 
  }
};

template<class OBJ1, class OBJ2>
inline bool intersects(OBJ1 const& o1, OBJ2 const& o2)
{
  return intersection_by_separating_axes<OBJ1,OBJ2>::intersects(o1,o2);
} 

#endif
