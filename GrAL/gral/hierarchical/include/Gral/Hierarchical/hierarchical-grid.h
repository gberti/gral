#ifndef GRAL_GB_HIERARCHICAL_GRID_H
#define GRAL_GB_HIERARCHICAL_GRID_H

// $LICENSE_NEC

#include "Gral/Base/common-grid-basics.h"
#include "Container/bivector.h"
#include "Utility/pre-post-conditions.h"
#include "Utility/const-ptr.h"

/*! \brief Support for hierarchical grids

    \author Guntram Berti
 */
namespace hierarchical {

  template<class HGrid> class h_cell;
  template<class HGrid> class h_cell_child_iterator;

  /*! \brief Handle type for hierarchical cells.

  */
  template<class HGrid> class h_cell_handle {
    typedef h_cell_handle<HGrid> self;
    typedef HGrid                                     hier_grid_type;
    typedef typename hier_grid_type::level_handle     level_handle;
    typedef typename hier_grid_type::flat_cell_handle flat_cell_handle;

    flat_cell_handle h;
    level_handle     lev;
  public:
    h_cell_handle() {}
    h_cell_handle(flat_cell_handle hh, level_handle llev) : h(hh), lev(llev) {}
    level_handle     level()       const { return lev;}
    flat_cell_handle flat_handle() const { return h;}
    
    bool operator==(self const& rhs) const { return h == rhs.h && lev == rhs.lev;}
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
  class hgrid_cartesian {
    typedef hgrid_cartesian<Grid> self;
  public:
    typedef  Grid  flat_grid_type;
    typedef  Grid  pattern_grid_type;
    typedef  GT    patterngt;
    typedef  int   level_handle;

    typedef typename patterngt::index_type pattern_index_type;

    typedef  typename GT::Cell        FlatCell;
    typedef  typename GT::cell_handle flat_cell_handle;
    typedef  typename GT::index_type  index_type;

    typedef h_cell_handle<self>          hier_cell_handle;
    typedef h_cell<self>                 HierCell;
    typedef h_cell_child_iterator<self>  CellChildIterator;

    // export type of hierarchical indexed table for dependent classes
    template<class T>
    struct table_type_template { typedef bivector<T> type;};
 
   typedef typename table_type_template<flat_grid_type>::type table_type;
    // typedef  bivector<flat_grid_type> table_type;
    typedef typename table_type::size_type size_type;

    // type for dependent classes (like grid functions) to inherit from
    // typedef  xxx   observer_type;
  private:
    pattern_grid_type      the_pattern;
    table_type             grids;
  public:
    hgrid_cartesian() {}
    hgrid_cartesian(flat_grid_type    const& root, // const_ptr<grid_type> root,
		    pattern_grid_type const& pat)
    {
      init(root,pat);
    }
    void init(flat_grid_type    const& root, 
	      pattern_grid_type const& pat)
    {
      grids.push_back(root);
      the_pattern = pat;
    }

    /*! \name Level navigation
     */
    //@{
    //! get the coarsest (root) level
    level_handle  coarsest_level() const { return level_handle(grids.begin_index());}
    //! get the finest level
    level_handle  finest_level()   const {  return level_handle(grids.back_index());}
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

    pattern_grid_type const& ThePatternGrid() const { return the_pattern;}

    flat_grid_type    const& operator()(level_handle lev) const { cv(lev); return grids(lev);}
    flat_grid_type    const& FlatGrid(level_handle lev) const { cv(lev); return grids(lev);}
    bool  empty() const { return num_of_levels()==0;}

    const_ptr<table_type> table() const { return const_ptr<table_type>(grids);}

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
      return grids.back_index();
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
      return grids.begin_index();
    }
    
    /*! 
      \pre <tt> num_of_levels() > 0 </tt>
    */
    void remove_finest_level() { grids.pop_back();}
    
    /*! 
      \pre <tt> num_of_levels() > 0 </tt>
    */
    void remove_coarsest_level() { grids.pop_front();}
    //@}

    bool valid(hier_cell_handle h) const { return valid(h.level()) && FlatGrid(h.level()).valid(h.flat_handle());} 
    bool valid(level_handle lev)   const { return coarsest_level() <= lev && lev <= finest_level();}
    void cv   (level_handle lev) const { REQUIRE( valid(lev), "lev=" << lev, 1); }

    index_type parent_cell_index(index_type ch, level_handle lev) const 
    { return quotient(ch,  the_pattern.cell_size());}
    HierCell  Parent(HierCell const& ch) const 
    {
      level_handle pl = next_coarser_level(ch.level());
      return HierCell(*this,FlatCell(FlatGrid(pl), parent_cell_index(ch.Flat().index(), ch.level())), pl); 
    }

    index_type child_cell_index(index_type parent, pattern_index_type loc_ch, level_handle parent_lev) const
    { return product(parent, the_pattern.cell_size()) + loc_ch; }
    HierCell Child(HierCell const& p, pattern_index_type loc) const 
    { 
      level_handle chl = next_finer_level(p.level());
      return HierCell(*this, FlatCell(FlatGrid(chl), child_cell_index(p.Flat().index(), loc, chl)), chl);
    }

    // hier_cell_handle parent(HierCell const& c) const { return quotient(c.Flat().index(), the_pattern.size());}
    // hier_cell_handle child(HCell const& c, pattern_cell_handle h) const;
  }; // class hgrid<Grid>



  //-------------------- Grid Elements and Iterators ------------------------


  template<class HGrid>
  class h_cell {
    typedef h_cell<HGrid> self;
    typedef HGrid    hier_grid_type;
    typedef typename HGrid::level_handle level_handle;
    typedef typename HGrid::FlatCell FlatCell;
    typedef typename HGrid::HierCell HierCell;

    typedef FlatCell                          flat_element_type;
    typedef typename HGrid::hier_cell_handle  hier_cell_handle;
    typedef typename HGrid::CellChildIterator ChildIterator;

    hier_grid_type const* g;
    hier_cell_handle      h;
  public:
    h_cell(hier_grid_type const& gg, hier_cell_handle hh) : g(&gg), h(hh) {}
    h_cell(hier_grid_type const& gg, FlatCell f, level_handle lev) : g(&gg), h(f.handle(), lev) {}

    FlatCell Flat() const { return FlatCell(g->FlatGrid(level()), h.flat_handle());}
    operator FlatCell() const { return Flat();}

    level_handle level() const { return h.level();}
    HierCell Parent    () { return g->Parent(*this);}
    HierCell ParentCell() { return g->Parent(*this);}

    hier_grid_type const& TheHGrid() const { return *g;}
    bool operator==(self const& rhs) const { return h == rhs.h;}
   
    inline ChildIterator FirstChild() const;
    inline ChildIterator EndChild  () const;

    // checking functions
    bool bound() const { return g != 0;}
    bool valid() const { return bound() && TheHGrid().valid(h);}
    void cb() const { REQUIRE(bound(), "", 1);}
    void cv() const { REQUIRE(valid(), "", 1);}
    
  }; // class h_cell<HGrid>




  template<class HGrid>
  class h_cell_child_iterator {
    typedef h_cell_child_iterator<HGrid> self;
  public:
    typedef HGrid    hier_grid_type;
    typedef typename HGrid::level_handle level_handle;
    typedef typename HGrid::FlatCell FlatCell;
    typedef typename HGrid::HierCell HierCell;
    typedef typename HGrid::patterngt::CellIterator PatternCellIterator;
    typedef typename HGrid::hier_cell_handle hier_cell_handle;

  private:
    HierCell             p;  // parent
    PatternCellIterator  ch; // current child
  public:
    h_cell_child_iterator(HierCell pp) : p(pp), ch(TheHGrid().ThePatternGrid().FirstCell()) {}
    h_cell_child_iterator(HierCell pp, PatternCellIterator cch) : p(pp), ch(cch) {}

    HierCell operator*()         const { cv(); return TheHGrid().Child(Parent(),(*ch).index());}
    self& operator++() { cv(); ++ch; return *this;}
    //    hier_cell_handle handle() const { return TheHGrid().child_cell(ch,p);}
    bool IsDone() { cb(); return ch.IsDone();}

    level_handle  level() const { return TheHGrid().next_finer_level(p.level());}
    FlatCell Flat() const { cv(); return operator*().Flat();} 

    HierCell const& Parent() const { cb(); return p;}
    hier_grid_type const& TheHGrid() const { cb(); return p.TheHGrid();}

    // checking functions
    bool bound() const { return p.bound();}
    bool valid() const { return bound() && ch.valid();}
    void cb() const { REQUIRE(bound(), "", 1);}
    void cv() const { REQUIRE(valid(), "", 1);}

    bool operator==(self const& rhs) const { return ch == rhs.ch;}

  }; // class h_cell_child_iterator<HGrid>



  //------------- inline functions -----------------
  
  template<class HGrid>
  inline  typename h_cell<HGrid>::ChildIterator
  h_cell<HGrid>::FirstChild() const 
  { return ChildIterator(*this, TheHGrid().ThePatternGrid().FirstCell());}

  template<class HGrid>
  inline  typename h_cell<HGrid>::ChildIterator
  h_cell<HGrid>::EndChild() const 
  { return ChildIterator(*this, TheHGrid().ThePatternGrid().EndCell());}

} // namespace hierarchical

#endif
