#ifndef GRAL_GB_HIERARCHICAL_GRID_H
#define GRAL_GB_HIERARCHICAL_GRID_H

// $LICENSE_NEC

#include "Gral/Base/common-grid-basics.h"
#include "Container/bivector.h"

#include "Utility/pre-post-conditions.h"
#include "Utility/ref-ptr.h"
#include "Utility/notifier.h"

#include <iostream>

namespace GrAL {

/*! \brief Support for hierarchical grids

    \author Guntram Berti
 */
namespace hierarchical {

  template<class Grid, class GT> class hgrid_cartesian;
  template<class HGrid> class h_element_base_t;
  template<class ELEMENTBASE> class h_vertex_t;
  template<class ELEMENTBASE> class h_facet_t;
  template<class ELEMENTBASE> class h_cell_t;
  template<class ELEMENTBASE, class FLATELEM> class h_element_t;

  template<class HGrid> class h_cell_child_iterator_t;
  template<class HGrid, class FLATELEM> class h_element_child_iterator_t;
  //template<class ELEMBASE, class ANCHORTAG> class  h_vertex_on_element_iterator_t;
  template<class ELEMBASE, class ELEMTAG, class ANCHORTAG> class  h_incidence_iterator_t;
  // template<class HGrid> class h_facet_child_iterator_t;

  /*! \brief Handle type for hierarchical elements

  */
  template<class HGrid, class FlatHandle> class h_element_handle_t {
    typedef h_element_handle_t<HGrid, FlatHandle>     self;
    typedef HGrid                                     hier_grid_type;
    typedef typename hier_grid_type::level_handle     level_handle;
    typedef FlatHandle                                flat_element_handle;

    flat_element_handle h;
    level_handle        lev;
  public:
    h_element_handle_t() {}
    h_element_handle_t(flat_element_handle hh, level_handle llev) : h(hh), lev(llev) {}
    level_handle     level()       const { return lev;}
    flat_element_handle flat_handle() const { return h;}

    operator flat_element_handle() const { return h;}    
    bool operator==(self const& rhs) const { return h == rhs.h && lev == rhs.lev;}
    bool operator!=(self const& rhs) const { return (!*this == rhs);}

    /*
    struct hasher_type : public typename base::hasher_type_elem_base {
      size_t operator()(h_element_handle_t<HGrid, FlatHandle> h) const { return 
    }
    */
  };

  template<class HGrid, class FlatHandle>
  std::ostream& operator<<(std::ostream& out, h_element_handle_t<HGrid, FlatHandle> h)
  { return (out << h.level() << ' ' << h.flat_handle());}


  template<class Grid, class GT>
  struct grid_types_hgrid_base {
    typedef hgrid_cartesian<Grid, GT>  grid_type;
    typedef typename grid_type::Vertex Vertex;
    typedef typename grid_type::Edge   Edge;
    typedef typename grid_type::Facet  Facet;
    typedef typename grid_type::Cell   Cell;


    typedef typename grid_type::vertex_handle vertex_handle;
    typedef typename grid_type::facet_handle  facet_handle;
    typedef typename grid_type::edge_handle   edge_handle;
    typedef typename grid_type::cell_handle   cell_handle;

    typedef typename grid_type::flatgt        flatgt;
    typedef typename flatgt::dimension_tag    dimension_tag;

    typedef typename grid_type::index_type        index_type;
    typedef typename grid_type::vertex_index_type vertex_index_type;
    typedef typename grid_type::cell_index_type   cell_index_type;
 
    typedef typename grid_type::archgt archgt;

    typedef typename grid_type::VertexOnEdgeIterator  VertexOnEdgeIterator;
    typedef typename grid_type::VertexOnFacetIterator VertexOnFacetIterator;
    typedef typename grid_type::VertexOnCellIterator  VertexOnCellterator;

    typedef typename grid_type::EdgeOnFacetIterator EdgeOnFacetIterator;
    typedef typename grid_type::EdgeOnCellIterator  EdgeOnCellterator;

    typedef typename grid_type::FacetOnCellIterator FacetOnCellIterator;

    typedef typename grid_type::CellChildIterator   CellChildIterator;
 };


  /*! \brief Hierarchical Cartesian grid

      \note
      For unstructured grid, we would need different modifying methods
      (adding a finer or coarser layer requires explicit specification of the new layer
       and its hierarchical relationship to the adjacent pre-existing layer).
       The interface could be unified by always passing a refiner/coarsener when
       adding a new layer, or by passing a map from cells to refinement-patterns and parents.     

      The read-only interface however could probably made generic.
      The only difference is the calculation of parent and child elements,
      which can share a common interface.
  */
  template<class Grid, class GT = grid_types<Grid> >
  class hgrid_cartesian : public notifier {
    typedef hgrid_cartesian<Grid,GT> self;
  public:
    typedef  Grid  flat_grid_type;
    typedef  Grid  pattern_grid_type;
    typedef  GT    flatgt;
    typedef  GT    patterngt;
    typedef  int   level_handle;

    enum { dim = flat_grid_type::dim};

    typedef typename patterngt::index_type pattern_index_type;

    typedef  typename flatgt::Cell          flat_cell_type;  
    typedef  typename flatgt::Facet         flat_facet_type;  
    typedef  typename flatgt::Edge          flat_edge_type;  
    typedef  typename flatgt::Vertex        flat_vertex_type;  
    typedef  typename flatgt::cell_handle   flat_cell_handle;
    typedef  typename flatgt::facet_handle  flat_facet_handle;
    typedef  typename flatgt::edge_handle   flat_edge_handle;
    typedef  typename flatgt::vertex_handle flat_vertex_handle;

    typedef  typename flatgt::index_type    index_type;
    typedef  typename flatgt::vertex_index_type   vertex_index_type; // TODO: make types different.
    typedef  typename flatgt::cell_index_type     cell_index_type; // TODO: make types different.

    typedef typename flatgt::archetype_type     archetype_type;
    typedef typename flatgt::archetype_handle   archetype_handle;
    typedef typename flatgt::archetype_iterator archetype_iterator;
    typedef grid_types<archetype_type>          archgt;               

    typedef typename flatgt::cartesian_subrange_type cart_subrange_type;
    typedef cart_subrange_type                       flat_subrange_type;

    typedef h_element_base_t<self>                        element_base_type;
    typedef h_element_handle_t<self, flat_vertex_handle>  hier_vertex_handle;
    typedef h_element_handle_t<self, flat_cell_handle>    hier_cell_handle;
    typedef h_element_handle_t<self, flat_facet_handle>   hier_facet_handle;
    typedef h_element_handle_t<self, flat_edge_handle>    hier_edge_handle;
    typedef h_cell_t   <element_base_type>                         hier_cell_type;
    typedef h_vertex_t <element_base_type>                         hier_vertex_type;
    typedef h_element_t<element_base_type, typename flatgt::Edge>  hier_edge_type;
    typedef h_element_t<element_base_type, typename flatgt::Facet> hier_facet_type;

    typedef hier_vertex_handle vertex_handle;
    typedef hier_facet_handle  facet_handle;
    typedef hier_cell_handle   cell_handle;
    typedef hier_edge_handle   edge_handle;
    typedef hier_vertex_type         Vertex;
    typedef hier_edge_type           Edge;
    typedef hier_facet_type          Facet;
    typedef hier_cell_type           Cell;

    typedef h_incidence_iterator_t<element_base_type, vertex_type_tag, edge_type_tag > VertexOnEdgeIterator;    
    typedef h_incidence_iterator_t<element_base_type, vertex_type_tag, facet_type_tag> VertexOnFacetIterator;
    typedef h_incidence_iterator_t<element_base_type, vertex_type_tag, cell_type_tag > VertexOnCellIterator;

    typedef h_incidence_iterator_t<element_base_type, edge_type_tag, facet_type_tag> EdgeOnFacetIterator;
    typedef h_incidence_iterator_t<element_base_type, edge_type_tag, cell_type_tag > EdgeOnCellIterator;
    typedef h_incidence_iterator_t<element_base_type, facet_type_tag,cell_type_tag > FacetOnCellIterator;
   
    typedef h_cell_child_iterator_t<self>  CellChildIterator;

    // export associated templates
    template<class HG, class FLAT_HANDLE>
    struct element_handle_template { typedef h_element_handle_t<HG, FLAT_HANDLE> type;};
    template<class ELEMENT_BASE>
    struct vertex_template { typedef h_vertex_t<ELEMENT_BASE> type;};
    template<class ELEMENT_BASE>
    struct facet_template  { typedef h_element_t<ELEMENT_BASE, typename flatgt::Facet> type;};
    template<class ELEMENT_BASE>
    struct cell_template   { typedef h_cell_t  <ELEMENT_BASE> type;};

    // export type of hierarchical indexed table for dependent classes
    template<class T>
    struct table_type_template { typedef bivector<T> type;};
 
   typedef typename table_type_template<flat_grid_type>::type table_type;
    // typedef  bivector<flat_grid_type> table_type;
    typedef typename table_type::size_type size_type;


    // type for dependent classes (like grid functions) to inherit from
    class observer_type : public observer {
      typedef observer                   base;
    public:
      typedef hgrid_cartesian<Grid, GT>  notifier_type;      
      typedef notifier                   notifier_base;

      observer_type() {}
      observer_type(notifier_type const* n) // : base(n) {} // static_cast<notifier_base const*>(n)) {}
      { connect(n); }
 
      virtual void set_grid(notifier_type const* n) { connect(n);}
      virtual void notifier_assigned(notifier_base const* n) = 0;
      virtual void hgrid_level_added  (notifier_type const* n, level_handle added)   = 0;
      virtual void hgrid_level_removed(notifier_type const* n, level_handle removed) = 0;
    };

  private:
    pattern_grid_type      the_pattern;
    table_type             grids;
  public:
    hgrid_cartesian() {}
    hgrid_cartesian(flat_grid_type    const& root,
		    pattern_grid_type const& pat);
    hgrid_cartesian(ref_ptr<flat_grid_type const> root,
		    pattern_grid_type const& pat);
    void init(flat_grid_type    const& root, 
	      pattern_grid_type const& pat);

    self& operator=(self const& rhs);
 
    /*! \name Level navigation
     */
    //@{
    //! get the coarsest (root) level
    level_handle  coarsest_level() const { cv(); return level_handle(grids.begin_index());}
    //! get the finest level
    level_handle  finest_level()   const { cv(); return level_handle(grids.back_index());}
    /*! \brief get handle of next finer level
      \pre <tt> lev != finest_level() </tt>
    */
    level_handle  next_finer_level  (level_handle lev) const { cv(lev); return ++lev;}
    /*! \brief  get handle of next coarser level
      \pre <tt> lev != coarsest_level() </tt>
    */
    level_handle  next_coarser_level(level_handle lev) const { cv(lev); return --lev;}
    //! get the number of levels
    size_type     num_of_levels() const { return grids.size();}
    //@}

    //! Get the refinement pattern
    ref_ptr<pattern_grid_type const> ThePatternGrid() const 
    {  return ref_ptr<pattern_grid_type const>(the_pattern);}

    //! Shortcut for \c FlatGrid(lev)
    ref_ptr<flat_grid_type const> operator()(level_handle lev) const { return FlatGrid(lev);}
    //! Get the flat grid of level \c lev
    ref_ptr<flat_grid_type const> FlatGrid(level_handle lev) const   
    { cv(lev); return  ref_ptr<flat_grid_type const>(grids(lev));}

 
    //! Equivalent to <tt> num_of_levels() == 0 </tt>
    bool  empty() const { return num_of_levels()==0;}

    /*! True iff for each grid dimension, the number of cells of the refinement pattern in this dimension
        divides the corresponding number of cells of the coarse grid.
        E.g. if the cell size of the pattern is 2x3x4 and the cell size of the coarsest grid is 8x6x10,
        it does not work because 10 is not a multiple of 4.
    */
    bool coarsenable() const { return does_divide(the_pattern.cell_size(), grids.front().cell_size());}
    
    ref_ptr<const table_type> table() const { return ref_ptr<table_type const>(grids);}

    /*! \name Modifying operations
     */
    //@{
    /*! \brief Add a finer level.
      
    \pre There is at least one level: <tt> num_of_levels() > 0 </tt>
    \post <tt> level_handle lev = add_finer_level(); lev == finest_level(); </tt>
    \post No cells are active in the newly created level level.
    */
    level_handle add_finer_level();

    /*! \brief add a coarser level
       
         \pre <tt> num_of_levels() > 0 </tt>
	 \pre The number of cells in each direction of \c coarsest_level() is dividable by 
              the corresponding number of cells in pattern.

    */
    level_handle add_coarser_level();
    
    /*! 
      \pre <tt> num_of_levels() > 0 </tt>
    */
    void remove_finest_level(); 
    
    /*! 
      \pre <tt> num_of_levels() > 0 </tt>
    */
    void remove_coarsest_level();
    //@}

    template<class HG, class flat_handle_type>
    bool valid_handle(h_element_handle_t<HG,flat_handle_type> h) const 
    { return valid_level(h.level()) && FlatGrid(h.level())->valid_handle(h.flat_handle());} 
    bool valid_handle(hier_cell_handle   h) const 
    { return valid_level(h.level()) && FlatGrid(h.level())->valid_handle(h.flat_handle());} 
    bool valid_level(level_handle lev)   const { return coarsest_level() <= lev && lev <= finest_level();}
    void cv   (level_handle lev) const { REQUIRE( valid_level(lev), "lev=" << lev, 1); }
    bool valid() const { return !empty();}
    void cv   () const { REQUIRE(valid(), "", 1);}
    void ce   () const { REQUIRE( (!empty()), "", 1);}

    index_type parent_cell_index(index_type ch, level_handle lev) const 
    { cv(lev); cv(lev-1); return quotient(ch,  the_pattern.cell_size());}

    index_type parent_cell_index(index_type ch, level_handle lev, level_handle parent_lev) const 
    { 
      cv(lev); cv(parent_lev); 
      REQUIRE(parent_lev <= lev, "parent_lev=" << parent_lev << " lev=" << lev, 1);
      return quotient(ch,  power(the_pattern.cell_size(), lev-parent_lev));
    }

    hier_cell_type  Parent(hier_cell_type const& ch) const 
    {
      cv(ch.level()); 
      level_handle pl = next_coarser_level(ch.level());
      return hier_cell_type(*this,flat_cell_type(* FlatGrid(pl), parent_cell_index(ch.Flat().index(), ch.level())), pl); 
    }

    hier_cell_type  Parent(hier_cell_type const& ch, level_handle pl) const 
    {
      return hier_cell_type(*this,flat_cell_type(* FlatGrid(pl), parent_cell_index(ch.Flat().index(), ch.level(), pl)), pl); 
    }

    index_type child_cell_index(index_type parent, pattern_index_type loc_ch, level_handle parent_lev) const
    { cv(parent_lev); return product(parent, the_pattern.cell_size()) + loc_ch; }

    hier_cell_type Child(hier_cell_type const& p, pattern_index_type loc) const 
    { 
      level_handle chl = next_finer_level(p.level());
      return hier_cell_type(*this, flat_cell_type(* FlatGrid(chl), child_cell_index(p.Flat().index(), loc, chl)), chl);
    }

    template<class HELEM>
    HELEM Child(HELEM const& p, pattern_index_type loc) const;



    template<class HVERTEX>
    HVERTEX CoarsestParent(HVERTEX const& v) const
    { 
      return HVERTEX(v.TheGrid(),coarsest_parent(v));
    } 

    template<class HVERTEX>
    typename HVERTEX::vertex_handle
    coarsest_parent(HVERTEX const& v) const;

    template<class HVERTEX>
    HVERTEX DescendantVertex(HVERTEX const& v, level_handle fine_level) const 
    {
      return HVERTEX(v.TheGrid(), descendant_vertex(v,fine_level));
    }

    template<class HVERTEX>
    typename HVERTEX::vertex_handle
    descendant_vertex(HVERTEX const& v, level_handle fine_level) const 
    {
      unsigned level_diff = fine_level - v.level();
      index_type factor = power(the_pattern.cell_size(), level_diff);
      return typename HVERTEX::vertex_handle(FlatGrid(fine_level)->get_vertex_handle(product(v.Flat().index(),factor)), fine_level);
    }
 
    /*! \brief Descendant on level \c h of the cell \p 
      If <tt> h == p.level() +1 </tt>, this are just  the children
    */
    //temporary<cart_subrange_type>  descendants(hier_cell_type const& p, level_handle h) const;
    ref_ptr<cart_subrange_type>  descendants(hier_cell_type const& p, level_handle h) const;

    // hier_cell_handle parent(hier_cell_type const& c) const { return quotient(c.Flat().index(), the_pattern.size());}
    // hier_cell_handle child(HCell const& c, pattern_cell_handle h) const;

    template<class ELEMENTBASE, class FLATELEM>
    ref_ptr<cart_subrange_type> descendants(h_element_t<ELEMENTBASE,FLATELEM> const& p, level_handle h) const;
    // temporary<cart_subrange_type> descendants(h_element_t<ELEMENTBASE,FLATELEM> const& p, level_handle h) const;

    template<class ELEMENTBASE, class FLATELEM>
    //  temporary<cart_subrange_type> children   (h_element_t<ELEMENTBASE,FLATELEM> const& p) const 
    ref_ptr<cart_subrange_type> children   (h_element_t<ELEMENTBASE,FLATELEM> const& p) const 
    { return descendants(p, p.level()+1);}


  }; // class hgrid_cartesian<Grid>

} // namespace hierarchical

template<class Grid, class GT>
struct grid_types<hierarchical::hgrid_cartesian<Grid, GT> >
  : public grid_types_base<hierarchical::grid_types_hgrid_base<Grid,GT> > 
{ };

namespace hierarchical {

  //-------------------- Grid Elements and Iterators ------------------------

  /*! \brief Base type to parameterize element and iterator types on \c HGrid

      \author Guntram Berti

      \templateparams
      - \c HGrid is a hierarchical grid type.
         Model: hgrid_cartesian<GRID, GT>

      For using the element types like \c h_vertex_t<>, \c h_cell_t<> for other
      hierarchical structures, we must provide an adaptation of this class,
      see e.g. 
      \c octree::oct_element_base_t or 
      \c octree::nc_leaf_grid_element_base_t.
   */
  template<class HGrid>
  class h_element_base_t {
  public:
    // types related to hierarchical grid type
    typedef HGrid                        hier_grid_type;
    typedef HGrid                        hgt; // hierarchical grid types

    // types related to the 'current' grid type (which is the hierarchical grid)
    // This (and all types derived from gt) will be the main difference when
    // implementing adaptations of this class to other hierarchical grid classes
    // building on hgrid<>. 
    typedef hgt                          gt; //  grid types
    typedef HGrid                        grid_type;

    typedef typename gt::Cell           Cell;
    typedef typename gt::Vertex         Vertex;
    typedef typename gt::cell_handle    cell_handle;
    typedef typename gt::vertex_handle  vertex_handle;
    typedef typename gt::level_handle   level_handle;

    // types related to the underlying flat grid
    typedef typename hgt::flatgt         flatgt;
    typedef typename hgt::flat_grid_type flat_grid_type;
  private:
    ref_ptr<const grid_type> hg;
    level_handle             lev;
  public:
    h_element_base_t() {}
    explicit h_element_base_t(grid_type const& hgg)
      : hg(hgg), lev(hgg.finest_level())  {}
    h_element_base_t(grid_type const& hgg, level_handle lv)
      : hg(hgg), lev(lv)  {}
    explicit h_element_base_t(ref_ptr<const grid_type> hgg)
      : hg(hgg), lev(hgg->finest_level())  {}
    h_element_base_t(ref_ptr<const grid_type> hgg, level_handle lv)
      : hg(hgg), lev(lv)  {}

  protected:
    void set(level_handle l) { lev = l;}
  public:
    bool bound() const { return hg != 0;}
    void cb()    const { REQUIRE(bound(), "", 1); }
    level_handle level() const { return lev;}

    ref_ptr<const hier_grid_type> TheHierGrid() const { return hg;}
    ref_ptr<const flat_grid_type> TheFlatGrid() const { return hg->FlatGrid(lev);}
    ref_ptr<const grid_type>      TheGrid() const { return TheHierGrid();}

    // transparent access to different grid types via their types
    // we cannot specialize member function templates of a non-specialized template class,
    // so we must use the tp<> hack.
    ref_ptr<const hier_grid_type> Grid(tp<hier_grid_type>) const { return TheHierGrid();}
    ref_ptr<const flat_grid_type> Grid(tp<flat_grid_type>) const { return TheFlatGrid();}
  };

  /*! \brief A family of vertex types, to be used with hierarchical grids and 'derivative work'.

      \author Guntram Berti

      'Derivative work' may be octrees which use hierarchical grids.

      \templateparams
      - \c ELEMENTBASE
         defines the types needed by elements, introducing grid type specific
         information.
         Example: \c  h_element_base_t<> 
   */
  template<class ELEMENTBASE>
  class h_vertex_t : public ELEMENTBASE {
    typedef h_vertex_t<ELEMENTBASE> self;
    typedef ELEMENTBASE             base;

  public:
    typedef typename base::gt              gt;
    typedef typename base::grid_type       grid_type;
    typedef typename base::level_handle    level_handle;
    typedef typename gt::Vertex            Vertex;
    typedef typename gt::vertex_handle     vertex_handle;

    typedef typename base::flatgt          flatgt;
    typedef typename flatgt::vertex_handle flat_vertex_handle;
    typedef typename flatgt::Vertex        flat_vertex_type;

    typedef flat_vertex_type               flat_element_type;
    typedef typename flat_element_type::index_type  index_type;
  private:
    flat_vertex_handle       h;
  public:
    // icc 6.0 does not like this
    //using base::TheFlatGrid;
    //using base::TheHierGrid;
    //using base::level;
    //using base::bound;

    h_vertex_t() {}
    h_vertex_t(grid_type const& gg, vertex_handle hh) : base(gg, hh.level()), h(hh.flat_handle()) {}
    h_vertex_t(grid_type const& gg, flat_vertex_type   f, level_handle lev) : base(gg,lev), h(f.handle()) {}
    h_vertex_t(grid_type const& gg, flat_vertex_handle f, level_handle lev) : base(gg,lev), h(f) {}
    h_vertex_t(ref_ptr<grid_type const> gg, vertex_handle hh) : base(gg, hh.level()), h(hh.flat_handle()) {}
    h_vertex_t(ref_ptr<grid_type const> gg, flat_vertex_type   f, level_handle lev) : base(gg,lev), h(f.handle()) {}
    h_vertex_t(ref_ptr<grid_type const> gg, flat_vertex_handle f, level_handle lev) : base(gg,lev), h(f) {}
    h_vertex_t(ref_ptr<grid_type const> gg, index_type const& idx, level_handle lev) 
      : base(gg,lev), h(base::TheFlatGrid()->template index2handle<0>(idx,0)) {}

    // construct from 'sibling' using another ELEMBASE type.
    // Can only upcast: ELEMBASE must know about EBASE2
    template<class EBASE2>
    h_vertex_t(h_vertex_t<EBASE2> const& s) : base(s.Grid(tp<grid_type>()), s.level()), h(s.flat_handle()) {}

    // can only 'downcast' :ELEMBASE must know about EBASE2
    template<class EBASE2>
    h_vertex_t<EBASE2> to() const { cv(); return h_vertex_t<EBASE2>(Grid(tp<typename EBASE2::grid_type>()), base::level(), h);}

  protected:
    void set(vertex_handle newh) { set(newh.flat_handle(), newh.level());}
    void set(flat_vertex_handle newfh, level_handle newlev) { base::set(newlev); h = newfh;}
  public:
    vertex_handle handle() const { cv(); return vertex_handle(h,base::level());}

    flat_vertex_handle flat_handle() const { cv(); return h;}
    flat_vertex_type Flat() const { cv(); return flat_vertex_type(* base::TheFlatGrid(),h);}
    operator flat_vertex_type() const { return Flat();}

    index_type low_vertex_index() const { return Flat().index();}
    index_type high_vertex_index() const { return Flat().index();}
    // level_handle natural_level() const { return TheHierGrid()->natural_level(*this);}

    // hier_vertex_type Parent () { return TheHierGrid()->Parent(*this);}
    // unsigned dim_of_parent() const { return TheHierGrid()->dim_of_parent(*this);}
    // hier_cell_type   ParentCell() { return g->Parent(*this);}
    // ParentCellIterator   FirstParentCell() { return g->Parent(*this);}

    // should "equivalent" vertices on different levels compare equal? No, only in leaf_grid!
    friend bool operator==(self const& lhs, self const& rhs) { 
      lhs.cv(); rhs.cv(); return lhs.h == rhs.h && lhs.level() == rhs.level();}
    friend bool operator!=(self const& lhs, self const& rhs) { lhs.cv(); rhs.cv(); return !(lhs == rhs);}
   
    // inline ChildIterator FirstChild() const;
    // inline ChildIterator EndChild  () const;

    // checking functions
    bool valid() const { return base::bound() && base::TheHierGrid()->valid_handle(vertex_handle(h,base::level()));}
    void cv() const { REQUIRE(valid(), "", 1);}
    
  }; // class h_vertex_t<HGrid>


  /*! \brief A family of cell types, to be used with hierarchical grids and 'derivative work'.
      \author Guntram Berti

      'Derivative work' may be octrees which use hierarchical grids.

      \templateparams
      - \c ELEMENTBASE
         defines the types needed by elements, introducing grid type specific
         information.
         Example: \c  h_element_base_t<> 
   */
  template<class ELEMENTBASE>
  class h_cell_t : public ELEMENTBASE {
    typedef h_cell_t<ELEMENTBASE> self;
    typedef ELEMENTBASE           base;
  public:
    typedef typename base::grid_type       grid_type;
    typedef typename base::gt              bgt;
    typedef grid_types<grid_type>          gt;
    typedef typename base::level_handle    level_handle;
    typedef typename gt::Cell              Cell;
    typedef typename gt::cell_handle       cell_handle;
    typedef typename gt::Vertex            Vertex;
    typedef typename gt::vertex_handle     vertex_handle;
    typedef typename bgt::archgt            archgt;
    //    typedef typename gt::archgt            archgt;

    typedef typename archgt::Vertex        archVertex;

    typedef typename base::flatgt          flatgt;
    typedef typename flatgt::cell_handle   flat_cell_handle;
    typedef typename flatgt::Cell          flat_cell_type;
    typedef flat_cell_type                 flat_element_type;
    typedef element_traits<flat_element_type>   fet;

    enum { dim = fet::dim, griddim = grid_type::dim };

    typedef typename flat_element_type::index_type       index_type;
    typedef typename flat_element_type::local_index_type local_index_type;

    //    typedef typename gt::incidence_iterator_d<dim-1,dim>::type SideIterator;
    typedef h_incidence_iterator_t<base, facet_type_tag, cell_type_tag> SideIterator;
    typedef h_incidence_iterator_t<base, edge_type_tag,  cell_type_tag> EdgeOnElementIterator;
    
    //  typedef typename gt::CellChildIterator                     ChildIterator;
    typedef h_cell_child_iterator_t<grid_type> ChildIterator;
  private:

    flat_cell_handle         h;
  public:
    //using base::TheGrid;
    //using base::TheFlatGrid;
    //using base::TheHierGrid;
    //using base::level;
    //using base::bound;

    h_cell_t() {}
    h_cell_t(grid_type const& gg, cell_handle hh) : base(gg, hh.level()), h(hh.flat_handle()) {}
    h_cell_t(grid_type const& gg, flat_cell_type   f, level_handle lev) : base(gg,lev), h(f.handle()) {}
    h_cell_t(grid_type const& gg, flat_cell_handle f, level_handle lev) : base(gg,lev), h(f) {}
    h_cell_t(ref_ptr<grid_type const> gg, cell_handle hh) : base(gg, hh.level()), h(hh.flat_handle()) {}
    h_cell_t(ref_ptr<grid_type const> gg, flat_cell_type   f, level_handle lev) : base(gg,lev), h(f.handle()) {}
    h_cell_t(ref_ptr<grid_type const> gg, ref_ptr<flat_cell_type const>  f, level_handle lev) : base(gg,lev), h(f->handle()) {}
    h_cell_t(ref_ptr<grid_type const> gg, flat_cell_handle f, level_handle lev) : base(gg,lev), h(f) {}
 
    // construct from 'sibling' using another ELEMBASE type
    // Can only 'upcast':  EBASE2 must 'know' about ELEMENTBASE
    template<class EBASE2>
    h_cell_t(h_cell_t<EBASE2> const& s) : base(s.Grid(tp<grid_type>()), s.level()), h(s.flat_handle()) {}

    // can only 'downcast' :ELEMBASE must know about EBASE2
    template<class EBASE2>
    h_cell_t<EBASE2> to() const { cv(); return h_cell_t<EBASE2>(Grid(tp<typename EBASE2::grid_type>()), base::level(), h);}

    cell_handle handle() const { cv(); return cell_handle(h,base::level());}

    flat_cell_handle flat_handle() const { cv();  return h;}
    flat_cell_type Flat() const { cv(); return flat_cell_type(* base::TheFlatGrid(), flat_handle());}
    operator flat_cell_type() const { cv(); return Flat();}

    index_type low_vertex_index()  const { return Flat().low_vertex_index();}
    index_type high_vertex_index() const { return Flat().high_vertex_index();}

    Vertex        V(archVertex const& lV) const { return Vertex(base::TheGrid(), v(lV));}
    vertex_handle v(archVertex const& lV) const { return vertex_handle(Flat().v(lV), base::level());}

    vertex_handle v(index_type const& iv) const { return vertex_handle(Flat().v(iv), base::level());}
    Vertex        V(index_type const& iv) const { return Vertex(base::TheGrid(), v(iv));}

    typedef typename flat_element_type::local_index_type local_index_type;
    vertex_handle vl(local_index_type const& lv) const { return vertex_handle(Flat().vl(lv), base::level());}
    Vertex        Vl(local_index_type const& lv) const { return Vertex(base::TheGrid(), vl(lv));}



    // Does not work for conforming leaf grids, but these should pobably not use this class anyhow.
    size_t NumOfVertices() const { return Flat().NumOfVertices();}

    // will this work with ELEMENTBASE other than h_element_base_t<> ?
    Cell Parent    () { cv(); return Cell(base::TheHierGrid()->Parent(*this));}
    Cell ParentCell() { cv(); return Cell(base::TheHierGrid()->Parent(*this));}

    friend bool operator==(self const& lhs, self const& rhs) { lhs.cv(); rhs.cv(); return lhs.h == rhs.h && lhs.level() == rhs.level();}
    friend bool operator!=(self const& lhs, self const& rhs) { lhs.cv(); rhs.cv(); return !(lhs == rhs);}

    inline ChildIterator FirstChild() const;
    inline ChildIterator EndChild  () const;

    // checking functions
    bool valid() const { return base::bound() && base::TheHierGrid()->valid_handle(cell_handle(h, base::level()));}
    void cv() const { REQUIRE(valid(), "h=" << h << " level=" << base::level(), 1);}
  }; // class h_cell_t<ELEMENTBASE>



  /*! \brief A family of general types, to be used with hierarchical grids and 'derivative work'.
      \author Guntram Berti

      'Derivative work' may be octrees which use hierarchical grids.

      \templateparams
      - \c ELEMENTBASE
         defines the types needed by elements, introducing grid type specific
         information.
         Example: \c  h_element_base_t<> 
   */
  template<class ELEMENTBASE, class FLATELEM>
  class h_element_t : public ELEMENTBASE {
    typedef h_element_t<ELEMENTBASE, FLATELEM> self;
  public:
    typedef ELEMENTBASE                        base;
    typedef typename base::grid_type           grid_type;
    typedef grid_types<grid_type>              gt;

    typedef typename base::level_handle    level_handle;
    // typedef typename gt::index_type        index_type;

    typedef FLATELEM                            flat_element_type;
    typedef element_traits<flat_element_type>   fet;
    typedef typename base::flatgt               flatgt;
    typedef typename fet::handle_type           flat_element_handle;
 
    typedef typename flat_element_type::index_type       index_type;
    typedef typename flat_element_type::local_index_type local_index_type;

    enum { dim = fet::dim, griddim = grid_type::dim };

    typedef typename gt::template element_d       <dim>::type     Element;
    typedef typename gt::template element_handle_d<dim>::type     element_handle;
    typedef typename gt::template incidence_iterator_d<0,dim>::type VertexOnElementIterator;
    typedef typename gt::template incidence_iterator_d<1,dim>::type EdgeOnElementIterator;

    // Without this enum, ICE in gcc 2.96 in incidence_iterator_d<dim-1,dim>
    enum { dim1 = dim -1};
    typedef typename gt::template incidence_iterator_d<dim1,dim>::type SideIterator;


    typedef typename gt::Vertex            Vertex;
    typedef typename gt::vertex_handle     vertex_handle;
    typedef typename gt::archgt            archgt;
    typedef typename archgt::Vertex        archVertex;

    typedef h_element_child_iterator_t<grid_type, flat_element_type> ChildIterator;

    flat_element_handle         h;
  public:
    //using base::TheGrid;
    //using base::TheFlatGrid;
    //using base::TheHierGrid;
    //using base::level;
    //using base::bound;

    h_element_t() {}
    h_element_t(grid_type const& gg, element_handle hh) : base(gg, hh.level()), h(hh.flat_handle()) {}
    h_element_t(grid_type const& gg, flat_element_type   f, level_handle lev) : base(gg,lev), h(f.handle()) {}
    h_element_t(grid_type const& gg, flat_element_handle f, level_handle lev) : base(gg,lev), h(f) {}
    h_element_t(ref_ptr<grid_type const> gg, element_handle hh) : base(gg, hh.level()), h(hh.flat_handle()) {}
    h_element_t(ref_ptr<grid_type const> gg, flat_element_type   f, level_handle lev) : base(gg,lev), h(f.handle()) {}
    h_element_t(ref_ptr<grid_type const> gg, ref_ptr<flat_element_type const>  f, level_handle lev) : base(gg,lev), h(f->handle()) {}
    h_element_t(ref_ptr<grid_type const> gg, flat_element_handle f, level_handle lev) : base(gg,lev), h(f) {}
 
    // construct from 'sibling' using another ELEMBASE type
    // Can only 'upcast':  EBASE2 must 'know' about ELEMENTBASE
    template<class EBASE2>
    h_element_t(h_element_t<EBASE2, flat_element_type> const& s) : base(s.Grid(tp<grid_type>()), s.level()), h(s.flat_handle()) {}

    // can only 'downcast' :ELEMBASE must know about EBASE2
    template<class EBASE2>
    h_element_t<EBASE2, flat_element_type> to() const 
    { cv(); return h_element_t<EBASE2,flat_element_type>(Grid(tp<typename EBASE2::grid_type>()), base::level(), h);}

    element_handle handle() const { cv(); return element_handle(h,base::level());}

       
    index_type low_vertex_index()  const { return Flat().low_vertex_index();}
    index_type high_vertex_index() const { return Flat().high_vertex_index();}
   

    flat_element_handle flat_handle() const { cv();  return h;}
    flat_element_type   Flat()        const { cv(); return flat_element_type(* base::TheFlatGrid(), flat_handle());}
    operator flat_element_type()      const { cv(); return Flat();}

    Vertex        V(archVertex const& lV) const { return Vertex(base::TheGrid(), v(lV));}
    vertex_handle v(archVertex const& lV) const { return vertex_handle(Flat().v(lV), base::level());}

    vertex_handle v(index_type const& iv) const { return vertex_handle(Flat().v(iv), base::level());}
    Vertex        V(index_type const& iv) const { return Vertex(base::TheGrid(), v(iv));}

    typedef typename flat_element_type::local_index_type local_index_type;
    vertex_handle vl(local_index_type const& lv) const { return vertex_handle(Flat().vl(lv), base::level());}
    Vertex        Vl(local_index_type const& lv) const { return Vertex(base::TheGrid(), vl(lv));}


    VertexOnElementIterator FirstVertex() const { return VertexOnElementIterator(*this);}
    VertexOnElementIterator EndVertex()   const { return VertexOnElementIterator(*this, Flat().EndVertex());}

    // Does not work for conforming leaf grids, but these should pobably not use this class anyhow.
    size_t NumOfVertices() const { return Flat().NumOfVertices();}

    // will this work with ELEMENTBASE other than h_element_base_t<> ?
    //  Element Parent    () { cv(); return Element(TheHierGrid()->Parent(*this));}
    //  Element ParentElement() { cv(); return Element(TheHierGrid()->Parent(*this));}

    friend bool operator==(self const& lhs, self const& rhs) { lhs.cv(); rhs.cv(); return lhs.h == rhs.h && lhs.level() == rhs.level();}
    friend bool operator!=(self const& lhs, self const& rhs) { lhs.cv(); rhs.cv(); return lhs.h != rhs.h;}

    inline ChildIterator FirstChild() const;
    inline ChildIterator EndChild  () const;

    // checking functions
    bool valid() const { return base::bound() && base::TheHierGrid()->valid_handle(element_handle(h, base::level()));}
    void cv() const { REQUIRE(valid(), "h=" << h << " level=" << base::level(), 1);}
  }; // class h_element_t<ELEMENTBASE>



  //------------- child iterator on cells ------------------

  template<class HGrid>
  class h_cell_child_iterator_t {
    typedef h_cell_child_iterator_t<HGrid> self;
  public:
    typedef HGrid    hier_grid_type;
    typedef typename HGrid::level_handle level_handle;
    typedef typename HGrid::flat_cell_type flat_cell_type;
    typedef typename HGrid::hier_cell_type hier_cell_type;
    typedef typename HGrid::patterngt::CellIterator PatternCellIterator;
    typedef typename HGrid::hier_cell_handle hier_cell_handle;

  private:
    hier_cell_type             p;  // parent
    PatternCellIterator  ch; // current child
  public:
    h_cell_child_iterator_t() {}
    h_cell_child_iterator_t(hier_cell_type pp) : p(pp), ch(TheHierGrid()->ThePatternGrid()->FirstCell()) {}
    h_cell_child_iterator_t(hier_cell_type pp, PatternCellIterator cch) : p(pp), ch(cch) {}

    hier_cell_type operator*()         const { cv(); return TheHierGrid()->Child(Parent(),(*ch).index());}
    self& operator++() { cv(); ++ch; return *this;}
    //    hier_cell_handle handle() const { return TheHGrid().child_cell(ch,p);}
    bool IsDone() { cb(); return ch.IsDone();}

    level_handle  level() const { return TheHierGrid()->next_finer_level(p.level());}
    flat_cell_type Flat() const { cv(); return operator*().Flat();} 

    hier_cell_type const& Parent() const { cb(); return p;}
    ref_ptr<const hier_grid_type>  TheHierGrid() const { cb(); return p.TheHierGrid();}

    // checking functions
    bool bound() const { return p.bound();}
    bool valid() const { return bound() && ch.valid();}
    void cb() const { REQUIRE(bound(), "", 1);}
    void cv() const { REQUIRE(valid(), "", 1);}

    friend bool operator==(self const& lhs, self const& rhs)  
    { lhs.cb(); rhs.cb(); return (lhs.p == rhs.p) && (lhs.ch == rhs.ch);}
    friend bool operator!=(self const& lhs, self const& rhs)  { return !(lhs == rhs);}

  }; // class h_cell_child_iterator_t<HGrid>



  //------------- child iterator on general elements ------------------

  template<class HGRID, class FLATELEM>
  class h_element_child_iterator_t {
    typedef h_element_child_iterator_t<HGRID,FLATELEM> self;
  public:
    typedef HGRID                        hier_grid_type;
    typedef grid_types<hier_grid_type>   hgt;

    typedef FLATELEM                            flat_element_type;
    typedef element_traits<flat_element_type>   fet;
    enum { dim = fet::dim };
    typedef typename fet::handle_type           flat_element_handle;
    typedef typename hgt::flatgt                flatgt;

    typedef typename hgt::template element_d       <dim>::type     element_type;
    typedef typename hgt::template element_handle_d<dim>::type     element_handle;

    typedef typename HGRID::level_handle level_handle;


    typedef typename HGRID::cart_subrange_type           cart_subrange_type;
    typedef grid_types<cart_subrange_type>               rgt;
    typedef typename rgt::template sequence_iterator_d<dim>::type RangeElementIterator;
  private:
    element_type                        p;  // parent
    ref_ptr<cart_subrange_type const>   children; // range of children
    RangeElementIterator                ch; // current child

  public:
    h_element_child_iterator_t() {}
    h_element_child_iterator_t(element_type const& pp) 
      : p(pp), children(TheHierGrid()->children(p)), ch(*children) {}
    // hack: must pass subrange ... 
    h_element_child_iterator_t(element_type         const& pp, 
			       cart_subrange_type   const& children_,
			       RangeElementIterator const& child) 
      : p(pp), children(children_), ch(child) {}
    h_element_child_iterator_t(element_type         const& pp, 
			       ref_ptr<cart_subrange_type const> children_,
			       RangeElementIterator const& child) 
      : p(pp), children(children_), ch(child) {}

    element_type operator*()         const { cv(); return element_type(TheHierGrid(), *ch, level()); }
    // TheHierGrid()->Child(Parent(),(*ch).index());}
    self& operator++() { cv(); ++ch; return *this;}

    bool IsDone() { cb(); return ch.IsDone();}

    level_handle      level() const { return TheHierGrid()->next_finer_level(p.level());}
    flat_element_type Flat() const { cv(); return operator*().Flat();} 

    element_type const& Parent() const { cb(); return p;}
    ref_ptr<const hier_grid_type>  TheHierGrid() const { cb(); return p.TheHierGrid();}

    // checking functions
    bool bound() const { return p.bound();}
    bool valid() const { return bound() && ch.valid();}
    void cb() const { REQUIRE(bound(), "", 1);}
    void cv() const { REQUIRE(valid(), "", 1);}

    friend bool operator==(self const& lhs, self const& rhs)  
    { lhs.cb(); rhs.cb(); return (lhs.p == rhs.p) && (lhs.ch == rhs.ch);}
    friend bool operator!=(self const& lhs, self const& rhs)  { return !(lhs == rhs);}

  }; // class h_element_child_iterator_t<HGrid>


  /*! VertexOnElementIterator for hierarchical grids

  */

  template<class ELEMBASE, class ELEMTAG,  class ANCHORTAG> 
  //template<class ELEMBASE, class ANCHORTAG> 
  class h_incidence_iterator_t : public ELEMBASE {
    typedef  h_incidence_iterator_t<ELEMBASE, ELEMTAG, ANCHORTAG> self;
    typedef  ELEMBASE                                            base;
  public:
    typedef typename base::grid_type     grid_type;
    //    typedef typename base::gt            gt;
    typedef grid_types<grid_type>        gt;
    typedef ANCHORTAG                    anchor_type_tag;
    typedef ELEMTAG                      element_type_tag;
    typedef typename gt::template element<ANCHORTAG>::type  anchor_type;
    
    typedef typename gt::template element<ELEMTAG>::type  element_type;
    typedef typename gt::template element_handle<element_type_tag>::type element_handle;
    //typedef typename gt::Vertex          Vertex;
    //typedef typename gt::vertex_handle   vertex_handle;

    // typedef Cell                         anchor_type;
    //  typedef Vertex                       value_type;
    // typedef Vertex                       element_type;
    typedef element_type value_type;

    typedef typename gt::flatgt          flatgt;
    typedef typename flatgt::template incidence_iterator<element_type_tag, anchor_type_tag>::type  
                                         flat_incidence_iterator;

    //using base::cb;
    //using base::bound;
    //using base::level;
    //using base::TheGrid;

    h_incidence_iterator_t() {}
    explicit 
    h_incidence_iterator_t(anchor_type const& cc)
      : base(cc),  vc(cc.Flat()) {}  // vc(cc.Flat().FirstVertex()) {}
    h_incidence_iterator_t(anchor_type const& cc, flat_incidence_iterator vcc)
      : base(cc),  vc(vcc) {}

    self&   operator++()       { cv(); ++vc; return  (*this);}
    value_type  operator*()  const { return get_element();}
    bool    IsDone()     const { base::cb(); return vc.IsDone();}
  
    element_type get_element() const { cv(); return element_type(base::TheGrid(),  element_handle(vc.handle(), base::level()));}
    element_handle   handle()  const { cv(); return get_element().handle();}

    // temporary<Cell> TheCell()     const { cb(); return temporary<Cell>(Cell(TheGrid(), vc.TheAnchor(), level())) ;}
    temporary<anchor_type> TheAnchor() const { base::cb(); return temporary<anchor_type>(anchor_type(base::TheGrid(), *vc.TheAnchor(), base::level())) ;}
  
    friend bool operator==(self const& lhs, self const& rhs) 
    { lhs.cb(); rhs.cb(); return lhs.level() == rhs.level() && lhs.vc == rhs.vc;}
    friend bool operator!=(self const& lhs, self const& rhs) { return !(lhs == rhs);}
  
    bool valid() const { return base::bound() && vc.valid();}
    void cv()    const { REQUIRE(valid(), "", 1);}
  private:
    flat_incidence_iterator  vc;
  };


  //------------- inline functions -----------------

  template<class Grid, class GT>
  template<class HELEM>
  HELEM hgrid_cartesian<Grid,GT>::Child(HELEM const& p,  
					typename hgrid_cartesian<Grid,GT>::pattern_index_type loc) const
    { 
      level_handle chl = next_finer_level(p.level());
      return HELEM(*this, 
		   typename HELEM::flat_element_type(* FlatGrid(chl), 
						     loc,
						     p.Flat().direction()), 
		   chl);
    }

  
  template<class HGrid>
  inline  typename h_cell_t<HGrid>::ChildIterator
  h_cell_t<HGrid>::FirstChild() const 
  { return ChildIterator(*this, base::TheHierGrid()->ThePatternGrid()->FirstCell());}

  template<class HGrid>
  inline  typename h_cell_t<HGrid>::ChildIterator
  h_cell_t<HGrid>::EndChild() const 
  { return ChildIterator(*this, base::TheHierGrid()->ThePatternGrid()->EndCell());}

  template<class ELEMBASE, class FLATELEM>
  inline h_element_child_iterator_t<typename ELEMBASE::grid_type, FLATELEM>
  h_element_t<ELEMBASE,FLATELEM>::FirstChild() const
  { return ChildIterator(*this);}

  template<class ELEMBASE, class FLATELEM>
  inline h_element_child_iterator_t<typename ELEMBASE::grid_type, FLATELEM>
  h_element_t<ELEMBASE,FLATELEM>::EndChild() const
  { return ChildIterator(*this, 
			 base::TheHierGrid()->children(*this), 
			 base::TheHierGrid()->children(*this)->template EndElement<dim>());}

  /*
  template<class HGRID, class FLATELEM>
  h_element_child_iterator_t<HGRID,FLATELEM>::
  h_element_child_iterator_t(typename h_element_child_iterator_t<HGRID,FLATELEM>::element_type const& pp) 
    : p(pp), ch(* TheHierGrid()->children(p)) {}
  */


  /*
  template<class ELEMBASE, class FLATELEM, int DIM, int CODIM>
  struct h_element_traits_base_aux {};

  template<class ELEMBASE, class FLATELEM, int CODIM>
  struct h_element_traits_base_aux<ELEMBASE, FLATELEM, 0, CODIM>
    : public element_traits_vertex_base<typename ELEMBASE::grid_type> 
  {};

  template<class ELEMBASE, class FLATELEM, int CODIM>
  struct h_element_traits_base_aux<ELEMBASE, FLATELEM, 1, CODIM>
    : public element_traits_edge_base<typename ELEMBASE::grid_type> 
  {};

  template<class ELEMBASE, class FLATELEM, int CODIM>
  struct h_element_traits_base_aux<ELEMBASE, FLATELEM, 2, CODIM>
    : public element_traits_face_base<typename ELEMBASE::grid_type> 
  {};

  template<class ELEMBASE, class FLATELEM, int DIM>
  struct h_element_traits_base_aux<ELEMBASE, FLATELEM, DIM, 0>
    : public element_traits_cell_base<typename ELEMBASE::grid_type> 
  {};

  template<class ELEMBASE, class FLATELEM, int DIM>
  struct h_element_traits_base_aux<ELEMBASE, FLATELEM, DIM, 1>
    : public element_traits_facet_base<typename ELEMBASE::grid_type> 
  {};
  

  template<class ELEMBASE, class FLATELEM>
  struct h_element_traits_base_aux<ELEMBASE, FLATELEM, 2, 0>
    : public element_traits_cell_base<typename ELEMBASE::grid_type> 
  {};

  template<class ELEMBASE, class FLATELEM>
  struct h_element_traits_base_aux<ELEMBASE, FLATELEM, 1, 0>
    : public element_traits_cell_base<typename ELEMBASE::grid_type> 
  {};

  template<class ELEMBASE, class FLATELEM>
  struct h_element_traits_base_aux<ELEMBASE, FLATELEM, 0, 0>
    : public element_traits_vertex_base<typename ELEMBASE::grid_type> 
  {};

  template<class ELEMBASE, class FLATELEM>
  struct h_element_traits_base_aux<ELEMBASE, FLATELEM, 0, 1>
    : public element_traits_vertex_base<typename ELEMBASE::grid_type> 
  {};

  template<class ELEMBASE, class FLATELEM>
  struct h_element_traits_base_aux<ELEMBASE, FLATELEM, 1, 1>
    : public element_traits_edge_base<typename ELEMBASE::grid_type> 
  {};



  template<class ELEMBASE, class FLATELEM>
  struct h_element_traits_base 
    : public h_element_traits_base_aux<ELEMBASE, FLATELEM, 
				       h_element_t<ELEMBASE,FLATELEM>::dim,
				       h_element_t<ELEMBASE,FLATELEM>::griddim> 
  {};
  */
} // namespace hierarchical


template<class ELEMBASE, class FLATELEM>
struct element_traits<hierarchical::h_element_t<ELEMBASE, FLATELEM> > 
//  : public hierarchical::h_element_traits_base<ELEMBASE, FLATELEM> {};
{
  typedef typename hierarchical::h_element_t<ELEMBASE, FLATELEM>::element_handle handle_type;
};

} // namespace GrAL 

#ifdef NMWR_INCLUDE_TEMPLATE_DEFS
#include "Gral/Hierarchical/hierarchical-grid.C"
#endif

#endif
