#ifndef GRAL_GB_BASE_ELEMENT_GRID_H
#define GRAL_GB_BASE_ELEMENT_GRID_H

// $LICENSE_NEC

#include "Gral/Base/common-grid-basics.h"
#include "Utility/ref-ptr.h"

/*! \brief View a k-dimensional grid element as a k-dimensional grid   

    \ingroup  subranges
    \ingroup  elements
    \ingroup  gridviews

    The element grid will contain exactly one cell.

   \todo We should wrap the element/iterator types instead of simply typedefing
   to the types of the underlying base grid.

   \todo Need facet types
   \todo Need boundary
*/
template<class E> class element_grid;


namespace detail {

template<class E, int k>
class element_sides_range : public element_sides_range<E, k-1> 
{
public:
  element_sides_range() {}
  element_sides_range(E const& e) : element_sides_range<E, k-1>(e) {}
};


template<class E, int k>
struct grid_types_element_sides_range : public grid_types_element_sides_range<E, k-1>
{};

template<class E>
struct grid_types_element_sides_range<E,0> {};


template<class E>
class single_element_iterator {
  typedef single_element_iterator<E> self;
  typedef element_traits<E>          et;
  E   e;
  int ne;
public:
  typedef E value_type;
  typedef typename et::handle_type element_handle;
  typedef typename E::grid_type grid_type;

  single_element_iterator() {}
  explicit 
  single_element_iterator(E const&         ee, int nne = 0) : e(ee), ne(nne) {}
  explicit 
  single_element_iterator(ref_ptr<E const> ee, int nne = 0) : e(*ee), ne(nne) {}

  single_element_iterator(element_grid<E> const& EG, int nne = 0);

  self& operator++() { ++ne; return *this;}
  value_type const& operator*() const { return e;}
  bool IsDone() const { return ne > 0;}
  element_handle handle() const { return e.handle();}

  grid_type const& TheGrid() const { return e.TheGrid();}
  bool equal(self const& rhs) const { return ne==rhs.ne;}
};

template<class E>
inline bool operator==(single_element_iterator<E> const& lhs,
		       single_element_iterator<E> const& rhs)
{ return lhs.equal(rhs);}

#ifndef GRAL_INCLUDES_RELOPS
template<class E>
inline bool operator!=(single_element_iterator<E> const& lhs,
		       single_element_iterator<E> const& rhs)
{ return !(lhs == rhs);}
#endif

// sides of an element of dimension 0
template<class E>
class element_sides_range<E,0> {
private:
  E e;
public:
  typedef E element_type;
 
  element_sides_range() {}
  element_sides_range(element_type const& ee) : e(ee) {}

  ref_ptr<element_type const> TheElement() const { return ref_ptr<element_type const>(e);}
};


template<class E>
struct grid_types_element_sides_range<E, 1> {
  typedef element_traits<E> et;
  typedef typename et::element_type_tag etag;
  typedef typename et::grid_type grid_type;
  typedef grid_types<grid_type>  gt;

  typedef typename gt::template incidence_iterator<vertex_type_tag, etag>::type VertexIterator;
  typedef typename gt::Vertex                                                   Vertex;
  typedef typename gt::vertex_handle                                            vertex_handle; 

  typedef typename gt::VertexOnEdgeIterator                                     VertexOnEdgeIterator;

};


// sides of an element of dimension 1
template<class E>
class element_sides_range<E,1> : public grid_types_element_sides_range<E, 1> {
  typedef grid_types_element_sides_range<E, 1> gts;
private:
  E e;
public:
  typedef E element_type;
  
  element_sides_range() {}
  element_sides_range(element_type const& ee) : e(ee) {}

  ref_ptr<element_type const> TheElement() const { return ref_ptr<element_type const>(e);}
 
  typename gts::VertexIterator FirstVertex() const { return TheElement()->FirstVertex();}
  typename gts::VertexIterator EndVertex()   const { return TheElement()->EndVertex();}
  unsigned NumOfVertices() const { return TheElement()->NumOfVertices();}
};

// sides of an element of dimension 2
template<class E>
struct grid_types_element_sides_range<E, 2> : public grid_types_element_sides_range<E, 1> 
{
private:
  typedef grid_types_element_sides_range<E, 1>  base;
public: 
  typedef typename base::gt   gt;
  typedef typename base::etag etag;
  typedef typename gt::template incidence_iterator<edge_type_tag,etag>::type  EdgeIterator;
  typedef typename gt::Edge                                                   Edge;
  typedef typename gt::edge_handle                                            edge_handle;

  typedef typename gt::VertexOnFaceIterator                                   VertexOnFaceIterator;
 
};

// sides of an element of dimension 2
template<class E>
class element_sides_range<E,2> : public element_sides_range<E,1>, public grid_types_element_sides_range<E, 2> {
  typedef element_sides_range<E,1>    base;
  typedef grid_types_element_sides_range<E, 2> gts;
public:
  using base::TheElement;
  typedef typename base::element_type element_type;
  typedef typename base::etag         etag;

  element_sides_range() {}
  element_sides_range(element_type const& e) : base(e) {}

  typename gts::EdgeIterator FirstEdge()  const  { return TheElement()->FirstEdge();}
  typename gts::EdgeIterator EndEdge()    const  { return TheElement()->EndEdge();}
  unsigned     NumOfEdges() const  { return TheElement()->NumOfEdges();}
};


// sides of an element of dimension 3
template<class E>
struct grid_types_element_sides_range<E, 3> : public grid_types_element_sides_range<E, 2> 
{
private:
  typedef grid_types_element_sides_range<E, 2>  base;
  typedef typename base::gt   gt;
  typedef typename base::etag etag;
public: 
  typedef typename gt::template incidence_iterator<face_type_tag,etag>::type FaceIterator;
  typedef typename gt::Face                                                  Face;
  typedef typename gt::face_handle                                           face_handle;

 
};

// sides of an element of dimension 3
template<class E>
class element_sides_range<E,3> : public element_sides_range<E,2>, public grid_types_element_sides_range<E, 3> {
  typedef element_sides_range<E,2>             base;
  typedef grid_types_element_sides_range<E, 3> gts;
public:
  using base::TheElement;
  typedef typename base::element_type element_type;
  typedef typename base::etag         etag;
  element_sides_range() {}
  element_sides_range(element_type const& e) : base(e) {}

  typename gts::FaceIterator FirstFace()  const  { return TheElement()->FirstFace();}
  typename gts::FaceIterator EndFace()    const  { return TheElement()->EndFace();}
  unsigned     NumOfFaces() const  { return TheElement()->NumOfFaces();}
};



//---- Mixin classes for injecting the dimension-oriented name of the element E ----
// otherwise, if E = Edge or E = Face, there will be no type Edge or Face in element_grid<E>

template<class E, class ELEMGRID, int DIM>
class element_grid_mixin_aux {};

template<class E, class ELEMGRID>
class element_grid_mixin_aux<E, ELEMGRID, 0>
{
public:
  typedef E                                    Vertex;
  typedef single_element_iterator<E>           VertexIterator;
  typedef typename VertexIterator::element_handle vertex_handle;
  VertexIterator FirstVertex() const { return VertexIterator(static_cast<ELEMGRID const*>(this)->TheElement(),0);}
  VertexIterator EndVertex()   const { return VertexIterator(static_cast<ELEMGRID const*>(this)->TheElement(),1);}
  unsigned NumOfVertices() const { return 1;}
};

template<class E, class ELEMGRID>
class element_grid_mixin_aux<E, ELEMGRID, 1>
{
public:
  typedef E                                     Edge;
  typedef single_element_iterator<E>            EdgeIterator;
  typedef typename EdgeIterator::element_handle edge_handle;

  EdgeIterator FirstEdge() const { return EdgeIterator(static_cast<ELEMGRID const*>(this)->TheElement(),0);}
  EdgeIterator EndEdge()   const { return EdgeIterator(static_cast<ELEMGRID const*>(this)->TheElement(),1);}
  unsigned NumOfEdges() const { return 1;}
};

template<class E, class ELEMGRID>
class element_grid_mixin_aux<E, ELEMGRID, 2>
{
public:
  typedef E                                     Face;
  typedef single_element_iterator<E>            FaceIterator;
  typedef typename FaceIterator::element_handle face_handle;

  FaceIterator FirstFace() const { return FaceIterator(static_cast<ELEMGRID const*>(this)->TheElement(),0);}
  FaceIterator EndFace()   const { return FaceIterator(static_cast<ELEMGRID const*>(this)->TheElement(),1);}
  unsigned NumOfFaces() const { return 1;}
};


template<class E, class ELEMGRID, int DIM>
class element_grid_types_mixin_aux {};

template<class E, class ELEMGRID>
class element_grid_types_mixin_aux<E, ELEMGRID, 0>
{
public:
  typedef E                                    Vertex;
  typedef single_element_iterator<E>           VertexIterator;
  typedef typename VertexIterator::element_handle vertex_handle;
};

template<class E, class ELEMGRID>
class element_grid_types_mixin_aux<E, ELEMGRID, 1>
{
public:
  typedef E                                     Edge;
  typedef single_element_iterator<E>            EdgeIterator;
  typedef typename EdgeIterator::element_handle edge_handle;
};


template<class E, class ELEMGRID>
class element_grid_types_mixin_aux<E, ELEMGRID, 2>
{
public:
  typedef E                                     Face;
  typedef single_element_iterator<E>            FaceIterator;
  typedef typename FaceIterator::element_handle face_handle;
};


template<class E, class ELEMGRID>
class element_grid_mixin
  : public element_grid_mixin_aux<E, ELEMGRID,  element_traits<E>::dim> //element_dimension_tag::dim>
{};

template<class E, class ELEMGRID>
class element_grid_types_mixin
  : public element_grid_types_mixin_aux<E, ELEMGRID, element_traits<E>::dim> // element_dimension_tag::dim> 
{};

} // namespace detail 




template<class E> 
class element_grid : public detail::element_sides_range<E, element_traits<E>::dim>, // element_dimension_tag::dim>,
		     public detail::element_grid_mixin<E, element_grid<E> >
{
  typedef E element_type;
  typedef detail::element_sides_range<E, element_traits<E>::dim> base; // element_dimension_tag::dim> base;
public:
  typedef element_traits<E> et;
  typedef grid_types<typename et::grid_type> gt;

  using base::TheElement;
  typedef detail::single_element_iterator<E> CellIterator;
  typedef E                          Cell;
  typedef typename CellIterator::element_handle  cell_handle;

  element_grid() {}
  element_grid(element_type const& e) : base(e) {}

  operator element_type const&() const {return *TheElement();}

  CellIterator FirstCell() const { return CellIterator(TheElement(),0);}
  CellIterator EndCell()   const { return CellIterator(TheElement(),1);}
  unsigned NumOfCells() const { return 1;}

  int dimension()       const { return et::dimension(*TheElement());}

};


namespace detail {

template<class E>
inline
single_element_iterator<E>::single_element_iterator(element_grid<E> const& EG, int nne)
  : e(* EG.TheElement()), ne(nne) {}


template<class E>
struct grid_types_elem_grid 
  : public grid_types_element_sides_range<E,element_traits<E>::dim>, // element_dimension_tag::dim>,
    public element_grid_types_mixin<E, element_grid<E> >
{
  typedef element_grid<E>                  grid_type;
  typedef typename grid_type::gt           basegt;

  typedef typename grid_type::Cell         Cell;
  typedef typename grid_type::CellIterator CellIterator;
  typedef typename grid_type::cell_handle  cell_handle;

  typedef typename basegt::VertexOnCellIterator VertexOnCellIterator;
  typedef typename basegt::EdgeOnCellIterator   EdgeOnCellIterator;

  typedef grid_dim_tag<element_traits<E>::dim>  dimension_tag;
};

} // namespace detail

template<class E>
struct grid_types<element_grid<E> > : public grid_types_base<detail::grid_types_elem_grid<E> > 
{};

#endif
