#ifndef GRAL_BASE_GB_ENUMERATED_SUBRANGE_H
#define GRAL_BASE_GB_ENUMERATED_SUBRANGE_H



/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti, 
    Contact: guntram AT users DOT berlios DOT de

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


/*! \file
 */

#include "Config/compiler-config.h"
#include "Utility/pre-post-conditions.h"
#include "Utility/ref-ptr.h"

#include "Container/integer-iterator.h"

#include "Gral/Base/common-grid-basics.h"
#include "Gral/Base/archetypes-from-base.h"
#include "Gral/Base/map-element-iter-names.h"
#include "Gral/Base/wrap-elements.h"

#include "Gral/Subranges/enumerated-element-range-ref.h"
#include "Gral/Iterators/sequence-element-iterator.h"
#include "Gral/Iterators/closure-iterator.h"

#include "Gral/Base/element-handle.h"
#include "Gral/Base/grid-function-hash.h"
#include "Gral/Base/element-numbering.h"

#include <vector>   


namespace GrAL {



/*! \brief  A grid subrange that is a simple enumeration of its elements.
   \ingroup enumsubranges
   \see enumsubranges

  Model of $GrAL GridVertex Range, $GrAL CellRange.<BR>

  This class has value-semantics, i.e. really stores
  the enumerated sequence.

  The main purpose of enumerated_subrange is to implement a
  cell-based subrange of a grid by explicitely enumerating its cells,
  and giving access to elements of lower dimension by computing the closure
  of the cell range.
  Cells can be added at once or in a piecemeal fashion.

  For historical reasons, enumerated_subranges also permits
  addition of vertices. If vertex addition is used, 
  there is not guarantee that the subrange is  <em>closed</em>,
  i.e. that the sequence of vertices is exactly 
  the set of vertices incident to the sequence of cells.
  In the long run, vertex insertion will be removed,
  and a special class (multi_enumerated_element_range or so, which collects
  unrelated sequences of elements of different dimension) 
  will be implemented for algorithms needing this.

  There is a trade-off between storage space efficiency and grid function efficiency.
  For instance, when storing vertices explicitely, it is possible to define consecutive
  vertex handles, allowing the use of vector-based grid functions. However, in order
  to permit the reuse of incidence information of the base grid, there must be a way to convert
  a GT::VertexOnCellIterator to a vertex of the subrange, which involves an additional mapping 
  of base grid vertices to its subrange copies.
  If many grid functions on the subrange vertices are needed, the consecutive option would be
  better, else the non-consecutive version (reusing the handles from the base grid).
  Currently, the latter, non-consecutive version is choosen.
  This could be made into a compile-time configuration option.    

  \todo Make the closure property a compile time or runtime policy.
  It is not consistent to allow both vertex and cell insertion if mixed insertion
  will fail. It is better to use a vertex range if only vertices are wanted
  (In some cases, algorithms formally expect also CellIterator types. Either these
  algorithms should be redesigned, or formal definitions of empty CellIterator types
   must be added to element sequences.)
*/

template<class Grid, class GT = grid_types<Grid> >
class enumerated_subrange : public archetypes_from_base<enumerated_subrange<Grid>, Grid>  {
  typedef enumerated_subrange<Grid, GT> self;
public:
  using archetypes_from_base<enumerated_subrange<Grid, GT>, Grid>::handle;

  //------- referenced types ------------------
  typedef self grid_type;
  typedef Grid base_grid_type;
  typedef GT   gt;
  typedef typename gt::size_type size_type;
  typedef grid_view_category     category;

  enum { dim = base_grid_type::dim };

  typedef typename gt::vertex_handle    base_vertex_handle;
  typedef typename gt::cell_handle      base_cell_handle;
  typedef typename gt::Vertex           base_vertex_type;
  typedef typename gt::Cell             base_cell_type;

  /*
  typedef vertex_handle_int<self>       vertex_handle;
  typedef wrap_vertex_t<self>           Vertex;
  typedef cell_handle_int<self>         cell_handle;
  typedef wrap_cell_t<self>             Cell;
  */

  //  typedef typename gt::vertex_handle vertex_handle;
  //typedef typename gt::cell_handle   cell_handle;
  typedef wrapped_element_handle<self, base_vertex_handle> vertex_handle;
  typedef wrapped_element_handle<self, base_cell_handle>   cell_handle;
  typedef wrapped_element<self, typename gt::Vertex, vertex_handle> Vertex;
  typedef wrapped_element<self, typename gt::Cell,   cell_handle  > Cell;

  base_vertex_type BaseVertex(vertex_handle h)   const { return base_vertex_type(BaseGrid(), *(vertices.begin() +h));}
  base_cell_type   BaseCell  (cell_handle   h)   const { return base_cell_type  (BaseGrid(), *(cells   .begin() +h));}

  typedef tp<Vertex> tpV;
  typedef tp<Cell>   tpC;

  //---------- own types ----------------------
  typedef std::vector<base_vertex_handle> vertex_sequence;
  typedef std::vector<base_cell_handle>   cell_sequence;

  //  typedef integer_iterator<vertex_handle> v_iterator;
  //  typedef integer_iterator<cell_handle>   c_iterator;
  typedef typename vertex_sequence::const_iterator v_iterator;
  typedef typename cell_sequence  ::const_iterator c_iterator;

  typedef vertex_range_ref<base_grid_type, vertex_sequence>  vertex_range_type;
  typedef cell_range_ref  <base_grid_type, cell_sequence  >  cell_range_type;

  typedef sequence_element_iterator<Vertex, grid_type, v_iterator> VertexIterator;
  typedef sequence_element_iterator<Cell,   grid_type, c_iterator> CellIterator;


private:
  ref_ptr<const base_grid_type> the_grid;

  vertex_sequence vertices;
  cell_sequence   cells;
  // note : consistency in the sence that vertices = V(cells)
  // (set of adj. vertices of cells) cannot be ensured here!
  // It is ensured by calling init() before each access to vertices
  mutable bool initialized;
public:
  //-------------------- construction --------------------------

  /*! \name constructors 
   */
  //@{
  /*! \brief  Default constructor, results in unbound range.
   */
  enumerated_subrange()                           :              initialized(false) { init_counts(); }
  /*! \brief Construct empty subrange
   */
  enumerated_subrange(const base_grid_type& g)         : the_grid(g), initialized(false) { init_counts(); }
  enumerated_subrange(ref_ptr<const base_grid_type> g) : the_grid(g), initialized(false) { init_counts(); }


  /*! \brief Copy from cell range \c CR  
 
   */

  template<class CELLRANGE>
  enumerated_subrange(ref_ptr<const base_grid_type> g, 
		      ref_ptr<CELLRANGE> CR) :  
    the_grid(g), initialized(false) 
  { 
    construct(CR->FirstCell());
    init_counts();
  }
  /*! \brief Copy from cell range \c CR  
    
   */
  template<class CELLRANGE>
  enumerated_subrange(const base_grid_type& g, CELLRANGE const& CR) :  
    the_grid(g), initialized(false) 
  { 
    construct(CR.FirstCell());
    init_counts();
  }
  //@}

  //! \brief delayed constructor, constructs empty subrange
  void set_grid(const base_grid_type&         g) { init(g);}
  //! \brief delayed constructor, constructs empty subrange
  void set_grid(ref_ptr<const base_grid_type> g) { init(g);}


  //! \brief delayed constructor, constructs empty subrange
  void init(ref_ptr<const base_grid_type> g) { clear(); the_grid = g; initialized = false; init_counts(); }
  //! \brief delayed constructor, constructs empty subrange
  void init(const base_grid_type& g) { init(const_ref_to_ref_ptr(g));}


  //! make this an empty range. 
  void clear() { 
    vertices.clear();
    cells   .clear();
    initialized = false;
  }


  template<class CELLIT>
  void construct(CELLIT c);

  void init_counts() const { } // num_of_edges = -1; num_of_facets = -1;}

  void append_cell  (base_cell_handle   c) { cells.push_back(c); initialized = false;}
  void append_cell  (base_cell_type  const&  c) { append_cell(c.handle());}
  void push_back(base_cell_handle   c) { append_cell  (c);}
  void push_back(base_cell_type   const& c) { append_cell  (c);}

  // These are obsolete - but enumerated_subrange is used in contexts
  // where something like enumerated_multi_element_range would be more appropriate
  // (without any relationship between elements of different dimensions)
  void append_vertex(base_vertex_handle v) { vertices.push_back(v);}
  void append_vertex(base_vertex_type const& v) { append_vertex(v.handle());}
  void push_back(base_vertex_handle v) { append_vertex(v);}
  void push_back(base_vertex_type const& v) { append_vertex(v);}
  

  //-------------------- iteration  ----------------------------
  /*
  v_iterator begin(tp<Vertex>) const { return v_iterator(0); } // vertices.begin();}
  v_iterator end  (tp<Vertex>) const { return v_iterator(vertices.size());} // vertices.end  ();}
  c_iterator begin(tp<Cell>  ) const { return c_iterator(0); }//  cells   .begin();}
  c_iterator end  (tp<Cell>  ) const { return c_iterator(cells.size());} // cells   .end  ();}
  */
  v_iterator begin(tp<Vertex>) const { init(); return vertices.begin();}
  v_iterator end  (tp<Vertex>) const { init(); return vertices.end  ();}
  c_iterator begin(tp<Cell>  ) const { init(); return cells   .begin();}
  c_iterator end  (tp<Cell>  ) const { init(); return cells   .end  ();}

  size_type NumOfCells()    const { init(); return cells.size();}
  size_type NumOfVertices() const { init(); return vertices.size();}

  CellIterator   FirstCell()   const { init(); return CellIterator  (*this, begin(tpC()), end(tpC())); }
  CellIterator   EndCell()     const { init(); return CellIterator  (*this, end  (tpC()), end(tpC())); }
  VertexIterator FirstVertex() const { init(); return VertexIterator(*this, begin(tpV()), end(tpV())); }
  VertexIterator EndVertex()   const { init(); return VertexIterator(*this, end  (tpV()), end(tpV())); }


  vertex_range_type range(tpV) const 
    { init(); return vertex_range_type(0, vertices.size(), vertices, *the_grid);}
  vertex_range_type vertex_range() const 
    { init(); return vertex_range_type(0, vertices.size(), vertices, *the_grid);}
  vertex_range_type Vertices() const 
    { init(); return vertex_range_type(0, vertices.size(), vertices, *the_grid);}

  cell_range_type   range(tpC) const 
    { init(); return cell_range_type  (0,(int) cells.size(),    cells,    *the_grid);}
  cell_range_type   cell_range() const 
    { init(); return cell_range_type  (0,(int) cells.size(),    cells,    *the_grid);}
  cell_range_type   Cells() const 
    { init(); return cell_range_type  (0,(int) cells.size(),    cells,    *the_grid);}

  const base_grid_type& TheGrid()  const { return *the_grid;}
  const base_grid_type& BaseGrid() const { return *the_grid;}
  bool bound() const { return the_grid != 0;}
  bool empty() const { return NumOfCells() == 0 && NumOfVertices() == 0;}

  //  cell_handle   handle(const Cell& C)   const { return TheGrid().handle(C);}
  // vertex_handle handle(const Vertex& V) const { return TheGrid().handle(V);}

private:
  // compute the closure of the cells.
  // NOTE: vertices must be empty
   void init() const;		     
};
  





namespace detail {

template<class Grid, class GT  = grid_types<Grid> >
struct grid_types_esr :
    public archetypes_from_base_grid_types<GT>,
    public grid_types_detail::grid_types_root 
{
  typedef enumerated_subrange<Grid, GT>       grid_type;
  typedef typename GT::dimension_tag          dimension_tag;

  typedef typename grid_type::size_type      size_type;

  typedef typename grid_type::vertex_handle  vertex_handle;
  typedef typename grid_type::Vertex         Vertex;
  typedef typename grid_type::VertexIterator VertexIterator;

  typedef typename grid_type::cell_handle    cell_handle;
  typedef typename grid_type::Cell           Cell;
  typedef typename grid_type::CellIterator   CellIterator;
};

  template<class Grid, class GT>
  struct grid_types_esr2 : public wrap_all_downward_incidence_iterator_types<grid_types_esr<Grid,GT>, GT>
  {
    typedef grid_types_esr2<Grid,GT> self;
    typedef closure_iterator<typename self::CellIterator,
			     typename self::EdgeOnCellIterator,
			     self>    EdgeIterator;
    typedef closure_iterator<typename self::CellIterator,
			     typename self::FacetOnCellIterator,
			     self>    FacetIterator;

  };




} // namespace detail


/*! Partial specializaton of grid_types traits for enumerated_subrange<Grid>.
    \ingroup traits enumsubranges

    \see Module traits
 */
  template<class Grid, class GT>
  struct grid_types<enumerated_subrange<Grid, GT> >
    :  public grid_types_base<detail::grid_types_esr2<Grid,GT> > {};


template<class Grid, class GT, class E, class EH>
struct element_traits<wrapped_element<enumerated_subrange<Grid,GT>, E, EH> >
  : public element_traits_base<wrapped_element<enumerated_subrange<Grid,GT>, E, EH> >
{
private:
  typedef element_traits_base<wrapped_element<enumerated_subrange<Grid,GT>, E, EH> > base;
public:
  typedef typename base::hasher_type_elem_base       hasher_type;
};


#define gt grid_types<enumerated_subrange<Grid> >


  template<class Grid, class GT>
typename gt::VertexIterator
  gral_begin(enumerated_subrange<Grid,GT> const& g, typename gt::VertexIterator)
{ return g.FirstVertex();}

  template<class Grid, class GT>
typename gt::VertexIterator
gral_end  (enumerated_subrange<Grid,GT> const& g, typename gt::VertexIterator)
{ return g.EndVertex();}

  template<class Grid, class GT>
typename gt::size_type
gral_size (enumerated_subrange<Grid,GT> const& g, typename gt::VertexIterator)
{ return g.NumOfVertices();}



template<class Grid, class GT>
typename gt::CellIterator
gral_begin(enumerated_subrange<Grid,GT> const& g, typename gt::CellIterator)
{ return g.FirstCell();}

template<class Grid, class GT>
typename gt::CellIterator
gral_end  (enumerated_subrange<Grid,GT> const& g, typename gt::CellIterator)
{ return g.EndCell();}

template<class Grid, class GT>
typename gt::size_type
gral_size (enumerated_subrange<Grid,GT> const& g, typename gt::CellIterator)
{ return g.NumOfCells();}


#undef gt


  template<class Grid, class GT, class E, class EH, class T>
  class grid_function<wrapped_element<enumerated_subrange<Grid,GT>, E, EH>, T>
    : public grid_function_hash<wrapped_element<enumerated_subrange<Grid,GT>, E, EH>, T>
  {
    typedef grid_function_hash<wrapped_element<enumerated_subrange<Grid,GT>, E, EH>, T> base;
    typedef enumerated_subrange<Grid,GT>                                                grid_type;
  public:
    grid_function() {}
    grid_function(grid_type const& g) : base(g) {}
    grid_function(grid_type const& g, T const& t) : base(g,t) {}
    grid_function(ref_ptr<grid_type const> g) : base(g) {}
    grid_function(ref_ptr<grid_type const> g, T const& t) : base(g,t) {}
  };


} // namespace GrAL 

#ifdef NMWR_INCLUDE_TEMPLATE_DEFS
#include "Gral/Subranges/enumerated-subrange.C"
#endif


#endif
