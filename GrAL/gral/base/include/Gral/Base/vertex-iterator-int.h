#ifndef GRAL_GB_VERTEX_ITERATOR_INT_H
#define GRAL_GB_VERTEX_ITERATOR_INT_H

// $LICENSE

#include "Utility/pre-post-conditions.h"
#include "Utility/ref-ptr.h"

#include "Gral/Base/common-grid-basics.h"

namespace GrAL {

template<class GT>
class vertex_iterator_int;

template<class GT>
inline bool operator==(vertex_iterator_int<GT> const& lhs, 
		       vertex_iterator_int<GT> const& rhs);

// mixin for vertex-on-vertex-iteration

// default: no VertexOnVertexIterator defined
template<class GT, unsigned HASVTXONVTX>
class vertex_iterator_int_vov_aux {};

// default: no EdgeOnVertexIterator defined
template<class GT, unsigned HASEDGEONVTX>
class vertex_iterator_int_eov_aux {};

// default: no FaceOnVertexIterator defined
template<class GT, unsigned HASFACEONVTX>
class vertex_iterator_int_fov_aux {};

// default: no FacetOnVertexIterator defined
template<class GT, unsigned HASFACETONVTX>
class vertex_iterator_int_fctov_aux {};

// default: no CellOnVertexIterator defined
template<class GT, unsigned HASCELLONVTX>
class vertex_iterator_int_cov_aux {};


// specialization: VertexOnVertexIterator defined
template<class GT>
class vertex_iterator_int_vov_aux<GT,1>
{
  typedef vertex_iterator_int<GT> vertex_iter_type;
  vertex_iter_type const& to_derived() const { return static_cast<vertex_iter_type const&>(*this);}
public:
  typedef typename GT::VertexOnVertexIterator VertexOnVertexIterator;
  VertexOnVertexIterator FirstVertex() const { return VertexOnVertexIterator(to_derived());}
  unsigned NumOfVertices() const { return to_derived().TheGrid().NumOfVertices(to_derived());}
};

// specialization: EdgeOnVertexIterator defined
template<class GT>
class vertex_iterator_int_eov_aux<GT,1>
{
  typedef vertex_iterator_int<GT> vertex_iter_type;
  vertex_iter_type const& to_derived() const { return static_cast<vertex_iter_type const&>(*this);}
public:
  typedef typename GT::EdgeOnVertexIterator EdgeOnVertexIterator;
  EdgeOnVertexIterator FirstEdge() const { return EdgeOnVertexIterator(to_derived());}
  unsigned NumOfEdges() const { return to_derived().TheGrid().NumOfEdges(to_derived());}
};

// specialization: FaceOnVertexIterator defined
template<class GT>
class vertex_iterator_int_fov_aux<GT,1>
{
  typedef vertex_iterator_int<GT> vertex_iter_type;
  vertex_iter_type const& to_derived() const { return static_cast<vertex_iter_type const&>(*this);}
public:
  typedef typename GT::FaceOnVertexIterator FaceOnVertexIterator;
  FaceOnVertexIterator FirstFace() const { return FaceOnVertexIterator(to_derived());}
  unsigned NumOfFaces() const { return to_derived().TheGrid().NumOfFaces(to_derived());}
};

// specialization: FacetOnVertexIterator defined
template<class GT>
class vertex_iterator_int_fctov_aux<GT,1>
{
  typedef vertex_iterator_int<GT> vertex_iter_type;
  vertex_iter_type const& to_derived() const { return static_cast<vertex_iter_type const&>(*this);}
public:
  typedef typename GT::FacetOnVertexIterator FacetOnVertexIterator;
  FacetOnVertexIterator FirstFacet() const { return FacetOnVertexIterator(to_derived());}
  unsigned NumOfFacets() const { return to_derived().TheGrid().NumOfFacets(to_derived());}
};


// specialization: CellOnVertexIterator defined
template<class GT>
class vertex_iterator_int_cov_aux<GT,1>
{
  typedef vertex_iterator_int<GT> vertex_iter_type;
  vertex_iter_type const& to_derived() const { return static_cast<vertex_iter_type const&>(*this);}
public:
  typedef typename GT::CellOnVertexIterator CellOnVertexIterator;
  CellOnVertexIterator FirstCell() const { return CellOnVertexIterator(to_derived());}
  unsigned NumOfCells() const { return to_derived().TheGrid().NumOfCells(to_derived());}
};



/*! Generic vertex / vertex iterator type,
    based on vertices being numbered consecutively.
   
    \ingroup iterators
    \see \ref iterators
 */

template<class GT>
class vertex_iterator_int : 
    public GT,
    public vertex_iterator_int_vov_aux  <GT, has_VertexOnVertexIterator<GT>::result>,
    public vertex_iterator_int_eov_aux  <GT,   has_EdgeOnVertexIterator<GT>::result>,
    public vertex_iterator_int_fov_aux  <GT,   has_FaceOnVertexIterator<GT>::result>,
    public vertex_iterator_int_fctov_aux<GT,  has_FacetOnVertexIterator<GT>::result>,
    public vertex_iterator_int_cov_aux  <GT,   has_CellOnVertexIterator<GT>::result>
 {
  typedef vertex_iterator_int< GT>   self;
  typedef GT                         gt;
 public:
   typedef typename gt::grid_type     grid_type;
   typedef typename gt::vertex_handle vertex_handle;
   typedef vertex_type_tag            element_type_tag;

   typedef grid_type anchor_type;
   typedef self      value_type;
protected:
  ref_ptr<grid_type const> g;
  vertex_handle            v;
public:
  vertex_iterator_int() : g(0), v(-1) {}
  explicit 
  vertex_iterator_int(grid_type const&         gg, vertex_handle vv = vertex_handle(0)) : g(gg), v(vv) {}
  explicit 
  vertex_iterator_int(ref_ptr<grid_type const> gg, vertex_handle vv = vertex_handle(0)) : g(gg), v(vv) {}

  self      & operator++() { cv(); ++v; return *this;}
  self const& operator*() const { cv(); return *this;}
  grid_type   const& TheGrid()   const { cb(); return *g;}
  anchor_type const& TheAnchor() const { cb(); return *g;}
  bool IsDone() const { cb(); return (v >= g->NumOfVertices());}

  friend bool operator== <> (vertex_iterator_int<GT> const& lhs, 
			     vertex_iterator_int<GT> const& rhs);

  vertex_handle handle() const { cv(); return vertex_handle(v);}

  bool bound() const { return g != 0;}
  bool valid() const { return bound() && v < (int) g->NumOfVertices();}
  void cb()    const { REQUIRE(bound(), "", 1);}
  void cv()    const { REQUIRE(valid(), "v=" << v, 1);}
};

  
template<class GT>
inline bool operator==(vertex_iterator_int<GT> const& lhs, 
		       vertex_iterator_int<GT> const& rhs) { return (rhs.v == lhs.v);}
template<class GT>
inline bool operator< (vertex_iterator_int<GT> const& lhs, 
		       vertex_iterator_int<GT> const& rhs) { return (rhs.v < lhs.v);}

  // #ifndef GRAL_INCLUDES_RELOPS
template<class GT>
inline bool operator!=(vertex_iterator_int<GT> const& lhs, 
		       vertex_iterator_int<GT> const& rhs) { return !(lhs == rhs);}

template<class GT>
inline bool operator> (vertex_iterator_int<GT> const& lhs, 
		       vertex_iterator_int<GT> const& rhs) { return  (rhs < lhs);}

template<class GT>
inline bool operator<=(vertex_iterator_int<GT> const& lhs, 
		       vertex_iterator_int<GT> const& rhs) { return !(lhs > rhs);}

template<class GT>
inline bool operator>=(vertex_iterator_int<GT> const& lhs, 
		       vertex_iterator_int<GT> const& rhs) { return !(lhs < rhs);}

  //#endif

} // namespace GrAL 

#endif
