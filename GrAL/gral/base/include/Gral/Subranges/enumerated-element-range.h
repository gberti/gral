#ifndef GRAL_GB_BASE_SUBRANGES_ENUMERATED_ELEMENT_RANGE_H
#define GRAL_GB_BASE_SUBRANGES_ENUMERATED_ELEMENT_RANGE_H


/* ------------------------------------------------------------

    Copyright (C) 2006 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include "Gral/Base/common-grid-basics.h"
#include "Gral/Iterators/sequence-element-iterator.h"
#include "Gral/Iterators/dummy-iterator.h"
#include "Gral/Base/map-element-iter-names.h"
#include "Gral/Subranges/enumerated-element-range-ref.h"
#include "Gral/Subranges/internal/element-range-aux.h"

#include <vector>

namespace GrAL {

  /*! \defgroup enum_elementranges  Enumerated (stored) element ranges

      \ingroup enumsubranges
      \todo The classes with explicit type names like enumerated_vertex_range are obsolete
      and should be removed.
  */


  // helper class to access ElementIterator type
  template<class E, class R>
  struct  enumerated_element_range_aux {
    typedef sequence_element_iterator<E,
				      typename E::grid_type, 
				      typename R::const_iterator> ElementIterator;
  };



/*!  \brief Subrange of a grid element range, 
  defined by listing its members.

  \ingroup enum_elementranges
  \see Module enum_elementranges
  \see enumerated_vertex_range, enumerated_edge_range, 
  enumerated_facet_range, enumerated_cell_range
*/


template<class E, class R = std::vector<typename element_traits<E>::handle_type> >
class enumerated_element_range 
  : public map_element_iter_name<typename enumerated_element_range_aux<E,R>::ElementIterator,
				 enumerated_element_range<E,R>, 
				 typename element_traits<E>::element_type_tag>
{
  typedef enumerated_element_range<E,R> self;
public:

  //------- referenced types ------------------
  typedef element_traits<E>            et;
  typedef typename et::grid_type       grid_type;
  typedef grid_types<grid_type>        gt;
  typedef typename gt::size_type       size_type;
  typedef typename et::handle_type     element_handle;
  typedef E                            Element;

  typedef typename gt::dimension_tag   dimension_tag;
  enum { dim = dimension_tag::dim };

  typedef grid_range_category_d<dim>   category;

  //---------- own types ----------------------

  typedef R                                     elt_sequence;
  typedef elt_sequence                          container_type;
  typedef typename elt_sequence::const_iterator seq_iterator;
  typedef typename elt_sequence::const_iterator const_iterator;
  typedef typename enumerated_element_range_aux<E,R>::ElementIterator ElementIterator;
    
protected:
  ref_ptr<grid_type const> the_grid;
  elt_sequence             elements;

public:
  //-------------------- construction --------------------------

  enumerated_element_range() {}
  enumerated_element_range(const grid_type& g) : the_grid(g) {}
  enumerated_element_range(ref_ptr<grid_type const> g) : the_grid(g) {}

  void set_grid(ref_ptr<grid_type const> g) {
    REQUIRE(elements.empty(), "set_grid(): range is not empty!",1);
    the_grid = g;
  }
  void set_grid(const grid_type& g) {
    set_grid(const_ref_to_ref_ptr(g));
  }

  void append(element_handle v) { elements.push_back(v);}
  void append(Element v)    { elements.push_back(v.handle());}
  template<class EIT>
  void append(EIT b, EIT e) { while(b != e) { append(*b); ++b; } }
  void push_back(element_handle v) { elements.push_back(v);}
  void push_back(Element v)    { elements.push_back(v.handle());}
  void clear() { elements.clear();}

  template<class RANGE>
  void copy(RANGE const& r)
  {
    typedef grid_types<RANGE> rgt;
    typedef typename sequence_iterator<rgt, typename et::element_type_tag>::type RangeElementIterator;
    for(RangeElementIterator e(r);!e.IsDone(); ++e)
      push_back(*e);
  }

  //-------------------- component access ----------------------

  const grid_type&    TheGrid() const {
    REQUIRE((the_grid!=0),"no grid!\n",1);
    return *the_grid;
  }

  const elt_sequence& TheContainer() const { return elements;}
  elt_sequence&       TheContainer()       { return elements;}

  //-------------------- size queries --------------------------

  size_type NumOfElements() const { return elements.size();}
 
  bool      empty() const { return elements.empty();}
  size_type size()  const { return elements.size();}

  //-------------------- iteration  ----------------------------

  ElementIterator FirstElement() const 
  { return ElementIterator(TheGrid(), begin(), end());}
  ElementIterator EndElement() const 
  { return ElementIterator(TheGrid(), end(),   end());}

  const_iterator begin() const { return elements.begin();}
  const_iterator end  () const { return elements.end  ();}
};


//----------------------------------------------------------------
//     vertex_range / facet_range / cell_range (value semantics)
//----------------------------------------------------------------

/*! \brief Vertex subrange of a grid, defined by listing the member vertices
  \ingroup enum_elementranges
*/        
template<class Grid, class R = std::vector<typename grid_types<Grid>::vertex_handle> >
class enumerated_vertex_range 
  : public enumerated_element_range<typename grid_types<Grid>::Vertex,R>
{
  typedef enumerated_element_range<typename grid_types<Grid>::Vertex> base;
  using base::elements;
  using base::the_grid;
public:
  typedef typename base::Element         Vertex;
  typedef typename base::ElementIterator VertexIterator;
  typedef typename base::element_handle  vertex_handle;
  typedef typename base::grid_type       grid_type;
  typedef typename base::elt_sequence    elt_sequence;
  using base::size;
  using base::FirstElement;
  using base::EndElement;
  typedef typename base::size_type size_type;

  typedef vertex_range_ref<grid_type, elt_sequence>  range_type_ref;

  enumerated_vertex_range() {}
  enumerated_vertex_range(const grid_type& g) : base(g) {}
  enumerated_vertex_range(const base& b) : base(b) {}

  template<class RANGE>
  explicit enumerated_vertex_range(RANGE const& r) : base(r.TheGrid())  { copy(r); }

  
  size_type      NumOfVertices()     const { return size();}
  VertexIterator FirstVertex() const { return FirstElement();}
  VertexIterator EndVertex()   const { return EndElement();}

  range_type_ref range() const 
  { return range_type_ref(0,size(),elements,*the_grid);}
};


/*! \brief Edge subrange of a grid, defined by listing the member edges
  \ingroup enum_elementranges
*/        
template<class Grid, class R = std::vector<typename grid_types<Grid>::edge_handle> >
class enumerated_edge_range 
  : public enumerated_element_range<typename grid_types<Grid>::Edge,R>
{
  typedef enumerated_element_range<typename grid_types<Grid>::Edge> base;
  using base::elements;
  using base::the_grid;
public:
  typedef typename base::Element         Edge;
  typedef typename base::ElementIterator EdgeIterator;
  typedef typename base::element_handle  edge_handle;
  typedef typename base::grid_type       grid_type;
  typedef typename base::elt_sequence    elt_sequence;
  using base::size;
  using base::FirstElement;
  using base::EndElement;
  typedef typename base::size_type size_type;
  
  typedef edge_range_ref<grid_type, elt_sequence>  range_type_ref;

  enumerated_edge_range() {}
  enumerated_edge_range(const grid_type& g) : base(g) {}
  enumerated_edge_range(const base& b) : base(b) {}
  template<class RANGE>
  explicit enumerated_edge_range(RANGE const& r) : base(r.TheGrid())  { copy(r); }
  
  size_type    NumOfEdges()     const { return size();}
  EdgeIterator FirstEdge() const { return FirstElement();}
  EdgeIterator EndEdge()   const { return EndElement();}

  range_type_ref range() const 
  { return range_type_ref(0,size(),elements,*the_grid);}

};


/*! \brief Facet subrange of a grid, defined by listing the member facets
  \ingroup enum_elementranges
*/        
template<class Grid, class R = std::vector<typename grid_types<Grid>::facet_handle> > 
class enumerated_facet_range 
  : public enumerated_element_range<typename grid_types<Grid>::Facet,R>
{
  typedef enumerated_element_range<typename grid_types<Grid>::Facet> base;
  using base::elements;
  using base::the_grid;
public:
  typedef typename base::Element         Facet;
  typedef typename base::ElementIterator FacetIterator;
  typedef typename base::element_handle  facet_handle;
  typedef typename base::grid_type       grid_type;
  typedef typename base::elt_sequence    elt_sequence;
  using base::size;
  using base::FirstElement;
  using base::EndElement;
  typedef typename base::size_type size_type;
  
  typedef facet_range_ref<grid_type, elt_sequence>  range_type_ref;

  enumerated_facet_range() {}
  enumerated_facet_range(const grid_type& g) : base(g) {}
  enumerated_facet_range(const base& b) : base(b) {}
  template<class RANGE>
  explicit enumerated_facet_range(RANGE const& r) : base(r.TheGrid())  { copy(r); }

  size_type     NumOfFacets()     const { return size();}
  FacetIterator FirstFacet() const { return FirstElement();}
  FacetIterator EndFacet()   const { return EndElement();}

  range_type_ref range() const 
  { return range_type_ref(0,size(),elements,*the_grid);}

};


/*! \brief cell subrange of a grid, defined by listing the member cells
  \ingroup enum_elementranges

*/        
template<class Grid, class R = std::vector<typename grid_types<Grid>::cell_handle> >
class enumerated_cell_range 
  : public enumerated_element_range<typename grid_types<Grid>::Cell, R>
{
  typedef enumerated_element_range<typename grid_types<Grid>::Cell> base;
  using base::elements;
  using base::the_grid;
public:
  typedef typename base::Element         Cell;
  typedef typename base::ElementIterator CellIterator;
  typedef typename base::element_handle  cell_handle;
  typedef typename base::grid_type       grid_type;
  typedef typename base::elt_sequence    elt_sequence;
  using base::size;
  using base::FirstElement;
  using base::EndElement;
  typedef typename base::size_type size_type;

  typedef cell_range_ref<grid_type, elt_sequence>  range_type_ref;

  enumerated_cell_range() {}
  enumerated_cell_range(const grid_type& g) : base(g) {}
  enumerated_cell_range(const base& b) : base(b) {}
  
  template<class RANGE>
  explicit enumerated_cell_range(RANGE const& r) : base(r.TheGrid())  { copy(r); }
  
  size_type    NumOfCells()     const { return size();}
  CellIterator FirstCell()  const { return FirstElement();}
  CellIterator EndCell()    const { return EndElement();}

  range_type_ref range() const 
  { return range_type_ref(0,size(),elements,*the_grid);}
};


  namespace detail {

    template<class RANGE>
    struct grid_types_range_base : 
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
  struct grid_types<enumerated_element_range<E,R> >
    : public grid_types_base<detail::grid_types_range_base<enumerated_element_range<E,R> > > 
  {};
  
  template<class Grid, class R>
  struct grid_types<enumerated_vertex_range<Grid, R> > 
    : public grid_types_base<detail::grid_types_range_base<enumerated_vertex_range<Grid,R> > > 
  {};
  
  
  template<class Grid, class R>
  struct grid_types<enumerated_edge_range<Grid,R> > 
    : public grid_types_base<detail::grid_types_range_base<enumerated_edge_range<Grid,R> > > 
  {};
  
  
  template<class Grid, class R>
  struct grid_types<enumerated_facet_range<Grid,R> > 
    : public grid_types_base<detail::grid_types_range_base<enumerated_facet_range<Grid,R> > > 
  {};
  
  
  template<class Grid, class R>
  struct grid_types<enumerated_cell_range<Grid,R> > 
    : public grid_types_base<detail::grid_types_range_base<enumerated_cell_range<Grid,R> > > 
  {};



  // overloads for gral_begin etc.

  template<class E, class R>
  typename grid_types<enumerated_element_range<E,R> >::ElementIterator
  gral_begin(enumerated_element_range<E,R> const& a,
	     typename grid_types<enumerated_element_range<E,R> >::ElementIterator)
  { return a.FirstElement();}

  template<class E, class R>
  typename grid_types<enumerated_element_range<E,R> >::ElementIterator
  gral_end  (enumerated_element_range<E,R> const& a,
	     typename grid_types<enumerated_element_range<E,R> >::ElementIterator)
  { return a.EndElement();}

  template<class E, class R>
  typename grid_types<enumerated_element_range<E,R> >::size_type
  gral_size (enumerated_element_range<E,R> const& a,
	     typename grid_types<enumerated_element_range<E,R> >::ElementIterator)
  { return a.NumOfElements();}



} // namespace GrAL

#endif
