#ifndef GRAL_GB_VIEWS_RESTRICTED_GRID_VIEW_H
#define GRAL_GB_VIEWS_RESTRICTED_GRID_VIEW_H


/* ------------------------------------------------------------

    Copyright (C) 2001 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include "Gral/Base/common-grid-basics.h"
#include "Gral/Base/archetypes-from-base.h"
#include "Gral/Base/grid-function-hash.h"
#include "Gral/Base/wrap-elements.h"

#include "Gral/Subranges/enumerated-element-range.h"
#include "Gral/Iterators/closure-iterator.h"

// #include "Gral/Views/grid-function-view.h"

#include <boost/mpl/if.hpp>

namespace GrAL {


/*! \brief Provide a view on a grid part defined by a  cell predicate



   \todo implement Edge/Facet if supported by the base grid
   \see \ref gridviews module
   \see Test in \ref test-restricted-components-view.C

*/

namespace restricted_grid_view { 
  /*! \defgroup restricted_grid_view_grp  Restricted Grid View
      \brief Provide a view on a grid part defined by a  cell predicate
  
      \ingroup gridviews
  */

  template<class GT, class PRED> struct cfg { 
    typedef GT                     gt;
    typedef typename gt::grid_type grid_type;
    //    typedef typename PRED::template apply<grid_type>::type pred_type;
    typedef PRED pred_type;
  };

  template<class CFG>  class grid_view;

  template<class CFG>  class vertex;
  template<class CFG> 
  class cell;

  template<class CFG> 
  class vertex_iterator;
  template<class CFG> 
  class cell_iterator;

  template<class CFG> 
  class vertex_on_cell_iterator;
  template<class CFG> 
  class cell_on_cell_iterator;
  template<class CFG>
  class cell_on_vertex_iterator;


  template<class CFG>
  struct grid_types_grid_view_base1 
    : public archetypes_from_base_grid_types<typename CFG::gt>, 
      public grid_types_detail::grid_types_root 
  {
    typedef grid_types_grid_view_base1<CFG> self;
    typedef grid_view<CFG>                  grid_type;

    typedef typename  CFG::gt                             bgt;
    typedef typename  bgt::dimension_tag                  dimension_tag;
    typedef typename  bgt::size_type                      size_type;
    typedef typename  bgt::vertex_handle                  vertex_handle;
    typedef typename  bgt::cell_handle                    cell_handle;

    typedef vertex         <CFG>         Vertex;
    typedef cell           <CFG>         Cell;
    typedef cell_iterator  <CFG>         CellIterator;
    typedef vertex_iterator<CFG>         VertexIterator;
 
    typedef vertex_on_cell_iterator<CFG> VertexOnCellIterator;
  };


  template<class CFG>
  struct grid_types_grid_view_base2  
    : public wrap_all_downward_incidence_iterator_types<grid_types_grid_view_base1<CFG>, typename CFG::gt> 
  {};

  template<class CFG>
  struct grid_types_grid_view_base : 
    public grid_types_grid_view_base2<CFG> 
  {};



  template<class CFG>
  struct grid_types_grid_view_all :
    public grid_types_grid_view_base<CFG>
  {
    typedef grid_types_grid_view_base<CFG> base;

    typedef typename boost::mpl::if_c<has_CellOnCellIterator<typename CFG::gt>::value,
				      cell_on_cell_iterator<CFG>,
				      typename incidence_iterator_cd<base,0,0>::type>::type
    CellOnCellIterator;
    typedef typename boost::mpl::if_c<has_CellOnVertexIterator<typename CFG::gt>::value,
	   			      cell_on_vertex_iterator<CFG>,
				      typename incidence_iterator_cd_d<base,0,0>::type>::type
    CellOnVertexIterator;
  
  typedef closure_iterator<typename base::CellIterator, 
			   typename base::EdgeOnCellIterator, base> EdgeIterator;

    static typename base::cell_handle invalid_cell_handle(typename base::grid_type const& g)
    {  return CFG::gt::invalid_cell_handle(g.BaseGrid()); }

};
  
} // namespace restricted_grid_view


  /*! \brief Specialization of grid_types template
       \ingroup restricted_grid_view_grp
       \see Module \ref restricted_grid_view_grp

  */ 


template<class CFG>
struct grid_types<restricted_grid_view::grid_view<CFG> >
  : public grid_types_base<restricted_grid_view::grid_types_grid_view_all<CFG> >
{};


template<class CFG>
struct element_traits<restricted_grid_view::vertex<CFG> >
  : public element_traits_vertex_base<restricted_grid_view::grid_view<CFG> >
{
  typedef element_traits_vertex_base<restricted_grid_view::grid_view<CFG> > base;
  //  typedef typename base::hasher_type_elem_base       hasher_type;

  struct hasher_type : public base::hasher_type_elem_base {
    typedef typename base::element_type             Vertex; 
    typedef element_traits<typename Vertex::baseVertex> bet;
    typedef typename bet::hasher_type               base_hasher_type;
  
    typename base_hasher_type::result_type operator()(Vertex const& v) const {
      base_hasher_type bh;
      return bh(v.Base());
    }
  };
};


template<class CFG>
struct element_traits<restricted_grid_view::cell<CFG> >
  : public element_traits_cell_base<restricted_grid_view::grid_view<CFG> >
{
  typedef element_traits_cell_base<restricted_grid_view::grid_view<CFG> > base;
  // typedef typename base::hasher_type_elem_base       hasher_type;

  struct hasher_type : public base::hasher_type_elem_base {
    typedef typename base::element_type             Cell; 
    typedef element_traits<typename Cell::baseCell> bet;
    typedef typename bet::hasher_type               base_hasher_type;
  
    typename base_hasher_type::result_type operator()(Cell const& c) const {
      base_hasher_type bh;
      return bh(c.Base());
    }
  };
};







  namespace restricted_grid_view {

  //----- Mixin classes depending on presence of iterator types in base grid ----

  template<class CFG, class VERTEX, unsigned HASCELLONVERTEX>
  struct vertex_mixin_cov_iterator { };
  template<class CFG, class VERTEX>
  struct vertex_mixin_cov_iterator<CFG, VERTEX, 1>
  { 
    cell_on_vertex_iterator<CFG> FirstCell()  const;
    unsigned                     NumOfCells() const;
  };


  template<class CFG, class CELL, unsigned HASCELLONCELL>
  struct cell_mixin_coc_iterator { };
  template<class CFG, class CELL>
  struct cell_mixin_coc_iterator<CFG, CELL, 1>
  { 
    cell_on_cell_iterator<CFG> FirstCell()  const;
    cell_on_cell_iterator<CFG> EndCell()    const;
    unsigned                   NumOfCells() const;
  };



  /*! \brief The restricted grid view class

    This class is a model of $GrAL Cell-VertexInputGridRange.
    Grid functions are defined for the vertex and cell types.
    The following types are defined in <code> grid_types<grid_view<GRID, ...> > </code>
    if they are defined in \c CFG::gt 
    - CellOnCellIterator
    - CellOnVertexIterator


    \templateparams
    - CFG:
      - typedef \c grid_type
      - typedef \c pred_type
        - decide whether a cell is to be considered inside,
          <tt> bool operator()(Cell) </tt>
      - typedef \c gt
      - \c grid_types specialization for \c grid_type
        - Vertex/VertexIterator
        - Cell/CellIterator
        - VertexOnCellIterator

    
      \ingroup restricted_grid_view_grp
      \see Module \ref restricted_grid_view_grp
      \see Tested in \ref test-restricted-grid-view.C 

      \todo Implement conditionally other incidence iterators, and sequence iterators
      for element types other than vertex/cell.
  */

  template<class CFG>
  class grid_view : public archetypes_from_base<grid_view<CFG>, typename CFG::grid_type, typename CFG::gt>  {
    typedef grid_view<CFG> self;
  public:
    typedef self                     grid_type;
    typedef typename CFG::grid_type  base_grid_type;
    typedef typename CFG::pred_type  pred_type;
    typedef typename CFG::gt         basegt;
    
    typedef typename  basegt::Vertex               baseVertex;
    typedef typename  basegt::Cell                 baseCell;
    typedef typename  basegt::CellIterator         baseCellIterator;
    typedef typename  basegt::VertexOnCellIterator baseVertexOnCellIterator;
    typedef typename  basegt::vertex_handle        vertex_handle;
    typedef typename  basegt::cell_handle          cell_handle;
    
    typedef enumerated_vertex_range<base_grid_type>      range_type;
    typedef grid_types_grid_view_base1<CFG> gt;

    typedef typename gt::size_type             size_type;
    enum { dim = gt::dimension_tag::dim }; 
    typedef grid_view_category_d<dim> category;

  private:
    ref_ptr<base_grid_type const>  g;
    ref_ptr<pred_type>             inside_p;
    
    mutable range_type        range;
    mutable bool              vertices_initialized;
    mutable size_type         num_of_cells;
  public:
    grid_view() {}
    grid_view(base_grid_type const& gg,
	      pred_type        ins)
    { init(const_ref_to_ref_ptr(gg), copy_to_ref_ptr(ins)); }

    grid_view(ref_ptr<base_grid_type const> gg,
	      ref_ptr<pred_type>       ins)
    { init(gg, ins);}

    void init(ref_ptr<base_grid_type const> gg,
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
    typedef  vertex         <CFG>  Vertex;
    typedef  vertex_iterator<CFG>  VertexIterator;
    typedef  cell           <CFG>  Cell;
    typedef  cell_iterator  <CFG>  CellIterator;

    VertexIterator FirstVertex()   const { init_vertices(); return VertexIterator(*this);}
    VertexIterator EndVertex()     const { init_vertices(); return VertexIterator(*this, range.EndVertex());}
    size_type      NumOfVertices() const { init_vertices(); return range.NumOfVertices();}

    CellIterator   FirstCell()     const { return CellIterator(*this);}
    CellIterator   EndCell()       const { return CellIterator(*this, BaseGrid().EndCell());}
    size_type      NumOfCells()    const { if(num_of_cells < 0) init_num_cells(); return num_of_cells;}


    base_grid_type const& BaseGrid() const { return *g;}
    // FIXME: This is necessary when passing restricted_grid_view as subrange to incidence_hull
    base_grid_type const& TheGrid()  const { return *g;}

    pred_type const& pred()     const { return *inside_p;} 
    bool             inside(baseCell const& c)   const { return (*inside_p)(c);}   

    bool valid_cell  (cell_handle   h) const { return BaseGrid().valid_cell(h) && inside(baseCell(BaseGrid(), h));}

    //  bool valid_vertex(vertex_handle h) const;
  private:
    void init_num_cells() const;
    void init_vertices()  const;

    friend class restricted_grid_view::vertex_iterator<CFG>;
    friend class restricted_grid_view::vertex         <CFG>;
    friend class restricted_grid_view::cell_iterator  <CFG>;

  };





  //--- Vertex ---

  template<class CFG>
  class vertex : public vertex_mixin_cov_iterator<CFG, vertex<CFG>, 
						  has_CellOnVertexIterator<typename CFG::gt>::result> 
  {
    typedef vertex<CFG> self;
  public:
    typedef grid_view<CFG>       grid_type;
    typedef typename grid_type::baseVertex        baseVertex;
    typedef typename grid_type::baseVertex        base_element_type;
    typedef typename grid_type::vertex_handle     vertex_handle;

    typedef vertex_type_tag                       element_type_tag;
    typedef grid_vertex_category                  category;
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
    bool operator< (self const& rhs) const { return v <  rhs.v;}
  };

  //--- VertexIterator ---

  template<class CFG>
  class vertex_iterator {
    typedef vertex_iterator<CFG> self;
  public:
    typedef grid_vertex_iterator_category         category;
    typedef grid_view<CFG>                        grid_type;
    typedef typename grid_type::range_type        range_type;
    typedef typename range_type::VertexIterator   rangeVertexIterator; 
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
    bool operator< (self const& rhs) const { cb(); rhs.cb(); return v <  rhs.v;}

    static self begin(anchor_type const& gg) { return gg.FirstVertex();}
    static self end  (anchor_type const& gg) { return gg.EndVertex  ();}
    
    bool bound() const { return g != 0;}
    bool valid() const { return bound() && !IsDone();}
    void cb()    const { REQUIRE(bound(),"",1);}
    void cv()    const { REQUIRE(valid(),"", 1);}
  };



  //--- Cell ---
  template<class CFG>
  class cell : public cell_mixin_coc_iterator<CFG, cell<CFG>, 
					      has_CellOnCellIterator<typename CFG::gt>::result>
  {
    typedef cell<CFG> self;
    typedef grid_types_grid_view_base<CFG> gt;
    typedef grid_types<typename gt::archetype_type> archgt;
  public:
    typedef cell_type_tag                       element_type_tag;
    typedef grid_cell_category                  category;

    typedef grid_view<CFG>              grid_type;
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
    int  NumOfVertices() const { return Base().NumOfVertices();}
    vertex_on_cell_iterator<CFG> FirstVertex() const;
    vertex_on_cell_iterator<CFG> EndVertex  () const;

    vertex<CFG>  V(typename archgt::Vertex v) const { 
      return vertex<CFG>(TheGrid(), Base().V(v)); 
    }
    
    bool operator==(self const& rhs) const { cb(); rhs.cb(); return c == rhs.c;}
    bool operator!=(self const& rhs) const { return !(*this == rhs);}
    bool operator< (self const& rhs) const { cb(); rhs.cb(); return c <  rhs.c;}

    bool bound() const { return g != 0;}
    bool valid() const { return bound() && c.valid();}
    void cb()    const { REQUIRE(bound(),"",1);}
    void cv()    const { REQUIRE(valid(),"", 1);}
  };



  //--- CellIterator ---
  template<class CFG>
  class cell_iterator {
    typedef cell_iterator<CFG> self;
  public:
    typedef grid_cell_iterator_category                  category;
    typedef grid_view<CFG>                               grid_type;
    typedef grid_types<typename grid_type ::range_type>  rgt;
    typedef typename grid_type::baseCell        baseCell;
    typedef typename grid_type::cell_handle     cell_handle;
    typedef typename grid_type::baseCellIterator baseCellIterator;
    //   typedef typename grid_type::VertexOnCellIterator VertexOnCellIterator;

    typedef grid_type                     anchor_type;
    typedef cell<CFG>    value_type;
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
    cell<CFG>  operator*() const { cv(); return cell<CFG>(*g,*c); }
    operator baseCell() const { cv(); return *c;}
    baseCell  Base() const { cv(); return *c;}
    bool  IsDone() const { cb(); return c.IsDone();}
 
    cell_handle handle() const { cv(); return c.handle();}
    //unsigned NumOfVertices() const { cv(); return Base().NumOfVertices();}
    //vertex_on_cell_iterator<CFG> FirstVertex() const;
    //vertex_on_cell_iterator<CFG> EndVertex()   const;

    anchor_type const& TheAnchor() const { cb(); return *g;}
    grid_type   const& TheGrid  () const { cb(); return *g;} 
    grid_type   const& BaseGrid () const { cb(); return *g;} 

    static self begin(anchor_type const& gg) { return gg.FirstCell();}
    static self end  (anchor_type const& gg) { return gg.EndCell  ();}
    
    bool operator==(self const& rhs) const { cb(); rhs.cb(); return c == rhs.c;}
    bool operator!=(self const& rhs) const { return !(*this == rhs);}
    bool operator< (self const& rhs) const { cb(); rhs.cb(); return c <  rhs.c;}

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


  //--- VertexOnCellIterator ---
  template<class CFG>
  class vertex_on_cell_iterator {
    typedef vertex_on_cell_iterator<CFG> self;
  public:
    typedef grid_view<CFG>              grid_type;
    typedef typename grid_type::baseVertexOnCellIterator baseVertexOnCellIterator;
    typedef typename grid_type::vertex_handle            vertex_handle;
    typedef typename grid_type::Cell                     Cell;
    typedef typename grid_type::Vertex                   Vertex;

    typedef grid_incidence_iterator_category_d<0,0>      category;
    typedef Cell   anchor_type;
    typedef Vertex value_type;
  private:
    typedef grid_types_grid_view_base<CFG> gt;
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


  template<class CFG>
  inline
  vertex_on_cell_iterator<CFG>
  cell<CFG>::FirstVertex() const 
  { return vertex_on_cell_iterator<CFG>(*this);}

  template<class CFG>
  inline
  vertex_on_cell_iterator<CFG>
  cell<CFG>::EndVertex() const 
  { return vertex_on_cell_iterator<CFG>(*this, Base().EndVertex());}

 

  //------- CellOnCellIterator ---------

  template<class CFG>
  class cell_on_cell_iterator {
    typedef cell_on_cell_iterator<CFG>  self;
  public:

    typedef grid_view<CFG>                               grid_type;
    typedef typename CFG::gt                             bgt;
    typedef typename bgt::CellOnCellIterator             baseCellOnCellIterator;
    typedef grid_types<grid_type>                        gt;
    typedef typename grid_type::cell_handle              cell_handle;
    typedef typename grid_type::Cell                     Cell;

    typedef grid_incidence_iterator_category_cd<0,0>     category;
    typedef Cell                                         value_type;
    typedef Cell                                         anchor_type;
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
    typename gt::Facet TheFacet() const { return typename gt::Facet(TheGrid(), cc.TheFacet());}

    bool operator==(self const& rhs) const { return c == rhs.c && cc == rhs.cc;}
    bool operator!=(self const& rhs) const { return !(*this == rhs);}
    
  private:
    void advance_till_valid() {
      while(!cc.IsDone() && ! TheGrid().inside(*cc))
	++cc;
    }
  };



  // Mixins for cell corresponding to cell_on_cell_iterator

  template<class CFG, class CELL>
  cell_on_cell_iterator<CFG> 
  cell_mixin_coc_iterator<CFG, CELL, 1>::FirstCell() const 
  { return cell_on_cell_iterator<CFG>(static_cast<CELL const&>(*this)); }

  template<class CFG, class CELL>
  cell_on_cell_iterator<CFG> 
  cell_mixin_coc_iterator<CFG, CELL, 1>::EndCell() const 
  { return cell_on_cell_iterator<CFG>(static_cast<CELL const&>(*this), 
							static_cast<CELL const&>(*this).Base().EndCell()); }


  template<class CFG, class CELL>
  unsigned
  cell_mixin_coc_iterator<CFG, CELL, 1>::NumOfCells() const 
  { 
    unsigned n = 0;
    cell_on_cell_iterator<CFG> cc = FirstCell();
    while(! cc.IsDone()) {
      ++cc;
      ++n;
    }
    return n;
  }


  //--------------- CellOnVertexIterator ----------------

  template<class CFG>
  class cell_on_vertex_iterator {
    typedef cell_on_vertex_iterator<CFG>  self;
  public:
    typedef grid_view<CFG>                      grid_type;
    typedef typename CFG::gt                    bgt;
    typedef typename bgt::CellOnVertexIterator           baseCellOnVertexIterator;
    typedef typename grid_type::cell_handle              cell_handle;
    typedef typename grid_type::Cell                     Cell;
    typedef typename grid_type::Vertex                   Vertex;

    typedef grid_incidence_iterator_category_d<grid_type::dim,0> category;    
    typedef Cell                                                 value_type;
    typedef Vertex                                               anchor_type;
  private:
    Vertex                   v;
    baseCellOnVertexIterator cv;
  public:
    cell_on_vertex_iterator()  {}
    cell_on_vertex_iterator(Vertex const& v1) : v(v1), cv(v1.Base()) {}

    self& operator++() { ++cv; advance_till_valid(); return *this;}
    Cell  operator*() const { return Cell(TheGrid(),*cv);}
    bool  IsDone() const { return cv.IsDone();}
    grid_type   const& TheGrid()   const { return v.TheGrid();} 
    anchor_type const& TheAnchor() const { return v;}

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
  template<class CFG, class VERTEX>
  cell_on_vertex_iterator<CFG> 
  vertex_mixin_cov_iterator<CFG, VERTEX, 1>::FirstCell() const 
  { return cell_on_vertex_iterator<CFG>(static_cast<VERTEX const&>(*this)); }

  template<class CFG, class VERTEX>
  unsigned
  vertex_mixin_cov_iterator<CFG, VERTEX, 1>::NumOfCells() const 
  { 
    unsigned n = 0;
    cell_on_vertex_iterator<CFG> cv = FirstCell();
    while(! cv.IsDone()) {
      ++cv;
      ++n;
    }
    return n;
  }



  template<class CFG>
  vertex_iterator<CFG> 
  gral_begin(grid_view<CFG> const& g, vertex_iterator<CFG>)
  { return g.FirstVertex();}

  template<class CFG>
  vertex_iterator<CFG> 
  gral_end(grid_view<CFG> const& g,   vertex_iterator<CFG>)
  { return g.EndVertex();}

  template<class CFG>
  typename grid_types<grid_view<CFG> >::size_type
  gral_size(grid_view<CFG> const& g, vertex_iterator<CFG>)
  { return g.NumOfVertices();}



  template<class CFG>
  cell_iterator<CFG> 
  gral_begin(grid_view<CFG> const& g, cell_iterator<CFG>)
  { return g.FirstCell();}

  template<class CFG>
  cell_iterator<CFG> 
  gral_end(grid_view<CFG> const& g,   cell_iterator<CFG>)
  { return g.EndCell();}

  template<class CFG>
  typename grid_types<grid_view<CFG> >::size_type
  gral_size(grid_view<CFG> const& g, cell_iterator<CFG>)
  { return g.NumOfCells();}

 
  template<class CFG>
  vertex_on_cell_iterator<CFG> 
  gral_begin(cell<CFG> const& c, vertex_on_cell_iterator<CFG>)
  { return c.FirstVertex();}

  template<class CFG>
  vertex_on_cell_iterator<CFG> 
  gral_end  (cell<CFG> const& c, vertex_on_cell_iterator<CFG>)
  { return c.EndVertex();}

  template<class CFG>
  typename grid_types<grid_view<CFG> >::size_type
  gral_size  (cell<CFG> const& c, vertex_on_cell_iterator<CFG>)
  { return c.NumOfVertices();}
 
   
  template<class CFG>
  cell_on_cell_iterator<CFG> 
  gral_begin(cell<CFG> const& c, cell_on_cell_iterator<CFG>)
  { return c.FirstCell();}

  template<class CFG>
  cell_on_cell_iterator<CFG> 
  gral_end  (cell<CFG> const& c, cell_on_cell_iterator<CFG>)
  { return c.EndCell();}

  template<class CFG>
  typename grid_types<grid_view<CFG> >::size_type
  gral_size  (cell<CFG> const& c, cell_on_cell_iterator<CFG>)
  { return c.NumOfCells();}
 
  
  


} // namespace restricted_grid_view {


  /*! \brief Vertex grid function specialization 
 
       This models $GrAL TotalGridFunction on restricted_grid_view::grid_view
       and uses hash maps.

      \relates restricted_grid_view_grp
      \ingroup  restricted_grid_view_grp
      \see \ref restricted_grid_view_grp
  */
template<class CFG, class T>
class grid_function<restricted_grid_view::vertex<CFG>, T>
  : public grid_function_hash<restricted_grid_view::vertex<CFG>, T>
{
  typedef grid_function_hash<restricted_grid_view::vertex<CFG>, T>  base;
  typedef restricted_grid_view::grid_view<CFG> grid_type;
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
template<class CFG, class T>
class grid_function<restricted_grid_view::cell<CFG>, T>
  : public grid_function_hash<restricted_grid_view::cell<CFG>, T>
{
  typedef grid_function_hash<restricted_grid_view::cell<CFG>, T>  base;
  typedef restricted_grid_view::grid_view<CFG> grid_type;
public:
  grid_function() {}
  grid_function(grid_type const& g) : base(g) {}
  grid_function(grid_type const& g, T const& t) : base(g,t) {}
};


/*
namespace restricted_grid_view {

  template<class GF, class CFG>
  class grid_function_view : public GrAL::grid_function_view::view<GF, grid_view<CFG> >
  {
    typedef GrAL::grid_function_view::view<GF, grid_view<CFG> > base;
  public:
    grid_function_view(GF & gf, grid_view<CFG> const& g) : base(gf,g) {}
  };



  template<class GF, class CFG>
  grid_function_view<GF, CFG>
  get_grid_function_view(GF & gf, grid_view<CFG> const& view) 
  { return grid_function_view<GF, CFG>(gf,view); }


} // namespace restricted_grid_view
*/


} // namespace GrAL 




#ifdef NMWR_INCLUDE_TEMPLATE_DEFS
#include "restricted-grid-view.C"
#endif

#endif
