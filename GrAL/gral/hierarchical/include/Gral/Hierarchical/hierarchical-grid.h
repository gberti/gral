#ifndef GRAL_GB_HIERARCHICAL_GRID_H
#define GRAL_GB_HIERARCHICAL_GRID_H

// $LICENSE_NEC

#include "Gral/Base/common-grid-basics.h"
#include "Container/bivector.h"

#include "Utility/pre-post-conditions.h"
#include "Utility/ref-ptr.h"
#include "Utility/notifier.h"

/*! \brief Support for hierarchical grids

    \author Guntram Berti
 */
namespace hierarchical {

  template<class HGrid> class h_element_base_t;
  template<class ELEMENTBASE> class h_vertex_t;
  template<class ELEMENTBASE> class h_cell_t;
  template<class HGrid> class h_cell_child_iterator_t;

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

    typedef typename patterngt::index_type pattern_index_type;

    typedef  typename flatgt::Cell          flat_cell_type;  
    typedef  typename flatgt::Vertex        flat_vertex_type;  
    typedef  typename flatgt::cell_handle   flat_cell_handle;
    typedef  typename flatgt::vertex_handle flat_vertex_handle;
    typedef  typename flatgt::index_type    index_type;

    typedef typename flatgt::archetype_type     archetype_type;
    typedef typename flatgt::archetype_handle   archetype_handle;
    typedef typename flatgt::archetype_iterator archetype_iterator;
    typedef grid_types<archetype_type>          archgt;               


    typedef h_element_base_t<self>                        element_base_type;
    typedef h_element_handle_t<self, flat_vertex_handle>  hier_vertex_handle;
    typedef h_element_handle_t<self, flat_cell_handle>    hier_cell_handle;
    typedef h_cell_t  <element_base_type>                 hier_cell_type;
    typedef h_vertex_t<element_base_type>                 hier_vertex_type;

    typedef hier_vertex_handle vertex_handle;
    typedef hier_cell_handle   cell_handle;
    typedef hier_vertex_type         Vertex;
    typedef hier_cell_type           Cell;

    typedef h_cell_child_iterator_t<self>  CellChildIterator;

    // export associated templates
    template<class HG, class FLAT_HANDLE>
    struct element_handle_template { typedef h_element_handle_t<HG, FLAT_HANDLE> type;};
    template<class ELEMENT_BASE>
    struct vertex_template { typedef h_vertex_t<ELEMENT_BASE> type;};
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
    hgrid_cartesian(flat_grid_type    const& root, // ref_ptr<const grid_type> root,
		    pattern_grid_type const& pat)
    {
      init(root,pat);
    }
    void init(flat_grid_type    const& root, 
	      pattern_grid_type const& pat)
    {
      grids.push_back(root);
      the_pattern = pat;
      for(observer_iterator ob = begin_observer(); ob != end_observer(); ++ob)
	dynamic_cast<observer_type*>(*ob)->notifier_assigned(this);

    }
    self& operator=(self const& rhs) {
      if(this != &rhs) {
	before_assignment();
	grids = rhs.grids;
	after_assignment();
      }
      return *this;
    }

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

    ref_ptr<pattern_grid_type const> ThePatternGrid() const 
    {  return ref_ptr<pattern_grid_type const>(the_pattern);}

    ref_ptr<flat_grid_type const> operator()(level_handle lev) const { return FlatGrid(lev);}
    ref_ptr<flat_grid_type const> FlatGrid(level_handle lev) const   
    { cv(lev); return  ref_ptr<flat_grid_type const>(grids(lev));}
 
    bool  empty() const { return num_of_levels()==0;}
    
    ref_ptr<const table_type> table() const { return ref_ptr<table_type const>(grids);}

    /*! \name Modifying operations
     */
    //@{
    /*! \brief Add a finer level.
      
    \pre There is at least one level: <tt> num_of_levels() > 0 </tt>
    \post <tt> level_handle lev = add_finer_level(); lev == finest_level(); </tt>
    \post No cells are active in the newly created level level.
    */
    level_handle add_finer_level  () {
      REQUIRE_ALWAYS( (! empty()), "",1);
      grids.push_back(flat_grid_type(index_type(1) + product(grids.back().cell_size(), the_pattern.cell_size())));
      for(observer_iterator ob = begin_observer(); ob != end_observer(); ++ob)
	dynamic_cast<observer_type*>(*ob)->hgrid_level_added(this,finest_level());
      return finest_level();
    }

    /*! \brief add a coarser level
       
         \pre <tt> num_of_levels() > 0 </tt>
	 \pre The number of cells in each direction of \c coarsest_level() is dividable by 
              the corresponding number of cells in pattern.

    */
    level_handle add_coarser_level() { 
      REQUIRE_ALWAYS( (! empty()), "",1);
      REQUIRE_ALWAYS( does_divide(the_pattern.cell_size(), grids.front().cell_size()), "",1); 
      grids.push_front(flat_grid_type(index_type(1) + quotient(grids.front().cell_size(), the_pattern.cell_size())));
      for(observer_iterator ob = begin_observer(); ob != end_observer(); ++ob)
	dynamic_cast<observer_type*>(*ob)->hgrid_level_added(this,coarsest_level());
      return coarsest_level();
    }
    
    /*! 
      \pre <tt> num_of_levels() > 0 </tt>
    */
    void remove_finest_level()   { 
      ce();
      level_handle oldlev = finest_level();
      grids.pop_back(); 
      for(observer_iterator ob = begin_observer(); ob != end_observer(); ++ob)
	dynamic_cast<observer_type*>(*ob)->hgrid_level_removed(this,oldlev);
    }
    
    /*! 
      \pre <tt> num_of_levels() > 0 </tt>
    */
    void remove_coarsest_level() { 
      ce(); 
      level_handle oldlev = coarsest_level();
      grids.pop_front();
      for(observer_iterator ob = begin_observer(); ob != end_observer(); ++ob)
	dynamic_cast<observer_type*>(*ob)->hgrid_level_removed(this,oldlev);

    }
    //@}

    template<class HG, class flat_handle_type>
    bool valid(h_element_handle_t<HG,flat_handle_type> h) const 
    { return valid(h.level()) && FlatGrid(h.level())->valid(h.flat_handle());} 
    bool valid(hier_cell_handle   h) const { return valid(h.level()) && FlatGrid(h.level())->valid(h.flat_handle());} 
    bool valid(level_handle lev)   const { return coarsest_level() <= lev && lev <= finest_level();}
    void cv   (level_handle lev) const { REQUIRE( valid(lev), "lev=" << lev, 1); }
    bool valid() const { return !empty();}
    void cv   () const { REQUIRE(valid(), "", 1);}
    void ce   () const { REQUIRE( (!empty()), "", 1);}

    index_type parent_cell_index(index_type ch, level_handle lev) const 
    { cv(lev); cv(lev-1); return quotient(ch,  the_pattern.cell_size());}
    hier_cell_type  Parent(hier_cell_type const& ch) const 
    {
      cv(ch.level()); 
      level_handle pl = next_coarser_level(ch.level());
      return hier_cell_type(*this,flat_cell_type(* FlatGrid(pl), parent_cell_index(ch.Flat().index(), ch.level())), pl); 
    }

    index_type child_cell_index(index_type parent, pattern_index_type loc_ch, level_handle parent_lev) const
    { cv(parent_lev); return product(parent, the_pattern.cell_size()) + loc_ch; }

    hier_cell_type Child(hier_cell_type const& p, pattern_index_type loc) const 
    { 
      level_handle chl = next_finer_level(p.level());
      return hier_cell_type(*this, flat_cell_type(* FlatGrid(chl), child_cell_index(p.Flat().index(), loc, chl)), chl);
    }

    template<class HVERTEX>
    HVERTEX CoarsestParent(HVERTEX const& v) const
    { 
      return HVERTEX(v.TheGrid(),coarsest_parent(v));
    } 

    template<class HVERTEX>
    //std::pair<flat_vertex_handle, level_handle> 
    typename HVERTEX::vertex_handle
    coarsest_parent(HVERTEX const& v) const {
      index_type pat0  = the_pattern.cell_size(); //vertex_size();
      index_type pat   = pat0;
      index_type vidx = v.Flat().index();
      int level_diff = 0;
      while(v.level() - level_diff > coarsest_level() && does_divide(pat, vidx)) {
	++level_diff;
	pat = product(pat, pat0);
      }
      pat = quotient(pat, pat0); // pat = pat0^level_diff
 
      level_handle  newlev = v.level() - level_diff;
      return  typename HVERTEX::vertex_handle(FlatGrid(newlev)->get_vertex_handle(quotient(vidx,pat)), newlev); 
    }

    // hier_cell_handle parent(hier_cell_type const& c) const { return quotient(c.Flat().index(), the_pattern.size());}
    // hier_cell_handle child(HCell const& c, pattern_cell_handle h) const;
  }; // class hgrid_cartesian<Grid>



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
    h_element_base_t(grid_type const* hgg, level_handle l = hgg->finest_level())
      : hg(hgg), lev(l)  {}
    h_element_base_t(ref_ptr<const grid_type> hgg, level_handle l = hgg->finest_level())
      : hg(hgg), lev(l)  {}

  protected:
    void set(level_handle l) { lev = l;}
  public:
    bool bound() const { return hg != 0;}
    void cb()    const { REQUIRE(bound(), "", 1); }
    level_handle level() const { return lev;}

    ref_ptr<const hier_grid_type> TheHierGrid() const { return hg;}
    ref_ptr<const flat_grid_type> TheFlatGrid() const { return hg->FlatGrid(lev);}
    // ref_ptr<const grid_type>      TheGrid() const { return TheHierGrid();}
    const grid_type&              TheGrid() const { return *TheHierGrid();}

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

    typedef flat_vertex_type                     flat_element_type;
  private:
    flat_vertex_handle       h;
  public:
    h_vertex_t() {}
    h_vertex_t(grid_type const& gg, vertex_handle hh) : base(&gg, hh.level()), h(hh.flat_handle()) {}
    h_vertex_t(grid_type const& gg, flat_vertex_type   f, level_handle lev) : base(&gg,lev), h(f.handle()) {}
    h_vertex_t(grid_type const& gg, flat_vertex_handle f, level_handle lev) : base(&gg,lev), h(f) {}
    h_vertex_t(ref_ptr<grid_type const> gg, vertex_handle hh) : base(gg, hh.level()), h(hh.flat_handle()) {}
    h_vertex_t(ref_ptr<grid_type const> gg, flat_vertex_type   f, level_handle lev) : base(gg,lev), h(f.handle()) {}
    h_vertex_t(ref_ptr<grid_type const> gg, flat_vertex_handle f, level_handle lev) : base(gg,lev), h(f) {}

    // construct from 'sibling' using another ELEMBASE type.
    // Can only upcast: ELEMBASE must know about EBASE2
    template<class EBASE2>
    h_vertex_t(h_vertex_t<EBASE2> const& s) : base(s.Grid(tp<grid_type>()), s.level()), h(s.flat_handle()) {}

    // can only 'downcast' :ELEMBASE must know about EBASE2
    template<class EBASE2>
    h_vertex_t<EBASE2> to() const { cv(); return h_vertex_t<EBASE2>(Grid(tp<typename EBASE2::grid_type>()), level(), h);}

  protected:
    void set(vertex_handle newh) { set(newh.flat_handle(), newh.level());}
    void set(flat_vertex_handle newfh, level_handle newlev) { base::set(newlev); h = newfh;}
  public:
    vertex_handle handle() const { cv(); return vertex_handle(h,level());}

    flat_vertex_handle flat_handle() const { cv(); return h;}
    flat_vertex_type Flat() const { cv(); return flat_vertex_type(* TheFlatGrid(),h);}
    operator flat_vertex_type() const { return Flat();}

    // level_handle natural_level() const { return TheHierGrid()->natural_level(*this);}

    // hier_vertex_type Parent () { return TheHierGrid()->Parent(*this);}
    // unsigned dim_of_parent() const { return TheHierGrid()->dim_of_parent(*this);}
    // hier_cell_type   ParentCell() { return g->Parent(*this);}
    // ParentCellIterator   FirstParentCell() { return g->Parent(*this);}

    friend bool operator==(self const& lhs, self const& rhs) { lhs.cv(); rhs.cv(); return lhs.h == rhs.h;}
    friend bool operator!=(self const& lhs, self const& rhs) { lhs.cv(); rhs.cv(); return lhs.h != rhs.h;}
   
    // inline ChildIterator FirstChild() const;
    // inline ChildIterator EndChild  () const;

    // checking functions
    bool valid() const { return bound() && TheHierGrid()->valid(vertex_handle(h,level()));}
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
    typedef typename base::gt              gt;
    typedef typename base::grid_type       grid_type;
    typedef typename base::level_handle    level_handle;
    typedef typename gt::Cell              Cell;
    typedef typename gt::cell_handle       cell_handle;
    typedef typename gt::Vertex            Vertex;
    typedef typename gt::vertex_handle     vertex_handle;
    typedef typename gt::archgt            archgt;
    typedef typename archgt::Vertex        archVertex;

    typedef typename base::flatgt          flatgt;
    typedef typename flatgt::cell_handle   flat_cell_handle;
    typedef typename flatgt::Cell          flat_cell_type;
    typedef flat_cell_type                       flat_element_type;

    typedef typename gt::CellChildIterator ChildIterator;

    flat_cell_handle         h;
  public:
    h_cell_t() {}
    h_cell_t(grid_type const& gg, cell_handle hh) : base(&gg, hh.level()), h(hh.flat_handle()) {}
    h_cell_t(grid_type const& gg, flat_cell_type   f, level_handle lev) : base(&gg,lev), h(f.handle()) {}
    h_cell_t(grid_type const& gg, flat_cell_handle f, level_handle lev) : base(&gg,lev), h(f) {}
    h_cell_t(ref_ptr<grid_type const> gg, cell_handle hh) : base(gg, hh.level()), h(hh.flat_handle()) {}
    h_cell_t(ref_ptr<grid_type const> gg, flat_cell_type   f, level_handle lev) : base(gg,lev), h(f.handle()) {}
    h_cell_t(ref_ptr<grid_type const> gg, flat_cell_handle f, level_handle lev) : base(gg,lev), h(f) {}
 
    // construct from 'sibling' using another ELEMBASE type
    // Can only 'upcast':  EBASE2 must 'know' about ELEMENTBASE
    template<class EBASE2>
    h_cell_t(h_cell_t<EBASE2> const& s) : base(s.Grid(tp<grid_type>()), s.level()), h(s.flat_handle()) {}

    // can only 'downcast' :ELEMBASE must know about EBASE2
    template<class EBASE2>
    h_cell_t<EBASE2> to() const { cv(); return h_cell_t<EBASE2>(Grid(tp<typename EBASE2::grid_type>()), level(), h);}

    cell_handle handle() const { cv(); return cell_handle(h,level());}

    flat_cell_handle flat_handle() const { cv();  return h;}
    flat_cell_type Flat() const { cv(); return flat_cell_type(* TheFlatGrid(), flat_handle());}
    operator flat_cell_type() const { cv(); return Flat();}

    Vertex        V(archVertex const& lV) const { return Vertex(TheGrid(), v(lV));}
    vertex_handle v(archVertex const& lV) const { return vertex_handle(Flat().v(lV), level());}

    // Does not work for conforming leaf grids, but these should pobably not use this class anyhow.
    size_t NumOfVertices() const { return Flat().NumOfVertices();}
    // will this work with ELEMENTBASE other than h_element_base_t<> ?
    Cell Parent    () { cv(); return Cell(TheHierGrid()->Parent(*this));}
    Cell ParentCell() { cv(); return Cell(TheHierGrid()->Parent(*this));}

    friend bool operator==(self const& lhs, self const& rhs) { lhs.cv(); rhs.cv(); return lhs.h == rhs.h;}
    friend bool operator!=(self const& lhs, self const& rhs) { lhs.cv(); rhs.cv(); return lhs.h != rhs.h;}

    inline ChildIterator FirstChild() const;
    inline ChildIterator EndChild  () const;

    // checking functions
    bool valid() const { return bound() && TheHierGrid()->valid(cell_handle(h, level()));}
    void cv() const { REQUIRE(valid(), "", 1);}
  }; // class h_cell_t<ELEMENTBASE>




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

    friend bool operator==(self const& lhs, self const& rhs)  { lhs.cb(); rhs.cb(); return lhs.ch == rhs.ch;}
    friend bool operator!=(self const& lhs, self const& rhs)  { lhs.cb(); rhs.cb(); return lhs.ch != rhs.ch;}

  }; // class h_cell_child_iterator_t<HGrid>



  template<class ELEMBASE> 
  class h_vertex_on_cell_iterator_t : public ELEMBASE {
    typedef  h_vertex_on_cell_iterator_t<ELEMBASE> self;
    typedef  ELEMBASE                              base;
  public:
    typedef typename base::grid_type     grid_type;
    typedef typename base::gt            gt;
    typedef typename gt::Vertex          Vertex;
    typedef typename gt::Cell            Cell;
    typedef typename gt::vertex_handle   vertex_handle;

    typedef Cell                         anchor_type;
    typedef Vertex                       value_type;
    typedef Vertex                       element_type;

    typedef typename gt::flatgt                   flatgt;
    typedef typename flatgt::VertexOnCellIterator flat_vertex_on_cell_iterator;


    h_vertex_on_cell_iterator_t() {}
    explicit h_vertex_on_cell_iterator_t(Cell const& cc)
      : base(cc),  vc(cc.Flat().FirstVertex()) {}

    self&   operator++()       { cv(); ++vc; return  (*this);}
    Vertex  operator*()  const { cv(); return Vertex(TheGrid(), handle());}
    bool    IsDone()     const { cb(); return vc.IsDone();}
  
    vertex_handle   handle() const { cv(); return vertex_handle(vc.handle(), level()); }
  
    temporary<Cell> TheCell()     const { cb(); return temporary<Cell>(Cell(TheGrid(), vc.TheCell(), level())) ;}
    temporary<Cell> TheAnchor()   const { cb(); return TheCell(); }
  
    friend bool operator==(self const& lhs, self const& rhs) { lhs.cb(); rhs.cb(); return lhs.vc == rhs.vc;}
    friend bool operator!=(self const& lhs, self const& rhs) { return !(lhs == rhs);}
  
    bool valid() const { return bound() && vc.valid();}
    void cv()    const { REQUIRE(valid(), "", 1);}
  private:
    flat_vertex_on_cell_iterator  vc;
  };



  //------------- inline functions -----------------
  
  template<class HGrid>
  inline  typename h_cell_t<HGrid>::ChildIterator
  h_cell_t<HGrid>::FirstChild() const 
  { return ChildIterator(*this, TheHierGrid()->ThePatternGrid()->FirstCell());}

  template<class HGrid>
  inline  typename h_cell_t<HGrid>::ChildIterator
  h_cell_t<HGrid>::EndChild() const 
  { return ChildIterator(*this, TheHierGrid()->ThePatternGrid()->EndCell());}

} // namespace hierarchical

#endif
