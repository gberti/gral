#ifndef GRAL_GB_VIEWS_RESTRICTED_GRID_VIEW_H
#define GRAL_GB_VIEWS_RESTRICTED_GRID_VIEW_H

// $LICENSE_NEC

#include "Gral/Base/common-grid-basics.h"
#include "Gral/Base/archetypes-from-base.h"
#include "Gral/Subranges/enumerated-element-range.h"
#include "Gral/Base/grid-function-hash.h"
#include "Gral/Iterators/closure-iterator.h"

#include "Gral/Base/wrap-elements.h"

namespace GrAL {


/*! \brief Provide a view on a grid part defined by a  cell predicate



   \todo implement Edge/Facet if supported by \c GRID
   \see \ref gridviews module
   \see Test in \ref test-restricted-components-view.C

*/

namespace restricted_grid_view { 
  /*! \defgroup restricted_grid_view_grp  Restricted Grid View
      \brief Provide a view on a grid part defined by a  cell predicate
  
      \ingroup gridviews
  */

  template<class GRID, class INSIDE_PRED, class GT = grid_types<GRID> >
  class grid_view;

  template<class GRID, class INSIDE_PRED, class GT> 
  class vertex;
  template<class GRID, class INSIDE_PRED, class GT> 
  class cell;

  template<class GRID, class INSIDE_PRED, class GT> 
  class vertex_iterator;
  template<class GRID, class INSIDE_PRED, class GT> 
  class cell_iterator;

  template<class GRID, class INSIDE_PRED, class GT> 
  class vertex_on_cell_iterator;
  template<class GRID, class INSIDE_PRED, class GT> 
  class cell_on_cell_iterator;
  template<class GRID, class INSIDE_PRED, class GT>
  class cell_on_vertex_iterator;


  template<class GRID, class INSIDE_PRED, class GT>
  struct grid_types_grid_view_base1 
    : public archetypes_from_base_grid_types<GT>, 
      public grid_types_detail::grid_types_root 
  {
    typedef grid_types_grid_view_base1<GRID, INSIDE_PRED,GT> self;
    typedef grid_view<GRID,INSIDE_PRED, GT>                  grid_type;

    typedef int test_type;
    typedef typename  GT::dimension_tag                   dimension_tag;
    typedef typename  GT::size_type                       size_type;
    typedef typename  GT::vertex_handle                   vertex_handle;
    typedef typename  GT::cell_handle                     cell_handle;

    typedef vertex         <GRID,INSIDE_PRED, GT>         Vertex;
    typedef cell           <GRID,INSIDE_PRED, GT>         Cell;
    typedef cell_iterator  <GRID,INSIDE_PRED, GT>         CellIterator;
    typedef vertex_iterator<GRID,INSIDE_PRED, GT>         VertexIterator;
  };


  template<class GRID, class INSIDE_PRED, class GT>
  struct grid_types_grid_view_base2  
    : public wrap_all_downward_incidence_iterator_types<grid_types_grid_view_base1<GRID,INSIDE_PRED, GT>, GT> 
  {};

  template<class GRID, class INSIDE_PRED, class GT>
  struct grid_types_grid_view_base : 
    public grid_types_grid_view_base2<GRID,INSIDE_PRED, GT> 
  {};


  

  template<class GRID, class INSIDE_PRED, class GT, unsigned HASCELLONCELL>
  struct grid_types_with_cell_on_cell_iterator {};

  template<class GRID, class INSIDE_PRED, class GT>
  struct grid_types_with_cell_on_cell_iterator<GRID, INSIDE_PRED, GT, 1>
  {
    typedef cell_on_cell_iterator<GRID, INSIDE_PRED, GT> CellOnCellIterator;
  };

  template<class GRID, class INSIDE_PRED, class GT, unsigned HASCELLONVERTEX>
  struct grid_types_with_cell_on_vertex_iterator {};

  template<class GRID, class INSIDE_PRED, class GT>
  struct grid_types_with_cell_on_vertex_iterator<GRID, INSIDE_PRED, GT, 1>
  {
    typedef cell_on_vertex_iterator<GRID, INSIDE_PRED, GT> CellOnVertexIterator;
  };


  template<class GRID, class INSIDE_PRED, class GT>
  struct grid_types_grid_view_all :
    public grid_types_grid_view_base<GRID, INSIDE_PRED, GT>,
    public grid_types_with_cell_on_cell_iterator  <GRID, INSIDE_PRED, GT, has_CellOnCellIterator  <GT>::result>,
    public grid_types_with_cell_on_vertex_iterator<GRID, INSIDE_PRED, GT, has_CellOnVertexIterator<GT>::result>
  {
    typedef grid_types_grid_view_base<GRID, INSIDE_PRED, GT>  base;
    typedef closure_iterator<typename base::CellIterator, typename base::EdgeOnCellIterator, base> EdgeIterator;

    static typename base::cell_handle invalid_cell_handle(typename base::grid_type const& g)
    {  return GT::invalid_cell_handle(g.BaseGrid()); }

  };

} // namespace restricted_grid_view





  namespace restricted_grid_view {

  //----- Mixin classes depending on presence of iterator types in base grid ----

  template<class GRID, class INSIDE_PRED, class GT, class VERTEX, unsigned HASCELLONVERTEX>
  struct vertex_mixin_cov_iterator { };
  template<class GRID, class INSIDE_PRED, class GT, class VERTEX>
  struct vertex_mixin_cov_iterator<GRID, INSIDE_PRED, GT, VERTEX, 1>
  { 
    cell_on_vertex_iterator<GRID, INSIDE_PRED, GT> FirstCell()  const;
    unsigned                                       NumOfCells() const;
  };


  template<class GRID, class INSIDE_PRED, class GT, class CELL, unsigned HASCELLONCELL>
  struct cell_mixin_coc_iterator { };
  template<class GRID, class INSIDE_PRED, class GT, class CELL>
  struct cell_mixin_coc_iterator<GRID, INSIDE_PRED, GT, CELL, 1>
  { 
    cell_on_cell_iterator<GRID, INSIDE_PRED, GT> FirstCell()  const;
    cell_on_cell_iterator<GRID, INSIDE_PRED, GT> EndCell()    const;
    unsigned                                     NumOfCells() const;
  };



  /*! \brief The restricted grid view class

    This class is a model of $GrAL Cell-VertexInputGridRange.
    Grid functions are defined for the vertex and cell types.
    The following types are defined in <code> grid_types<grid_view<GRID, ...> > </code>
    if they are defined in \c GRID or \c GT:
    - CellOnCellIterator
    - CellOnVertexIterator


    \templateparams
    - GRID:
      - Vertex/VertexIterator
      - Cell/CellIterator
      - VertexOnCellIterator
    - INSIDE_PRED
      - decide whether a cell is to be considered inside,
        <tt> bool operator()(Cell) </tt>
    - GT
      - \c grid_types specialization for GRID

    
      \ingroup restricted_grid_view_grp
      \see Module \ref restricted_grid_view_grp
      \see Tested in \ref test-restricted-grid-view.C 

      \todo Implement conditionally other incidence iterators, and sequence iterators
      for element types other than vertex/cell.
  */

  template<class GRID, class INSIDE_PRED, class GT>
  class grid_view : public archetypes_from_base<grid_view<GRID, INSIDE_PRED, GT>, GRID, GT>  {
  public:
    typedef grid_range_category category;
    typedef GRID        grid_type;
    typedef INSIDE_PRED pred_type;
    typedef GT          basegt;
    
    typedef typename  GT::Vertex               baseVertex;
    typedef typename  GT::Cell                 baseCell;
    typedef typename  GT::CellIterator         baseCellIterator;
    typedef typename  GT::VertexOnCellIterator baseVertexOnCellIterator;
    typedef typename  GT::vertex_handle        vertex_handle;
    typedef typename  GT::cell_handle          cell_handle;
    
    typedef enumerated_vertex_range<GRID>      range_type;
    typedef grid_types_grid_view_base1<GRID, INSIDE_PRED, GT> gt;
    typedef typename gt::size_type             size_type;
  private:
    ref_ptr<grid_type const>  g;
    ref_ptr<pred_type>        inside_p;
    
    mutable range_type        range;
    mutable bool              vertices_initialized;
    mutable size_type         num_of_cells;
  public:
    grid_view() {}
    grid_view(grid_type const& gg,
	      pred_type        ins)
    { init(const_ref_to_ref_ptr(gg), copy_to_ref_ptr(ins)); }

    grid_view(ref_ptr<grid_type const> gg,
	      ref_ptr<pred_type>       ins)
    { init(gg, ins);}

    void init(ref_ptr<grid_type const> gg,
	      ref_ptr<pred_type>       ins)
    {
      g = gg;
      inside_p = ins;
      range.set_grid(g);
      vertices_initialized = false;
      num_of_cells = -1;
    }

    unsigned dimension() const { return g->dimension();}
  public:
    typedef  vertex         <GRID,INSIDE_PRED, GT>  Vertex;
    typedef  vertex_iterator<GRID,INSIDE_PRED, GT>  VertexIterator;
    typedef  cell           <GRID,INSIDE_PRED, GT>  Cell;
    typedef  cell_iterator  <GRID,INSIDE_PRED, GT>  CellIterator;

    VertexIterator FirstVertex()   const { init_vertices(); return VertexIterator(*this);}
    VertexIterator EndVertex()     const { init_vertices(); return VertexIterator(*this, range.EndVertex());}
    size_type      NumOfVertices() const { init_vertices(); return range.NumOfVertices();}

    CellIterator   FirstCell()     const { return CellIterator(*this);}
    CellIterator   EndCell()       const { return CellIterator(*this, BaseGrid().EndCell());}
    size_type      NumOfCells()    const { if(num_of_cells < 0) init_num_cells(); return num_of_cells;}


    grid_type const& BaseGrid() const { return *g;}
    grid_type const& TheGrid()  const { return *g;}
    pred_type const& pred()     const { return *inside_p;} 
    bool             inside(baseCell const& c)   const { return (*inside_p)(c);}   

    bool valid_cell  (cell_handle   h) const { return BaseGrid().valid_cell(h) && inside(baseCell(BaseGrid(), h));}

    //  bool valid_vertex(vertex_handle h) const;
  private:
    void init_num_cells() const;
    void init_vertices()  const;

    friend class restricted_grid_view::vertex_iterator<GRID,INSIDE_PRED, GT>;
    friend class restricted_grid_view::vertex         <GRID,INSIDE_PRED, GT>;
    friend class restricted_grid_view::cell_iterator  <GRID,INSIDE_PRED, GT>;

  };





  //--- Vertex ---

  template<class GRID, class INSIDE_PRED, class GT = grid_types<GRID> >
  class vertex : public vertex_mixin_cov_iterator<GRID, INSIDE_PRED, GT, vertex<GRID, INSIDE_PRED, GT>, 
						  has_CellOnVertexIterator<GT>::result> 
  {
    typedef vertex<GRID,INSIDE_PRED, GT> self;
  public:
    typedef grid_view<GRID,INSIDE_PRED, GT>       grid_type;
    typedef typename grid_type::baseVertex        baseVertex;
    typedef typename grid_type::baseVertex        base_element_type;
    typedef typename grid_type::vertex_handle     vertex_handle;
    typedef vertex_type_tag                       element_type_tag;
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
    // There is no way to check whether vv is a vertex insided the view!
    // Perhaps vertex_handle = pair(base::cell_handle, int loc_v) is better?
    // but then, we cannot easily compare vertex_handles ...
    vertex(grid_type const& gg, vertex_handle vv) 
      : g(&gg)
      {
	g->init_vertices();
	v = baseVertex(g->BaseGrid(), vv);
      }
    vertex(grid_type const& gg, baseVertex vv) 
      : g(&gg)
      {
	g->init_vertices();
	v = vv;
      }

    operator baseVertex() const { return v;}
    baseVertex Base() const { return v;}
    vertex_handle handle() const { return v.handle();}
    grid_type const& TheGrid () const { return *g;} 
    grid_type const& BaseGrid() const { return *g;} 

    bool operator==(self const& rhs) const { return v == rhs.v;}
    bool operator!=(self const& rhs) const { return !(*this == rhs);}
  };

  //--- VertexIterator ---

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

    typedef grid_type anchor_type;
    typedef Vertex    value_type;
  private:
    grid_type   const*  g;
    rangeVertexIterator v;
  public:
    vertex_iterator() : g(0) {}
    vertex_iterator(grid_type const& gg)
      : g(&gg)
      {
	v = g->range.FirstVertex();
      }
    vertex_iterator(grid_type const& gg, rangeVertexIterator vv)
      : g(&gg), v(vv) 
      {
      }
   

    self& operator++() { cv(); ++v; return *this;}
    Vertex operator*() const { cv(); return Vertex(*g, handle());}
    operator baseVertex() const { cv(); return *v;}
    baseVertex Base() const { cv(); return *v;}
    bool  IsDone() const { cb(); return v.IsDone();}

    anchor_type const& TheAnchor() const { cb(); return *g;}
    grid_type   const& TheGrid  () const { cb(); return *g;} 
    grid_type   const& BaseGrid () const { cb(); return *g;} 

    vertex_handle handle() const { return v.handle();}

    bool operator==(self const& rhs) const { cb(); rhs.cb(); return v == rhs.v;}
    bool operator!=(self const& rhs) const { return !(*this == rhs);}

    static self begin(anchor_type const& gg) { return gg.FirstVertex();}
    static self end  (anchor_type const& gg) { return gg.EndVertex  ();}
    
    bool bound() const { return g != 0;}
    bool valid() const { return bound() && !IsDone();}
    void cb()    const { REQUIRE(bound(),"",1);}
    void cv()    const { REQUIRE(valid(),"", 1);}
  };



  //--- Cell ---
  template<class GRID, class INSIDE_PRED, class GT = grid_types<GRID> >
  class cell : public cell_mixin_coc_iterator<GRID, INSIDE_PRED, GT, cell<GRID, INSIDE_PRED, GT>, 
					      has_CellOnCellIterator<GT>::result>
  {
    typedef cell<GRID,INSIDE_PRED, GT> self;
    typedef grid_types_grid_view_base<GRID,INSIDE_PRED, GT> gt;
    typedef grid_types<typename gt::archetype_type> archgt;
  public:
    typedef cell_type_tag                       element_type_tag;

    typedef grid_view<GRID,INSIDE_PRED, GT>              grid_type;
    typedef grid_types<typename grid_type ::range_type>  rgt;
    typedef typename grid_type::baseCell        baseCell;
    typedef typename grid_type::baseCell        base_element_type;
    typedef typename grid_type::cell_handle     cell_handle;
    typedef typename grid_type::baseCellIterator baseCellIterator;
    //   typedef typename grid_type::VertexOnCellIterator VertexOnCellIterator;
 private:
    grid_type   const*  g;
    baseCell            c;
  public:
    cell() : g(0) {}
    cell(grid_type const& gg)
      : g(&gg) {} //  c(* g->BaseGrid().FirstCell()) {}
    cell(grid_type const& gg, baseCell cc)
      : g(&gg), c(cc) 
    { REQUIRE(g->inside(c), "Base cell not in component!", 1); }
    cell(grid_type const& gg, cell_handle h)
      : g(&gg), c(g->BaseGrid(),h) 
    { REQUIRE(g->inside(c), "Base cell not in component!", 1); }


    operator baseCell() const { return c;}
    baseCell  Base() const { return c;}
    grid_type const& TheGrid() const { return *g;} 
    cell_handle handle() const { return c.handle();}
    //unsigned NumOfVertices() const { return Base().NumOfVertices();}
    //vertex_on_cell_iterator<GRID,INSIDE_PRED, GT> FirstVertex() const;
    //vertex_on_cell_iterator<GRID,INSIDE_PRED, GT> EndVertex  () const;

    vertex<GRID,INSIDE_PRED, GT>  V(typename archgt::Vertex v) const { 
      return vertex<GRID,INSIDE_PRED, GT>(TheGrid(), Base().V(v)); 
    }
    
    bool operator==(self const& rhs) const { return c == rhs.c;}
    bool operator!=(self const& rhs) const { return !(*this == rhs);}

    bool bound() const { return g != 0;}
    bool valid() const { return bound() && c.valid();}
  };



  //--- CellIterator ---
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

    typedef grid_type                     anchor_type;
    typedef cell<GRID,INSIDE_PRED, GT>    value_type;
 private:
    ref_ptr<grid_type   const>  g;
    baseCellIterator    c;
  public:
    cell_iterator() : g(0) {}
    cell_iterator(grid_type const& gg) 
      : g(gg)
      {
	c = g->BaseGrid().FirstCell();
	advance_till_valid();
      }
    cell_iterator(grid_type const& gg, baseCellIterator cc) : g(gg), c(cc) {}

    self& operator++() { cv(); ++c; advance_till_valid();  return *this;}
    cell<GRID,INSIDE_PRED, GT>  operator*() const { cv(); return cell<GRID,INSIDE_PRED, GT>(*g,*c); }
    operator baseCell() const { cv(); return *c;}
    baseCell  Base() const { cv(); return *c;}
    bool  IsDone() const { cb(); return c.IsDone();}
 
    cell_handle handle() const { cv(); return c.handle();}
    //unsigned NumOfVertices() const { cv(); return Base().NumOfVertices();}
    //vertex_on_cell_iterator<GRID,INSIDE_PRED, GT> FirstVertex() const;
    //vertex_on_cell_iterator<GRID,INSIDE_PRED, GT> EndVertex()   const;

    anchor_type const& TheAnchor() const { cb(); return *g;}
    grid_type   const& TheGrid  () const { cb(); return *g;} 
    grid_type   const& BaseGrid () const { cb(); return *g;} 

    static self begin(anchor_type const& gg) { return gg.FirstCell();}
    static self end  (anchor_type const& gg) { return gg.EndCell  ();}
    
    bool operator==(self const& rhs) const { cb(); rhs.cb(); return c == rhs.c;}
    bool operator!=(self const& rhs) const { return !(*this == rhs);}

    bool bound() const { return g != 0;}
    bool valid() const { return bound() && !IsDone();}
    void cb()    const { REQUIRE(bound(),"",1);}
    void cv()    const { REQUIRE(valid(),"", 1);}
  private:
    void advance_till_valid() {
      while(! IsDone() && ! g->inside(*c))
	++c;
    }
  };


    /*
  //--- VertexOnCellIterator ---
  template<class GRID, class INSIDE_PRED, class GT = grid_types<GRID> >
  class vertex_on_cell_iterator {
    typedef vertex_on_cell_iterator<GRID,INSIDE_PRED, GT> self;
  public:
    typedef grid_view<GRID,INSIDE_PRED, GT>              grid_type;
    typedef typename grid_type::baseVertexOnCellIterator baseVertexOnCellIterator;
    typedef typename grid_type::vertex_handle            vertex_handle;
    typedef typename grid_type::Cell                     Cell;
    typedef typename grid_type::Vertex                   Vertex;

    typedef Cell   anchor_type;
    typedef Vertex value_type;
  private:
    typedef grid_types_grid_view_base<GRID,INSIDE_PRED, GT> gt;
    typedef grid_types<typename gt::archetype_type> archgt;
    typedef typename archgt::vertex_handle          arch_vertex_handle;
  private:
    Cell c;
    baseVertexOnCellIterator vc;
  public:
    vertex_on_cell_iterator()  {}
    vertex_on_cell_iterator(Cell const& cc) : c(cc), vc(c.Base()) {}
    vertex_on_cell_iterator(Cell const& cc, baseVertexOnCellIterator vcc) : c(cc), vc(vcc) {}

    self& operator++() { ++vc; return *this;}
    Vertex operator*() const { return Vertex(TheGrid(),handle());}
    bool  IsDone() const { return vc.IsDone();}

    vertex_handle handle() const { return vc.handle();}
    arch_vertex_handle local_handle() const { return vc.local_handle();}

    grid_type   const& TheGrid()   const { return c.TheGrid();} 
    anchor_type const& TheAnchor() const { return c;}
    anchor_type const& TheCell()   const { return c;}

    bool operator==(self const& rhs) const { return c == rhs.c && vc == rhs.vc;}
    bool operator!=(self const& rhs) const { return !(*this == rhs);}
    
  };


  template<class GRID, class INSIDE_PRED, class GT>
  inline
  vertex_on_cell_iterator<GRID, INSIDE_PRED, GT>
  cell<GRID, INSIDE_PRED, GT>::FirstVertex() const 
  { return vertex_on_cell_iterator<GRID,INSIDE_PRED, GT>(*this);}

  template<class GRID, class INSIDE_PRED, class GT>
  inline
  vertex_on_cell_iterator<GRID, INSIDE_PRED, GT>
  cell<GRID, INSIDE_PRED, GT>::EndVertex() const 
  { return vertex_on_cell_iterator<GRID,INSIDE_PRED, GT>(*this, Base().EndVertex());}

  */

  //------- CellOnCellIterator ---------

  template<class GRID, class INSIDE_PRED, class GT = grid_types<GRID> >
  class cell_on_cell_iterator {
    typedef cell_on_cell_iterator<GRID,INSIDE_PRED, GT>  self;
  public:
    typedef grid_view<GRID,INSIDE_PRED, GT>              grid_type;
    typedef typename GT::CellOnCellIterator              baseCellOnCellIterator;
    typedef typename grid_type::cell_handle              cell_handle;
    typedef typename grid_type::Cell                     Cell;
    typedef Cell value_type;
    typedef Cell anchor_type;
  private:
    Cell c;
    baseCellOnCellIterator cc;
  public:
    cell_on_cell_iterator()  {}
    cell_on_cell_iterator(Cell const& c1) : c(c1), cc(c1.Base()) { advance_till_valid();}
    cell_on_cell_iterator(Cell const& c1, baseCellOnCellIterator cc1) : c(c1), cc(cc1) { advance_till_valid();}

    self& operator++() { ++cc; advance_till_valid(); return *this;}
    Cell  operator*() const { return Cell(TheGrid(),*cc);}
    bool  IsDone() const { return cc.IsDone();}
    cell_handle handle() const { return cc.handle();}

    grid_type const& TheGrid  () const { return c.TheGrid();} 
    Cell      const& TheCell  () const { return c;}
    Cell      const& TheAnchor() const { return c;}

    bool operator==(self const& rhs) const { return c == rhs.c && cc == rhs.cc;}
    bool operator!=(self const& rhs) const { return !(*this == rhs);}
    
  private:
    void advance_till_valid() {
      while(!cc.IsDone() && ! TheGrid().inside(*cc))
	++cc;
    }
  };



  // Mixins for cell corresponding to cell_on_cell_iterator

  template<class GRID, class INSIDE_PRED, class GT, class CELL>
  cell_on_cell_iterator<GRID, INSIDE_PRED, GT> 
  cell_mixin_coc_iterator<GRID, INSIDE_PRED, GT, CELL, 1>::FirstCell() const 
  { return cell_on_cell_iterator<GRID, INSIDE_PRED, GT>(static_cast<CELL const&>(*this)); }

  template<class GRID, class INSIDE_PRED, class GT, class CELL>
  cell_on_cell_iterator<GRID, INSIDE_PRED, GT> 
  cell_mixin_coc_iterator<GRID, INSIDE_PRED, GT, CELL, 1>::EndCell() const 
  { return cell_on_cell_iterator<GRID, INSIDE_PRED, GT>(static_cast<CELL const&>(*this), 
							static_cast<CELL const&>(*this).Base().EndCell()); }


  template<class GRID, class INSIDE_PRED, class GT, class CELL>
  unsigned
  cell_mixin_coc_iterator<GRID, INSIDE_PRED, GT, CELL, 1>::NumOfCells() const 
  { 
    unsigned n = 0;
    cell_on_cell_iterator<GRID, INSIDE_PRED, GT> cc = FirstCell();
    while(! cc.IsDone()) {
      ++cc;
      ++n;
    }
    return n;
  }


  //--------------- CellOnVertexIterator ----------------

  template<class GRID, class INSIDE_PRED, class GT>
  class cell_on_vertex_iterator {
    typedef cell_on_vertex_iterator<GRID,INSIDE_PRED, GT>  self;
  public:
    typedef grid_view<GRID,INSIDE_PRED, GT>              grid_type;
    typedef typename GT::CellOnVertexIterator            baseCellOnVertexIterator;
    typedef typename grid_type::cell_handle              cell_handle;
    typedef typename grid_type::Cell                     Cell;
    typedef typename grid_type::Vertex                   Vertex;
  private:
    Vertex                   v;
    baseCellOnVertexIterator cv;
  public:
    cell_on_vertex_iterator()  {}
    cell_on_vertex_iterator(Vertex const& v1) : v(v1), cv(v1.Base()) {}

    self& operator++() { ++cv; advance_till_valid(); return *this;}
    Cell  operator*() const { return Cell(TheGrid(),*cv);}
    bool  IsDone() const { return cv.IsDone();}
    grid_type const& TheGrid() const { return v.TheGrid();} 
    cell_handle handle() const { return cv.handle();}

    bool operator==(self const& rhs) const { return v == rhs.v && cv == rhs.cv;}
    bool operator!=(self const& rhs) const { return !(*this == rhs);}
    
  private:
    void advance_till_valid() {
      while(!cv.IsDone() && ! TheGrid().inside(*cv))
	++cv;
    }
  };

  // Inline functions of mixin for vertex corresponding to cell_on_vertex_iterator
  template<class GRID, class INSIDE_PRED, class GT, class VERTEX>
  cell_on_vertex_iterator<GRID, INSIDE_PRED, GT> 
  vertex_mixin_cov_iterator<GRID, INSIDE_PRED, GT, VERTEX, 1>::FirstCell() const 
  { return cell_on_vertex_iterator<GRID, INSIDE_PRED, GT>(static_cast<VERTEX const&>(*this)); }

  template<class GRID, class INSIDE_PRED, class GT, class VERTEX>
  unsigned
  vertex_mixin_cov_iterator<GRID, INSIDE_PRED, GT, VERTEX, 1>::NumOfCells() const 
  { 
    unsigned n = 0;
    cell_on_vertex_iterator<GRID, INSIDE_PRED, GT> cv = FirstCell();
    while(! cv.IsDone()) {
      ++cv;
      ++n;
    }
    return n;
  }


  //---------- assemble complete grid_types ---------------

  /*
  template<class GRID, class INSIDE_PRED, class GT>
  struct grid_types_grid_view_all :
    public grid_types_grid_view_base<GRID, INSIDE_PRED, GT>,
    public grid_types_with_cell_on_cell_iterator  <GRID, INSIDE_PRED, GT, has_CellOnCellIterator  <GT>::result>,
    public grid_types_with_cell_on_vertex_iterator<GRID, INSIDE_PRED, GT, has_CellOnVertexIterator<GT>::result>
  {
    typedef grid_types_grid_view_base<GRID, INSIDE_PRED, GT>  base;
    typedef closure_iterator<typename base::CellIterator, typename base::EdgeOnCellIterator> EdgeIterator;

    static typename base::cell_handle invalid_cell_handle(typename base::grid_type const& g) 
    { return GT::invalid_cell_handle(g.BaseGrid());}

  };

  */
  
} // namespace restricted_grid_view


  /*! \brief Specialization of grid_types template
       \ingroup restricted_grid_view_grp
       \see Module \ref restricted_grid_view_grp

  */ 


template<class GRID, class INSIDE_PRED, class GT>
struct grid_types<restricted_grid_view::grid_view<GRID,INSIDE_PRED,GT> >
  : public grid_types_base<restricted_grid_view::grid_types_grid_view_all<GRID,INSIDE_PRED,GT> >
{};


template<class GRID, class INSIDE_PRED, class GT>
struct element_traits<restricted_grid_view::vertex<GRID,INSIDE_PRED,GT> >
  : public element_traits_vertex_base<restricted_grid_view::grid_view<GRID,INSIDE_PRED,GT> >
{
  typedef element_traits_vertex_base<restricted_grid_view::grid_view<GRID,INSIDE_PRED,GT> > base;
  typedef typename base::hasher_type_elem_base       hasher_type;
};


template<class GRID, class INSIDE_PRED, class GT>
struct element_traits<restricted_grid_view::cell<GRID,INSIDE_PRED,GT> >
  : public element_traits_cell_base<restricted_grid_view::grid_view<GRID,INSIDE_PRED,GT> >
{
  typedef element_traits_cell_base<restricted_grid_view::grid_view<GRID,INSIDE_PRED,GT> > base;
  typedef typename base::hasher_type_elem_base       hasher_type;
};





namespace restricted_grid_view {


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
  cell_on_cell_iterator<GRID,INSIDE_PRED, GT> 
  gral_begin(cell<GRID,INSIDE_PRED, GT> const& c, cell_on_cell_iterator<GRID,INSIDE_PRED, GT>)
  { return c.FirstCell();}

  template<class GRID, class INSIDE_PRED, class GT>
  cell_on_cell_iterator<GRID,INSIDE_PRED, GT> 
  gral_end  (cell<GRID,INSIDE_PRED, GT> const& c, cell_on_cell_iterator<GRID,INSIDE_PRED, GT>)
  { return c.EndCell();}

  template<class GRID, class INSIDE_PRED, class GT>
  typename grid_types<grid_view<GRID,INSIDE_PRED, GT> >::size_type
  gral_size  (cell<GRID,INSIDE_PRED, GT> const& c, cell_on_cell_iterator<GRID,INSIDE_PRED, GT>)
  { return c.NumOfCells();}
 
  
  


} // namespace restricted_grid_view {


  /*! \brief Vertex grid function specialization 
 
       This models $GrAL TotalGridFunction on restricted_grid_view::grid_view
       and uses hash maps.

      \relates restricted_grid_view_grp
      \ingroup  restricted_grid_view_grp
      \see \ref restricted_grid_view_grp
  */
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

  /*! \brief Cell grid function specialization 
 
       This models $GrAL TotalGridFunction  on restricted_grid_view::grid_view
       and uses hash maps.

      \relates restricted_grid_view_grp
      \ingroup  restricted_grid_view_grp
      \see \ref restricted_grid_view_grp
  */
template<class GRID, class INSIDE_PRED, class GT, class T>
class grid_function<restricted_grid_view::cell<GRID,INSIDE_PRED,GT>, T>
  : public grid_function_hash<restricted_grid_view::cell<GRID,INSIDE_PRED,GT>, T>
{
  typedef grid_function_hash<restricted_grid_view::cell<GRID,INSIDE_PRED,GT>, T>  base;
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
