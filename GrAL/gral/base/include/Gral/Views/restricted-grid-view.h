#ifndef GRAL_GB_VIEWS_RESTRICTED_GRID_VIEW_H
#define GRAL_GB_VIEWS_RESTRICTED_GRID_VIEW_H

// $LICENSE_NEC

#include "Gral/Base/common-grid-basics.h"
#include "Gral/Base/archetypes-from-base.h"
#include "Gral/Subranges/enumerated-subrange.h"
#include "Gral/Base/grid-function-hash.h"

namespace GrAL {

/*! \brief Provide a view on a grid part defined by a  cell predicate

    \ingroup gridviews


   \todo implement Edge/Facet if supported by \c GRID
   \todo reuse archetypes from GRID
   \see \ref gridviews module
   \see Test in \ref test-restricted-components-view.C

*/

namespace restricted_grid_view { 

  template<class GRID, class INSIDE_PRED, class GT> 
  class vertex;
  template<class GRID, class INSIDE_PRED, class GT> 
  class vertex_iterator;
  template<class GRID, class INSIDE_PRED, class GT> 
  class cell_iterator;
  template<class GRID, class INSIDE_PRED, class GT> 
  class vertex_on_cell_iterator;

  template<class GRID, class INSIDE_PRED, class GT>
  struct grid_types_grid_view : public archetypes_from_base_grid_types<GT> {
    typedef GRID        grid_type;

    typedef typename  GT::vertex_handle vertex_handle;
    typedef typename  GT::cell_handle   cell_handle;
    typedef vertex<GRID,INSIDE_PRED, GT>                  Vertex;
    typedef cell_iterator<GRID,INSIDE_PRED, GT>           Cell;
    typedef vertex_iterator<GRID,INSIDE_PRED, GT>         VertexIterator;
    typedef cell_iterator<GRID,INSIDE_PRED, GT>           CellIterator;
    typedef vertex_on_cell_iterator<GRID,INSIDE_PRED, GT> VertexOnCellIterator;

    typedef typename GT::dimension_tag dimension_tag;
  };

  /*! \brief The restricted grid view class

    This class is a model of $GrAL Cell-VertexInputGridRange.
    Grid functions are defined for the vertex type.

    \templateparams
    - GRID:
      - Vertex/VertexIterator
      - Cell/CellIterator
      - VertexOnCellIterator
    - INSIDE_PRED
      - decide whether a cell is to be considered inside,
        <tt> bool operator()(Cell) </tt>

    \see Tested in \ref test-restricted-grid-view.C 
  */
  template<class GRID, class INSIDE_PRED, class GT = grid_types<GRID> >
  class grid_view : public archetypes_from_base<grid_view<GRID, INSIDE_PRED, GT>, GRID, GT>  {
  public:
    typedef GRID        grid_type;
    typedef INSIDE_PRED pred_type;
    typedef GT          basegt;
    
    typedef typename  GT::Vertex baseVertex;
    typedef typename  GT::Cell   baseCell;
    typedef typename  GT::CellIterator         baseCellIterator;
    typedef typename  GT::CellOnCellIterator   baseCellOnCellIterator;
    typedef typename  GT::VertexOnCellIterator baseVertexOnCellIterator;
    typedef typename  GT::vertex_handle vertex_handle;
    typedef typename  GT::cell_handle   cell_handle;
    
    typedef enumerated_vertex_range<GRID> range_type;
  private:
    ref_ptr<grid_type const>  g;
    pred_type                 inside_p;
    
    mutable range_type        range;
    mutable bool              vertices_initialized;
    mutable int               num_of_cells;
  public:
    grid_view() {}
    grid_view(grid_type const& gg,
	      pred_type        ins)
      : g(&gg), inside_p(ins), 
	range(gg),  
	vertices_initialized(false),
	num_of_cells(-1)
    {}

  public:
    typedef  vertex<GRID,INSIDE_PRED, GT>           Vertex;
    typedef  vertex_iterator<GRID,INSIDE_PRED, GT>  VertexIterator;
    typedef  cell_iterator<GRID,INSIDE_PRED, GT>    Cell;
    typedef  cell_iterator<GRID,INSIDE_PRED, GT>    CellIterator;

    VertexIterator FirstVertex()   const { return VertexIterator(*this);}
    unsigned       NumOfVertices() const { init_vertices(); return range.NumOfVertices();}
    CellIterator   FirstCell()     const { return CellIterator(*this);}
    unsigned       NumOfCells()    const { if(num_of_cells < 0) init_num_cells(); return num_of_cells;}

    grid_type const& BaseGrid() const { return *g;}
    grid_type const& TheGrid()  const { return *g;}
    pred_type const& pred()     const { return inside_p;} 
    bool             inside(baseCell const& c)   const { return inside_p(c);}   
  private:
    void init_num_cells()    const;
    void init_vertices() const;

    friend class restricted_grid_view::vertex_iterator<GRID,INSIDE_PRED, GT>;
    friend class restricted_grid_view::vertex         <GRID,INSIDE_PRED, GT>;
    friend class restricted_grid_view::cell_iterator  <GRID,INSIDE_PRED, GT>;

  };






  template<class GRID, class INSIDE_PRED, class GT = grid_types<GRID> >
  class vertex {
    typedef vertex<GRID,INSIDE_PRED, GT> self;
  public:
    typedef grid_view<GRID,INSIDE_PRED, GT>       grid_type;
    typedef typename grid_type::baseVertex        baseVertex;
    typedef typename grid_type::vertex_handle     vertex_handle;
  private:
    grid_type   const*  g;
    baseVertex          v;
  public:
    vertex() : g(0) {}
    vertex(grid_type const& gg) 
      : g(&gg)
      {
	g->init_vertices();
	v = * g->range.FirstVertex();
      }
    vertex(grid_type const& gg, vertex_handle vv) 
      : g(&gg)
      {
	g->init_vertices();
	v = baseVertex(g->BaseGrid(), vv);
      }

    operator baseVertex() const { return v;}
    baseVertex Base() const { return v;}
    vertex_handle handle() const { return v.handle();}
    grid_type const& TheGrid () const { return *g;} 
    grid_type const& BaseGrid() const { return *g;} 

    bool operator==(self const& rhs) const { return v == rhs.v;}
    bool operator!=(self const& rhs) const { return !(*this == rhs);}
  };


  template<class GRID, class INSIDE_PRED, class GT = grid_types<GRID> >
  class vertex_iterator {
    typedef vertex_iterator<GRID,INSIDE_PRED, GT> self;
  public:
    typedef grid_view<GRID,INSIDE_PRED, GT>              grid_type;
    typedef typename grid_type::range_type               range_type;
    typedef typename range_type::VertexIterator          rangeVertexIterator; 
    typedef typename grid_type::baseVertex        baseVertex;
    typedef typename grid_type::Vertex            Vertex;
    typedef typename grid_type::vertex_handle     vertex_handle;
  private:
    grid_type   const*  g;
    rangeVertexIterator v;
  public:
    vertex_iterator() : g(0) {}
    vertex_iterator(grid_type const& gg) 
      : g(&gg)
      {
	g->init_vertices();
	v = g->range.FirstVertex();
      }

    self& operator++() { ++v; return *this;}
    Vertex operator*() const { return Vertex(*g, handle());}
    operator baseVertex() const { return *v;}
    baseVertex Base() const { return *v;}
    bool  IsDone() const { return v.IsDone();}
    grid_type const& TheGrid () const { return *g;} 
    grid_type const& BaseGrid() const { return *g;} 

    vertex_handle handle() const { return v.handle();}

    bool operator==(self const& rhs) const { return v == rhs.v;}
    bool operator!=(self const& rhs) const { return !(*this == rhs);}
    
  };

  // class CellIterator 
  template<class GRID, class INSIDE_PRED, class GT = grid_types<GRID> >
  class cell_iterator {
    typedef cell_iterator<GRID,INSIDE_PRED, GT> self;
  public:
    typedef grid_view<GRID,INSIDE_PRED, GT>              grid_type;
    typedef grid_types<typename grid_type ::range_type>  rgt;
    typedef typename grid_type::baseCell        baseCell;
    typedef typename grid_type::cell_handle     cell_handle;
    typedef typename grid_type::baseCellIterator baseCellIterator;
    //   typedef typename grid_type::VertexOnCellIterator VertexOnCellIterator;
 private:
    grid_type   const*  g;
    baseCellIterator    c;
  public:
    cell_iterator() : g(0) {}
    cell_iterator(grid_type const& gg) 
      : g(&gg)
      {
	c = g->BaseGrid().FirstCell();
	advance_till_valid();
      }

    self& operator++() { ++c; advance_till_valid();  return *this;}
    self  const& operator*() const { return *this;}
    operator baseCell() const { return *c;}
    baseCell  Base() const { return *c;}
    bool  IsDone() const { return c.IsDone();}
    grid_type const& TheGrid() const { return *g;} 
    cell_handle handle() const { return c.handle();}
    unsigned NumOfVertices() const { return Base().NumOfVertices();}
    vertex_on_cell_iterator<GRID,INSIDE_PRED, GT> FirstVertex() const;

    bool operator==(self const& rhs) const { return c == rhs.c;}
    bool operator!=(self const& rhs) const { return !(*this == rhs);}
  private:
    void advance_till_valid() {
      while(! IsDone() && ! g->inside(*c))
	++c;
    }
  };

  // class CellIterator 
  template<class GRID, class INSIDE_PRED, class GT = grid_types<GRID> >
  class vertex_on_cell_iterator {
    typedef vertex_on_cell_iterator<GRID,INSIDE_PRED, GT> self;
  public:
    typedef grid_view<GRID,INSIDE_PRED, GT>              grid_type;
    typedef typename grid_type::baseVertexOnCellIterator baseVertexOnCellIterator;
    typedef typename grid_type::vertex_handle            vertex_handle;
    typedef typename grid_type::Cell                     Cell;
    typedef typename grid_type::Vertex                   Vertex;
  private:
    Cell c;
    baseVertexOnCellIterator vc;
  public:
    vertex_on_cell_iterator()  {}
    vertex_on_cell_iterator(Cell const& cc) : c(cc), vc(c.Base()) {}

    self& operator++() { ++vc; return *this;}
    Vertex operator*() const { return Vertex(TheGrid(),handle());}
    bool  IsDone() const { return vc.IsDone();}
    grid_type const& TheGrid() const { return c.TheGrid();} 
    vertex_handle handle() const { return vc.handle();}

    bool operator==(self const& rhs) const { return c == rhs.c && vc == rhs.vc;}
    bool operator!=(self const& rhs) const { return !(*this == rhs);}
    
  };

  template<class GRID, class INSIDE_PRED, class GT>
  inline
  vertex_on_cell_iterator<GRID, INSIDE_PRED, GT>
  cell_iterator<GRID, INSIDE_PRED, GT>::FirstVertex() const 
  { return vertex_on_cell_iterator<GRID,INSIDE_PRED, GT>(*this);}

} // namespace restricted_grid_view

template<class GRID, class INSIDE_PRED, class GT>
struct grid_types<restricted_grid_view::grid_view<GRID,INSIDE_PRED,GT> >
  : public grid_types_base<restricted_grid_view::grid_types_grid_view<GRID,INSIDE_PRED,GT> >
{};

template<class GRID, class INSIDE_PRED, class GT>
struct element_traits<restricted_grid_view::vertex<GRID,INSIDE_PRED,GT> >
  : public element_traits_vertex_base<restricted_grid_view::grid_view<GRID,INSIDE_PRED,GT> >
{
  typedef element_traits_vertex_base<restricted_grid_view::grid_view<GRID,INSIDE_PRED,GT> > base;
  typedef typename base::hasher_type_elem_base       hasher_type;
};


template<class GRID, class INSIDE_PRED, class GT, class T>
class grid_function<restricted_grid_view::vertex<GRID,INSIDE_PRED,GT>, T>
  : public grid_function_hash<restricted_grid_view::vertex<GRID,INSIDE_PRED,GT>, T>
{
  typedef grid_function_hash<restricted_grid_view::vertex<GRID,INSIDE_PRED,GT>, T>  base;
  typedef restricted_grid_view::grid_view<GRID,INSIDE_PRED,GT> grid_type;
public:
  grid_function() {}
  grid_function(grid_type const& g) : base(g) {}
  grid_function(grid_type const& g, T const& t) : base(g,t) {}
};

} // namespace GrAL 

#ifdef NMWR_INCLUDE_TEMPLATE_DEFS
#include "restricted-grid-view.C"
#endif

#endif