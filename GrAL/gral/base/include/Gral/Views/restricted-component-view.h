#ifndef GRAL_GB_VIEWS_RESTRICTED_COMPONENT_VIEW_H
#define GRAL_GB_VIEWS_RESTRICTED_COMPONENT_VIEW_H

// $LICENSE_NEC

#include "Gral/Base/common-grid-basics.h"
#include "Gral/Base/archetypes-from-base.h"
#include "Gral/Subranges/enumerated-subrange.h"
#include "Gral/Base/grid-function-hash.h"

#include "Gral/Iterators/closure-iterator.h"
#include "Gral/Iterators/iterator-base.h"

#include "Gral/Base/wrap-elements.h"


namespace GrAL {


/*! \brief Provide a view on a connected grid part defined by a predicate

    \ingroup gridviews

    A typical use would be to restrict analysis to a small patch
    of a given grid in the neighborhood of some cell, bounded
    by some distance function.
    A typical usage would be the intersection of a grid with a
    geometric entity, when only one component of the intersection
    is of interest. For example, when determining the position where
    a ray (or cylinder if it has a thickness) enters a grid,
    only the first intersection with the grid is of interest.

 

   \todo implement Edge/Facet if supported by \c GRID
   \see \ref gridviews module
   \see Test in \ref test-restricted-components-view.C

*/

namespace restricted_grid_component_view { 

  template<class GRID, class INSIDE_PRED, class GT>  class vertex;
  template<class GRID, class INSIDE_PRED, class GT>  class vertex_iterator;
  template<class GRID, class INSIDE_PRED, class GT>  class cell;
  template<class GRID, class INSIDE_PRED, class GT>  class cell_iterator;
  template<class GRID, class INSIDE_PRED, class GT>  class vertex_on_cell_iterator;
  template<class GRID, class INSIDE_PRED, class GT>  class grid_view;


  template<class GRID, class INSIDE_PRED, class GT>
  struct grid_types_grid_view_b 
    : public grid_types_detail::grid_types_root, 
      public archetypes_from_base_grid_types<GT> 
  {
    typedef GRID                             base_grid_type;
    typedef grid_view<GRID, INSIDE_PRED, GT> grid_type;
    typedef GT                                             bgt;

    typedef typename  bgt::size_type                       size_type;
    typedef typename  bgt::dimension_tag                   dimension_tag;
    typedef typename  bgt::vertex_handle                   vertex_handle;
    typedef typename  bgt::cell_handle                     cell_handle;

    typedef vertex                 <GRID,INSIDE_PRED, GT> Vertex;
    typedef cell                   <GRID,INSIDE_PRED, GT> Cell;
    typedef vertex_iterator        <GRID,INSIDE_PRED, GT> VertexIterator;
    typedef cell_iterator          <GRID,INSIDE_PRED, GT> CellIterator;
    typedef vertex_on_cell_iterator<GRID,INSIDE_PRED, GT> VertexOnCellIterator;

    typedef typename bgt::Vertex BaseVertex;
    typedef typename bgt::Cell   BaseCell;
  };

  template<class GRID, class INSIDE_PRED, class GT>
  struct grid_types_grid_view_b2 :
    public wrap_all_downward_incidence_iterator_types<grid_types_grid_view_b<GRID, INSIDE_PRED, GT>, GT>
  {};

  template<class GRID, class INSIDE_PRED, class GT>
  struct grid_types_grid_view : public grid_types_grid_view_b2<GRID, INSIDE_PRED, GT> 
  {
    typedef grid_types_grid_view_b2<GRID, INSIDE_PRED, GT>  base;
    typedef closure_iterator<typename base::CellIterator, typename base::EdgeOnCellIterator, base> EdgeIterator;

    static typename base::cell_handle invalid_cell_handle(typename base::grid_type const& g)
    {  return GT::invalid_cell_handle(g.BaseGrid()); }
  };

  /*! \brief The restricted grid component view class

    This class is a model of $GrAL Cell-VertexInputGridRange.
    Grid functions are defined for the vertex type.

    \templateparams
    - GRID:
      - Vertex/VertexIterator
      - Cell/CellIterator
      - CellOnCellIterator
    - INSIDE_PRED
      - decide whether a cell is to be considered inside,
        <tt> bool operator()(Cell) </tt>

  */
  template<class GRID, class INSIDE_PRED, class GT = grid_types<GRID> > 
  class grid_view : public archetypes_from_base<grid_view<GRID, INSIDE_PRED, GT>, GRID, GT>  {
  public:
    typedef grid_view<GRID, INSIDE_PRED, GT> self;
    typedef grid_types_grid_view_b <GRID, INSIDE_PRED, GT> 
                                             gt;
    typedef GT                               bgt;
    typedef self                             grid_type;
    typedef GRID                             base_grid_type;
    typedef INSIDE_PRED                      pred_type;
    
    typedef typename  bgt::Vertex               baseVertex;
    typedef typename  bgt::Cell                 baseCell;
    typedef typename  bgt::CellOnCellIterator   baseCellOnCellIterator;
    typedef typename  bgt::VertexOnCellIterator baseVertexOnCellIterator;
    typedef typename  bgt::vertex_handle        vertex_handle;
    typedef typename  bgt::cell_handle          cell_handle;

    
    typedef  typename gt::Vertex         Vertex;
    typedef  typename gt::VertexIterator VertexIterator;
    typedef  typename gt::Cell           Cell;
    typedef  typename gt::CellIterator   CellIterator;
    
    typedef enumerated_subrange<GRID> range_type;
  private:
    ref_ptr<base_grid_type const>  g;
    pred_type                 inside;
    range_type                germs;

    mutable range_type range;
    mutable bool       cells_initialized;
    mutable bool       vertices_initialized;
  public:
    grid_view() {}
    grid_view(base_grid_type const& gg,
	      pred_type        ins,
	      baseCell  const& grm);
    template<class IT>
    grid_view(base_grid_type const& gg,
	      pred_type        ins,
	      IT begin_germ, IT end_germ);

    void init(base_grid_type const& gg,
	      pred_type        ins,
	      baseCell  const& grm);

    template<class IT>
    void init(base_grid_type const& gg,
	      pred_type        ins,
	      IT begin_germ, IT end_germ);

    void clear();

    friend class restricted_grid_component_view::vertex_iterator<GRID,INSIDE_PRED, GT>;
    friend class restricted_grid_component_view::cell_iterator  <GRID,INSIDE_PRED, GT>;
    friend class vertex<GRID,INSIDE_PRED, GT>;

    VertexIterator FirstVertex()   const { return VertexIterator(*this);}
    VertexIterator EndVertex()     const { return VertexIterator(*this, TheRange().EndVertex());}
    unsigned       NumOfVertices() const { return TheRange().NumOfVertices();}
    CellIterator   FirstCell()     const { return CellIterator(*this);}
    CellIterator   EndCell()       const { return CellIterator(*this, TheRange().EndCell());}
    unsigned       NumOfCells()    const { return TheRange().NumOfCells();}

    
    base_grid_type const& BaseGrid() const { return *g;}
    base_grid_type const& TheGrid()  const { return *g;}
    range_type     const& TheRange() const { init_cells(); init_vertices(); return range;}
  private:
    void init_cells()    const;
    void init_vertices() const;
  };



  template<class GRID, class INSIDE_PRED, class GT = grid_types<GRID> >
  class vertex {
    typedef vertex<GRID,INSIDE_PRED, GT> self;
  public:
    typedef vertex_type_tag                       element_type_tag;
    typedef grid_view<GRID,INSIDE_PRED, GT>       grid_type;
    typedef typename grid_type::baseVertex        baseVertex;
    typedef typename grid_type::vertex_handle     vertex_handle;

  private:
    grid_type   const*  g;
    baseVertex          v;
  public:
    vertex() : g(0) {}
    vertex(grid_type const& gg) 
      : g(&gg), v(* gg.TheRange().FirstVertex()) {}

    /*! \brief Construct from handle
        \pre \c vv is contained in  vertices of \c gg 
    */
    vertex(grid_type const& gg, vertex_handle vv) 
      : g(&gg), v(gg.BaseGrid(), vv) {}

    operator baseVertex() const { return v;}
    baseVertex Base() const { return v;}
    vertex_handle handle() const { return v.handle();}
    grid_type const& TheGrid () const { return *g;} 
    grid_type const& BaseGrid() const { return *g;} 

    bool operator==(self const& rhs) const { return v == rhs.v;}
    bool operator!=(self const& rhs) const { return !(*this == rhs);}
  };


  template<class GRID, class INSIDE_PRED, class GT = grid_types<GRID> >
  class vertex_iterator : public sequence_iter_base<grid_types_grid_view<GRID,INSIDE_PRED,GT>,
						    vertex_type_tag> 
  {
    typedef vertex_iterator<GRID,INSIDE_PRED, GT> self;
  public:
    typedef grid_view<GRID,INSIDE_PRED, GT>              grid_type;
    typedef grid_types<typename grid_type ::range_type>  rgt;
    typedef typename rgt::VertexIterator          rangeVertexIterator; 
    typedef typename grid_type::baseVertex        baseVertex;
    typedef typename grid_type::Vertex            Vertex;
    typedef typename grid_type::vertex_handle     vertex_handle;

    typedef grid_type anchor_type;
    typedef Vertex    value_type;
  private:
    grid_type   const*  g;
    rangeVertexIterator v;
  public:
    vertex_iterator() : g(0) {}
    vertex_iterator(grid_type const& gg)                         : g(&gg), v(gg.TheRange().FirstVertex()) {}
    vertex_iterator(grid_type const& gg, rangeVertexIterator vv) : g(&gg), v(vv) {}

    self& operator++() { ++v; return *this;}
    Vertex operator*() const { return Vertex(*g, handle());}
    operator baseVertex() const { return *v;}
    baseVertex Base() const { return *v;}
    bool  IsDone() const { return v.IsDone();}
    grid_type const& TheGrid   () const { return *g;} 
    grid_type const& TheAnchor () const { return *g;} 

    //    grid_type const& BaseGrid() const { return *g;} 

    vertex_handle handle() const { return v.handle();}

    bool operator==(self const& rhs) const { return v == rhs.v;}
    bool operator!=(self const& rhs) const { return !(*this == rhs);}

    bool bound() const { return g!=0;}
    bool valid() const { return v.valid();}
  };




  template<class GRID, class INSIDE_PRED, class GT = grid_types<GRID> >
  class cell {
    typedef cell<GRID,INSIDE_PRED, GT> self;
  public:
    typedef cell_type_tag                                element_type_tag;
    typedef grid_view<GRID,INSIDE_PRED, GT>              grid_type;
    typedef typename grid_type::baseCell        baseCell;
    typedef typename grid_type::cell_handle     cell_handle;

    typedef grid_type anchor_type;
    typedef self      value_type;
  private:
    grid_type   const*  g;
    cell_handle         c;
  public:
    cell() : g(0) {}
    cell(grid_type const& gg, cell_handle cc) : g(&gg), c(cc) {}
    cell(grid_type const& gg, baseCell const& bc) : g(&gg), c(bc.handle()) {}

    operator baseCell() const { return Base();}
    baseCell     Base() const { return baseCell(g->BaseGrid(),c);}
    cell_handle handle()     const { return c;}
    unsigned NumOfVertices() const { return Base().NumOfVertices();}

    grid_type const& TheGrid  () const { return *g;} 

    bool operator==(self const& rhs) const { return c == rhs.c;}
    bool operator!=(self const& rhs) const { return !(*this == rhs);}

    bool bound() const { return g!=0;}
  };




  template<class GRID, class INSIDE_PRED, class GT = grid_types<GRID> >
  class cell_iterator : public sequence_iter_base<grid_types_grid_view<GRID,INSIDE_PRED,GT>,
						  cell_type_tag> 
  {
    typedef cell_iterator<GRID,INSIDE_PRED, GT> self;
  public:
    typedef cell_type_tag                                element_type_tag;
    typedef grid_view<GRID,INSIDE_PRED, GT>              grid_type;
    typedef grid_types<typename grid_type ::range_type>  rgt;
    typedef typename rgt::CellIterator          RangeCellIterator; 
    typedef typename grid_type::baseCell        baseCell;
    typedef typename grid_type::Cell            Cell;
    typedef typename grid_type::cell_handle     cell_handle;

    typedef grid_type anchor_type;
    typedef Cell      value_type;
  private:
    grid_type   const*  g;
    RangeCellIterator   c;
  public:
    cell_iterator() : g(0) {}
    cell_iterator(grid_type const& gg)                                              
      : g(&gg), c(gg.TheRange().FirstCell()) {}
    cell_iterator(grid_type const& gg, RangeCellIterator cc) 
      : g(&gg), c(cc) {}

    self& operator++() { ++c; return *this;}
    Cell  operator*() const { return Cell(TheGrid(), handle());}
    bool  IsDone() const { return c.IsDone();}
    cell_handle handle() const { return c.handle();}

    grid_type const& TheGrid  () const { return *g;} 
    grid_type const& TheAnchor() const { return *g;} 

    bool operator==(self const& rhs) const { return c == rhs.c;}
    bool operator!=(self const& rhs) const { return !(*this == rhs);}

    bool bound() const { return g!=0;}
    bool valid() const { return c.valid();}
  };




  template<class GRID, class INSIDE_PRED, class GT = grid_types<GRID> >
  class vertex_on_cell_iterator 
    : public incidence_iter_base<grid_types_grid_view<GRID,INSIDE_PRED,GT>,
				 vertex_type_tag,
				 cell_type_tag>  
  {
    typedef vertex_on_cell_iterator<GRID,INSIDE_PRED, GT> self;
  public:
    typedef grid_view<GRID,INSIDE_PRED, GT>              grid_type;
    typedef typename grid_type::baseVertexOnCellIterator baseVertexOnCellIterator;
    typedef typename grid_type::vertex_handle            vertex_handle;
    typedef typename grid_type::Cell                     Cell;
    typedef typename grid_type::Vertex                   Vertex;

    typedef Vertex value_type;
    typedef Cell   anchor_type;
  private:
    Cell c;
    baseVertexOnCellIterator vc;
  public:
    vertex_on_cell_iterator()  {}
    vertex_on_cell_iterator(Cell const& cc) 
      : c(cc), vc(c.Base()) {}
    vertex_on_cell_iterator(Cell const& cc, baseVertexOnCellIterator vvc)
      : c(cc), vc(vvc) {}

    self&  operator++() { ++vc; return *this;}
    Vertex operator*() const { return Vertex(TheGrid(),handle());}
    bool   IsDone()    const { return vc.IsDone();}
    vertex_handle handle() const { return vc.handle();}

    grid_type   const& TheGrid()   const { return c.TheGrid();} 
    anchor_type const& TheAnchor() const { return c;}
    anchor_type const& TheCell  () const { return c;}

    bool operator==(self const& rhs) const { return c == rhs.c && vc == rhs.vc;}
    bool operator!=(self const& rhs) const { return !(*this == rhs);}
  };


} // namespace restricted_grid_component_view

template<class GRID, class INSIDE_PRED, class GT>
struct grid_types<restricted_grid_component_view::grid_view<GRID,INSIDE_PRED,GT> >
  : public grid_types_base<restricted_grid_component_view::grid_types_grid_view<GRID,INSIDE_PRED,GT> >
{};

template<class GRID, class INSIDE_PRED, class GT>
struct element_traits<restricted_grid_component_view::vertex<GRID,INSIDE_PRED,GT> >
  : public element_traits_vertex_base<restricted_grid_component_view::grid_view<GRID,INSIDE_PRED,GT> >
{
  typedef element_traits_vertex_base<restricted_grid_component_view::grid_view<GRID,INSIDE_PRED,GT> > base;
  typedef typename base::hasher_type_elem_base       hasher_type;
};

template<class GRID, class INSIDE_PRED, class GT>
struct element_traits<restricted_grid_component_view::cell<GRID,INSIDE_PRED,GT> >
  : public element_traits_cell_base<restricted_grid_component_view::grid_view<GRID,INSIDE_PRED,GT> >
{
  typedef element_traits_cell_base<restricted_grid_component_view::grid_view<GRID,INSIDE_PRED,GT> > base;
  typedef typename base::hasher_type_elem_base       hasher_type;
};



  namespace restricted_grid_component_view {
    
#define gt grid_types<grid_view<GRID,INSIDE_PRED,GT> > 

  template<class GRID, class INSIDE_PRED, class GT>
  vertex_iterator<GRID,INSIDE_PRED, GT> 
  gral_begin(grid_view<GRID,INSIDE_PRED, GT> const& g, vertex_iterator<GRID,INSIDE_PRED, GT>)
  { return g.FirstVertex();}

  template<class GRID, class INSIDE_PRED, class GT>
  vertex_iterator<GRID,INSIDE_PRED, GT> 
  gral_end(grid_view<GRID,INSIDE_PRED, GT> const& g,   vertex_iterator<GRID,INSIDE_PRED, GT>)
  { return g.EndVertex();}

  template<class GRID, class INSIDE_PRED, class GT>
  typename grid_types<grid_view<GRID,INSIDE_PRED, GT> >::size_type
  gral_size(grid_view<GRID,INSIDE_PRED, GT> const& g, vertex_iterator<GRID,INSIDE_PRED, GT>)
  { return g.NumOfVertices();}



  template<class GRID, class INSIDE_PRED, class GT>
  cell_iterator<GRID,INSIDE_PRED, GT> 
  gral_begin(grid_view<GRID,INSIDE_PRED, GT> const& g, cell_iterator<GRID,INSIDE_PRED, GT>)
  { return g.FirstCell();}

  template<class GRID, class INSIDE_PRED, class GT>
  cell_iterator<GRID,INSIDE_PRED, GT> 
  gral_end(grid_view<GRID,INSIDE_PRED, GT> const& g,   cell_iterator<GRID,INSIDE_PRED, GT>)
  { return g.EndCell();}

  template<class GRID, class INSIDE_PRED, class GT>
  typename grid_types<grid_view<GRID,INSIDE_PRED, GT> >::size_type
  gral_size(grid_view<GRID,INSIDE_PRED, GT> const& g, cell_iterator<GRID,INSIDE_PRED, GT>)
  { return g.NumOfCells();}


  template<class GRID, class INSIDE_PRED, class GT>
  vertex_on_cell_iterator<GRID,INSIDE_PRED, GT> 
  gral_begin(cell<GRID,INSIDE_PRED, GT> const& c, vertex_on_cell_iterator<GRID,INSIDE_PRED, GT>)
  { return vertex_on_cell_iterator<GRID,INSIDE_PRED, GT>(c); }

  template<class GRID, class INSIDE_PRED, class GT>
  vertex_on_cell_iterator<GRID,INSIDE_PRED, GT> 
  gral_end  (cell<GRID,INSIDE_PRED, GT> const& c, vertex_on_cell_iterator<GRID,INSIDE_PRED, GT>)
  { return vertex_on_cell_iterator<GRID,INSIDE_PRED, GT>(c, GrAL::end<typename gt::BaseVertex>(c.Base()));}

  template<class GRID, class INSIDE_PRED, class GT>
  typename grid_types<grid_view<GRID,INSIDE_PRED, GT> >::size_type
  gral_size  (cell<GRID,INSIDE_PRED, GT> const& c, vertex_on_cell_iterator<GRID,INSIDE_PRED, GT>)
  { return c.NumOfVertices();}

#undef gt
    
  }

template<class GRID, class INSIDE_PRED, class GT, class T>
class grid_function<restricted_grid_component_view::vertex<GRID,INSIDE_PRED,GT>, T>
  : public grid_function_hash<restricted_grid_component_view::vertex<GRID,INSIDE_PRED,GT>, T>
{
  typedef grid_function_hash<restricted_grid_component_view::vertex<GRID,INSIDE_PRED,GT>, T>  base;
  typedef restricted_grid_component_view::grid_view<GRID,INSIDE_PRED,GT> grid_type;
public:
  grid_function() {}
  grid_function(grid_type const& g) : base(g) {}
  grid_function(grid_type const& g, T const& t) : base(g,t) {}
};

template<class GRID, class INSIDE_PRED, class GT, class T>
class grid_function<restricted_grid_component_view::cell<GRID,INSIDE_PRED,GT>, T>
  : public grid_function_hash<restricted_grid_component_view::cell<GRID,INSIDE_PRED,GT>, T>
{
  typedef grid_function_hash<restricted_grid_component_view::cell<GRID,INSIDE_PRED,GT>, T>  base;
  typedef restricted_grid_component_view::grid_view<GRID,INSIDE_PRED,GT> grid_type;
public:
  grid_function() {}
  grid_function(grid_type const& g) : base(g) {}
  grid_function(grid_type const& g, T const& t) : base(g,t) {}
};

} // namespace GrAL 

#ifdef NMWR_INCLUDE_TEMPLATE_DEFS
#include "restricted-component-view.C"
#endif

#endif
