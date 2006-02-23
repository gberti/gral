#ifndef GRAL_GB_BASE_SUBRANGES_ELEMENT_RANGE_REF_H
#define GRAL_GB_BASE_SUBRANGES_ELEMENT_RANGE_REF_H

// $LICENSE_NEC_2006

#include "Gral/Base/common-grid-basics.h"
#include "Gral/Base/map-element-iter-names.h"

#include "Gral/Subranges/internal/element-range-aux.h"
#include "Gral/Iterators/sequence-element-iterator.h"

namespace GrAL {

/*! \defgroup enumsubranges Enumerated Grid Subranges
    \ingroup subranges 

    \brief Subranges defined by the sequence of their members 
*/

/*! \defgroup enum_elementranges_ref  Light-weight reference-based element ranges

    \ingroup enumsubranges

    \todo The classes with explicit type names like vertex_range_ref are obsolete
    and should be removed.
 */



// helper class to access ElementIterator type
template<class E, class R>
class element_range_ref_aux {
public:
  typedef R                         range_type;
  typedef sequence_element_iterator<E,
				    typename E::grid_type, 
				    typename R::const_iterator> ElementIterator;

};


/*! \brief reference to a (possibly subset of) element range
   \ingroup enum_elementranges_ref

   The dimension of the element remains unspecified.
   See vertex_range_ref, edge_range_ref, facet_range_ref, cell_range_ref
   for specializations.

 */

template<class E, class R>
class element_range_ref 
  : public map_element_iter_name<typename  element_range_ref_aux<E,R>::ElementIterator,
                                 element_range_ref<E,R>, 
                                 typename element_traits<E>::element_type_tag>
{
  typedef element_range_ref<E,R>       self;
public:
  typedef element_traits<E>            et;
  typedef typename et::grid_type       grid_type;
  typedef grid_types<grid_type>        gt;
  typedef typename gt::size_type       size_type;
  typedef typename et::handle_type     elt_handle;
  typedef E                            Element;
  typedef typename et::handle_type     element_handle;

  typedef R                                   range_type;
  typedef typename range_type::const_iterator const_iterator;

  typedef range_type sequence_type;
  typedef typename  element_range_ref_aux<E,R>::ElementIterator  ElementIterator;

  typedef typename gt::dimension_tag                dimension_tag;
  typedef grid_range_category_d<dimension_tag::dim> category;
private:
  //---- DATA -----
  size_type ebeg, eend;
  ref_ptr<const range_type> the_range;
  ref_ptr<const grid_type>  the_grid;
public:
  //---------------------- construction -------------------------

  //element_range_ref(HIt b, HIt e, const grid_type& g) : ebeg(b), eend(e), the_grid(&g) {}
  element_range_ref() : ebeg(0), eend(0) {}

  // should this be the empty range or the whole range ??
  element_range_ref(const grid_type& g) 
    : ebeg(0), eend(0), the_grid(g) {}
  element_range_ref(ref_ptr<const grid_type> g) 
    : ebeg(0), eend(0), the_grid(g) {}

  element_range_ref(const range_type& r, const grid_type& g) 
    : ebeg(0), eend(0), the_range(r), the_grid(g) {}
  element_range_ref(ref_ptr<const range_type> r, ref_ptr<const grid_type> g) 
    : ebeg(0), eend(0), the_range(r), the_grid(g) {}

  element_range_ref(int b, int e, const range_type& r,  const grid_type& g) 
    : ebeg(b), eend(e), the_range(r), the_grid(g) {}
  element_range_ref(int b, int e, ref_ptr<const range_type> r, ref_ptr<const grid_type> g) 
    : ebeg(b), eend(e), the_range(r), the_grid(g) {}


  // element_range_ref(const enumerated_element_range<E,R>& er);

  //---------------------- component access ---------------------

  grid_type const& TheGrid() const { return (*the_grid);}

  //---------------------- size queries -------------------------

  size_type NumOfElements() const { return size();}

  // STL-like
  size_type size()  const { return (eend - ebeg);} // random access!
  bool      empty() const { return (eend == ebeg);}

  //---------------------- iteration ----------------------------

  ElementIterator FirstElement() const 
  { return ElementIterator(TheGrid(), begin(), end()); }
   ElementIterator EndElement() const 
  { return ElementIterator(TheGrid(), end(), end()); }

 
  //! allow for ElementIterator(Range) constructor.
  operator ElementIterator() const { return FirstElement();}

  // STL-like iteration over handles
  // WARNING: THESE ITERATORS GET INVALID IF THE RANGE IS EXTENDED !!
  const_iterator begin() const { return (the_range->begin() + ebeg);}
  const_iterator end()   const { return (the_range->begin() + eend);}

};


/*! \brief Reference to a (possibly subset of) vertex range.
    \ingroup enum_elementranges_ref
 */

template<class Grid, class R>
class vertex_range_ref
  : public element_range_ref<typename grid_types<Grid>::Vertex, R>
{
  typedef element_range_ref<typename grid_types<Grid>::Vertex, R> base;
public:
  typedef typename base::Element         Vertex;
  typedef typename base::ElementIterator VertexIterator;   
  typedef typename base::grid_type       grid_type;
  typedef typename base::range_type      range_type;
  using base::size;
  using base::FirstElement;
  using base::EndElement;
  typedef typename base::size_type size_type;

  vertex_range_ref() {}
  vertex_range_ref(const grid_type& g) : base(g) {}
  vertex_range_ref(const range_type& r, const grid_type& g) : base(r,g) {}
  vertex_range_ref(int b, int e, const range_type& r, const grid_type& g) 
    : base(b,e,r,g) {}
  vertex_range_ref(const base& b) : base(b) {}

  
  size_type      NumOfVertices() const { return size();}
  VertexIterator FirstVertex()   const { return FirstElement();}
  VertexIterator EndVertex  ()   const { return EndElement();}

};





/*! \brief Reference to a (possibly subset of) edge range.
    \ingroup enum_elementranges_ref
 */

template<class Grid, class R>
class edge_range_ref
  : public element_range_ref<typename grid_types<Grid>::Edge, R>
{
  typedef element_range_ref<typename grid_types<Grid>::Edge, R> base;
public:
  typedef typename base::Element         Edge;
  typedef typename base::ElementIterator EdgeIterator;
  typedef typename base::grid_type       grid_type;
  typedef typename base::range_type      range_type;
  using base::size;
  using base::FirstElement;
  using base::EndElement;
  typedef typename base::size_type size_type;

  edge_range_ref() {}
  edge_range_ref(const grid_type& g) : base(g) {}
  edge_range_ref(const range_type& r, const grid_type& g) : base(r,g) {}
  edge_range_ref(int b, int e, const range_type& r, const grid_type& g) 
    : base(b,e,r,g) {}
  edge_range_ref(const base& b) : base(b) {}
  
  size_type     NumOfEdges() const { return size();}
  EdgeIterator  FirstEdge()  const { return FirstElement();}
  EdgeIterator  EndEdge()    const { return EndElement();}

};



/*! \brief Reference to a (possibly subset of) facet range.
    \ingroup enum_elementranges_ref
 */

template<class Grid, class R>
class facet_range_ref
  : public element_range_ref<typename grid_types<Grid>::Facet, R>
{
  typedef element_range_ref<typename grid_types<Grid>::Facet, R> base;
public:
  typedef typename base::Element         Facet;
  typedef typename base::ElementIterator FacetIterator;
  typedef typename base::grid_type       grid_type;
  typedef typename base::range_type      range_type;
  using base::size;
  using base::FirstElement;
  using base::EndElement;
  typedef typename base::size_type size_type;

  facet_range_ref() {}
  facet_range_ref(const grid_type& g) : base(g) {}
  facet_range_ref(const range_type& r, const grid_type& g) : base(r,g) {}
  facet_range_ref(int b, int e, const range_type& r, const grid_type& g) 
    : base(b,e,r,g) {}
  facet_range_ref(const base& b) : base(b) {}
  
  size_type      NumOfFacets() const { return size();}
  FacetIterator  FirstFacet()  const { return FirstElement();}
  FacetIterator  EndFacet()    const { return EndElement();}

};



/*! \brief Reference to a subset of a cell range.
    \ingroup enum_elementranges_ref
 */

template<class Grid, class R>
class cell_range_ref
  : public element_range_ref<typename grid_types<Grid>::Cell, R>
{
  typedef element_range_ref<typename grid_types<Grid>::Cell, R> base;
public:
  typedef typename base::element_handle  cell_handle;
  typedef typename base::Element         Cell;
  typedef typename base::ElementIterator CellIterator;
  typedef typename base::grid_type       grid_type;
  typedef typename base::range_type      range_type;
  using base::size;
  using base::FirstElement;
  using base::EndElement;
  typedef typename base::size_type size_type;


  cell_range_ref() {}
  cell_range_ref(const grid_type& g) : base(g) {}
  cell_range_ref(const range_type& r, const grid_type& g) : base(r,g) {}
  cell_range_ref(int b, int e, const range_type& r, const grid_type& g) 
    : base(b,e,r,g) {}
  cell_range_ref(const base& b) : base(b) {}

  
  size_type      NumOfCells() const { return size();}
  CellIterator   FirstCell()  const { return FirstElement();}
  CellIterator   EndCell()    const { return EndElement();}

};


  namespace detail {
    
    template<class RANGE>
    struct grid_types_range_ref_base : 
      public grid_types_detail::grid_types_root,
      public map_elem_names<RANGE, typename RANGE::Element::element_type_tag>
    {
      typedef RANGE                              range_type;
      typedef typename range_type::grid_type     grid_type;
      typedef typename range_type::size_type     size_type;
      typedef typename range_type::dimension_tag dimension_tag;
    };
    
  } // namespace detail

  template<class E, class R>
  struct grid_types<element_range_ref<E,R> >
    : public grid_types_base<detail::grid_types_range_ref_base<element_range_ref<E,R> > >
  {};

  template<class Grid, class R>
  struct grid_types<vertex_range_ref<Grid,R> > 
    : public  grid_types_base<detail::grid_types_range_ref_base<vertex_range_ref<Grid,R> > >
  {};


  template<class Grid, class R>
  struct grid_types<edge_range_ref<Grid,R> > 
    : public  grid_types_base<detail::grid_types_range_ref_base<edge_range_ref<Grid,R> > >
  {};
  
  template<class Grid, class R>
  struct grid_types<facet_range_ref<Grid,R> > 
    : public  grid_types_base<detail::grid_types_range_ref_base<facet_range_ref<Grid,R> > >
  {};


  template<class Grid, class R>
  struct grid_types<cell_range_ref<Grid,R> > 
    : public  grid_types_base<detail::grid_types_range_ref_base<cell_range_ref<Grid,R> > >
  {};


  /* these overloads for gral_begin etc. are also sufficient for element_specific ranges 
     like vertex_range_ref
  */
  template<class E, class R>
  typename element_range_ref<E,R>::ElementIterator
  gral_begin(element_range_ref<E,R> const& a,  typename element_range_ref<E,R>::ElementIterator)
  { return a.FirstElement(); }

  template<class E, class R>
  typename element_range_ref<E,R>::ElementIterator
  gral_end  (element_range_ref<E,R> const& a,  typename element_range_ref<E,R>::ElementIterator)
  { return a.EndElement(); }

  template<class E, class R>
  typename element_range_ref<E,R>::size_type
  gral_size (element_range_ref<E,R> const& a,  typename element_range_ref<E,R>::ElementIterator)
  { return a.NumOfElements(); }



} // namespace GrAL

#endif
