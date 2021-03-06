#ifndef GRAL_GB_BASE_SUBRANGES_ENUMERATED_SUBRANGE_REF_H
#define GRAL_GB_BASE_SUBRANGES_ENUMERATED_SUBRANGE_REF_H


/* ------------------------------------------------------------

    Copyright (C) 2006 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include "Utility/pre-post-conditions.h"
#include "Utility/ref-ptr.h"

#include "Gral/Base/common-grid-basics.h"
#include "Gral/Base/map-element-iter-names.h"
#include "Gral/Base/archetypes-from-base.h"
#include "Gral/Base/wrap-elements.h"

#include "Gral/Subranges/enumerated-element-range.h"
#include "Gral/Subranges/enumerated-subrange.h"

#include "Gral/Base/grid-function-hash.h"

#include "Gral/Iterators/sequence-element-iterator.h"
#include "Gral/Iterators/closure-iterator.h"

namespace GrAL {


/*! \brief  A grid subrange  view that is a simple enumeration of its elements.
   \ingroup enumsubranges
   \see enumsubranges

  Model of $Gral GridVertex Range, $GrAL CellRange.<BR>

  This class has reference-semantics.

  There is no check if the different element ranges are consistent,
  i.e. if the sequence of vertices is exactly the set of adjacent
  vertices to the sequence of cells. This has to be ensured by
  the underlying grid range.

  \todo Parameterize over the type of the underlying grid range.
*/

  template<class Grid, class GT = grid_types<Grid> >
  class enumerated_subrange_ref : public archetypes_from_base<enumerated_subrange_ref<Grid,GT>, Grid>  {
    typedef enumerated_subrange_ref<Grid,GT> self;
public:
  using archetypes_from_base<enumerated_subrange_ref<Grid>, Grid>::handle;

  typedef self grid_type;
  typedef Grid base_grid_type;
  typedef grid_types<Grid> gt;
  typedef typename gt::size_type size_type;

  //---------- own types ----------------------

    typedef enumerated_subrange<Grid,GT>         range_type;
    typedef typename range_type::vertex_range_type vertex_range_ref_t;
    typedef typename range_type::cell_range_type   cell_range_ref_t;

    typedef enumerated_vertex_range<base_grid_type> vertex_range_t;
    typedef enumerated_cell_range  <base_grid_type> cell_range_t;

    typedef typename vertex_range_ref_t::const_iterator  v_iterator;
    typedef typename cell_range_ref_t  ::const_iterator  c_iterator;

  //------- wrapped referenced types ------------------

  typedef typename gt::Vertex           base_vertex_type;
  typedef typename gt::Cell             base_cell_type;
    typedef typename gt::vertex_handle  base_vertex_handle;
    typedef typename gt::cell_handle    base_cell_handle;

    /*
  typedef vertex_handle_int<self>       vertex_handle;
  typedef wrap_vertex_t<self>           Vertex;
  typedef Vertex                        VertexIterator;
  typedef cell_handle_int<self>         cell_handle;
  typedef wrap_cell_t<self>             Cell;
  typedef Cell                          CellIterator;
    */  

    typedef wrapped_element_handle<self, base_vertex_handle> vertex_handle;
    typedef wrapped_element_handle<self, base_cell_handle>   cell_handle;
    typedef wrapped_element<self, typename gt::Vertex, vertex_handle> Vertex;
    typedef wrapped_element<self, typename gt::Cell,   cell_handle  > Cell;
    
    typedef sequence_element_iterator<Vertex, grid_type, v_iterator> VertexIterator;
    typedef sequence_element_iterator<Cell,   grid_type, c_iterator> CellIterator;

    // base_vertex_type BaseVertex(vertex_handle h) const { return base_vertex_type(BaseGrid(), *(vertices.begin() +h));}
    //base_cell_type   BaseCell  (cell_handle h)   const { return base_cell_type  (BaseGrid(), *(cells   .begin() +h));}

  typedef tp<Vertex> tpV;
  typedef tp<Cell>   tpC;

private:
  //---- DATA ---------
  vertex_range_ref_t vertices;
  //  edge_range_ref   edges;
  cell_range_ref_t   cells;
  // note : consistency in the sence that vertices = V(cells)
  // (set of incident vertices of cells) cannot be ensured here!

public:
  //-------------------- construction --------------------------

  enumerated_subrange_ref()  {}
  enumerated_subrange_ref(const range_type& R)
    : vertices(R.vertex_range()), cells(R.cell_range()) {}
  enumerated_subrange_ref(const vertex_range_ref_t& V,
			  const cell_range_ref_t  & C)
    : vertices(V), cells(C) {}

  enumerated_subrange_ref(const vertex_range_t& V,
			  const cell_range_t  & C)
    : vertices(V.range()),
      cells   (C.range())
  {}

  //-------------------- iteration  ----------------------------

  v_iterator begin(tp<Vertex>) const { return vertices.begin();}
  v_iterator end  (tp<Vertex>) const { return vertices.end  ();}
  c_iterator begin(tp<Cell>  ) const { return cells   .begin();}
  c_iterator end  (tp<Cell>  ) const { return cells   .end  ();}

  size_type NumOfCells()    const { return cells.size();}
  size_type NumOfVertices() const { return vertices.size();}
  CellIterator   FirstCell()   const { return CellIterator  (*this, begin(tpC()), end(tpC())); }
  CellIterator   EndCell()     const { return CellIterator  (*this, end  (tpC()), end(tpC())); }
  VertexIterator FirstVertex() const { return VertexIterator(*this, begin(tpV()), end(tpV())); }
  VertexIterator EndVertex()   const { return VertexIterator(*this, end  (tpV()), end(tpV())); }

    /*
  VertexIterator FirstVertex() const { return VertexIterator(*this);}
  CellIterator   FirstCell()   const { return CellIterator(*this);}

  CellIterator   EndCell()   const { return CellIterator(*this, NumOfCells());}
  VertexIterator EndVertex() const { return VertexIterator(*this, NumOfVertices());}
    */

  vertex_range_ref_t range(tpV) const { return vertices; }
  cell_range_ref_t   range(tpC) const { return cells; }

  base_grid_type const& TheGrid () const { return vertices.TheGrid();}
  base_grid_type const& BaseGrid() const { return vertices.TheGrid();}
};
  

  namespace detail {

    template<class Grid, class GT  = grid_types<Grid> >
    struct grid_types_esr_ref  : 
      public archetypes_from_base_grid_types<GT>,
      public grid_types_detail::grid_types_root 
    {
      typedef enumerated_subrange_ref<Grid,GT>    grid_type;
      typedef typename GT::dimension_tag          dimension_tag;
      typedef typename grid_type::size_type      size_type;
      
      typedef typename grid_type::Vertex         Vertex;
      typedef typename grid_type::vertex_handle  vertex_handle;
      typedef typename grid_type::Cell           Cell;
      typedef typename grid_type::cell_handle    cell_handle;
      
      typedef typename grid_type::VertexIterator VertexIterator;
      typedef typename grid_type::CellIterator   CellIterator;

    };

    template<class Grid, class GT>
    struct grid_types_esr_ref2 
      : public wrap_all_downward_incidence_iterator_types<grid_types_esr_ref<Grid,GT>, GT>
    {
      typedef grid_types_esr_ref2<Grid,GT> self;
      typedef closure_iterator<typename self::CellIterator,
			       typename self::EdgeOnCellIterator,
			       self>
                 			EdgeIterator;
      typedef closure_iterator<typename self::CellIterator,
			       typename self::FacetOnCellIterator,
			       self>    FacetIterator;
  
    };


  } // namespace detail 


/*! Partial specializaton of grid_types traits for enumerated_subrange_ref<Grid>
    \ingroup traits enumsubranges
    
    \see Module traits
  */
template<class Grid, class GT>
struct grid_types<enumerated_subrange_ref<Grid,GT> >
  :  public grid_types_base<detail::grid_types_esr_ref2<Grid,GT> > {};


template<class Grid, class GT, class E, class EH>
struct element_traits<wrapped_element<enumerated_subrange_ref<Grid,GT>, E, EH> >
  : public element_traits_base<wrapped_element<enumerated_subrange_ref<Grid,GT>, E, EH> >
{
private:
  typedef element_traits_base<wrapped_element<enumerated_subrange_ref<Grid,GT>, E, EH> > base;
public:
  typedef typename base::hasher_type_elem_base       hasher_type;
};





#define gt grid_types<enumerated_subrange_ref<Grid> >


  template<class Grid, class GT>
typename gt::VertexIterator
  gral_begin(enumerated_subrange_ref<Grid,GT> const& g, typename gt::VertexIterator)
{ return g.FirstVertex();}

  template<class Grid, class GT>
typename gt::VertexIterator
gral_end  (enumerated_subrange_ref<Grid,GT> const& g, typename gt::VertexIterator)
{ return g.EndVertex();}

  template<class Grid, class GT>
typename gt::size_type
gral_size (enumerated_subrange_ref<Grid,GT> const& g, typename gt::VertexIterator)
{ return g.NumOfVertices();}



template<class Grid, class GT>
typename gt::CellIterator
gral_begin(enumerated_subrange_ref<Grid,GT> const& g, typename gt::CellIterator)
{ return g.FirstCell();}

template<class Grid, class GT>
typename gt::CellIterator
gral_end  (enumerated_subrange_ref<Grid,GT> const& g, typename gt::CellIterator)
{ return g.EndCell();}

template<class Grid, class GT>
typename gt::size_type
gral_size (enumerated_subrange_ref<Grid,GT> const& g, typename gt::CellIterator)
{ return g.NumOfCells();}


#undef gt


template<class Grid, class GT, class E, class EH, class T>
class grid_function<wrapped_element<enumerated_subrange_ref<Grid,GT>, E, EH>, T>
  : public grid_function_hash<wrapped_element<enumerated_subrange_ref<Grid,GT>, E, EH>, T>
{
  typedef grid_function_hash<wrapped_element<enumerated_subrange_ref<Grid,GT>, E, EH>, T> base;
  typedef enumerated_subrange_ref<Grid,GT>                                                grid_type;
public:
  grid_function() {}
  grid_function(grid_type const& g) : base(g) {}
  grid_function(grid_type const& g, T const& t) : base(g,t) {}
  grid_function(ref_ptr<grid_type const> g) : base(g) {}
  grid_function(ref_ptr<grid_type const> g, T const& t) : base(g,t) {}
};




} // namespace GrAL

#endif
