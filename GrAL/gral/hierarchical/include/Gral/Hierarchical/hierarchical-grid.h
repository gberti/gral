#ifndef GRAL_GB_HIERARCHICAL_GRID_H
#define GRAL_GB_HIERARCHICAL_GRID_H

// $LICENSE_NEC

#include "Gral/Base/common-grid-basics.h"
#include "Container/bivector.h"
#include "Utility/pre-post-conditions.h"

/*! \brief Support for hierarchical grids

    \author Guntram Berti
 */
namespace hierarchical {

  template<class HGrid> class h_cell;
  template<class HGrid> class h_cell_child_iterator;

  /*! \brief Handle type for hierarchical cells.

  */
  template<class HGrid> class h_cell_handle {
    typedef HGrid h_grid_type;
    typedef typename h_grid_type::level_handle     level_handle;
    typedef typename h_grid_type::flat_cell_handle flat_cell_handle;

    flat_cell_handle h;
    level_handle lev;
  public:
    h_cell_handle() {}
    h_cell_handle(flat_cell_handle hh, level_handle llev) : h(hh), lev(llev) {}
    level_handle     level()       const { return lev;}
    flat_cell_handle flat_handle() const { return h;}
  };

  /*! \brief Hierarchical Cartesian grid

      \note
      For unstructured grid, we would need different modifying methods
      (adding a finer or coarser layer requires explicit specification of the new layer
       and its hierarchical relationship to the adjacent pre-existing layer).
       The interface could be unified by always passing a refiner/coarsener when
       adding a new layer.     

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
    typedef  int   level_handle;

    typedef  typename GT::Cell        FlatCell;
    typedef  typename GT::cell_handle flat_cell_handle;
    typedef  typename GT::index_type  index_type;

    typedef  h_cell_handle<self>  hier_cell_handle;
    typedef  h_cell<self>         HierCell;

    typedef typename bivector<flat_grid_type>::size_type size_type;
    // type for dependent classes (like grid functions) to inherit from
    // typedef  xxx   observer_type;
  private:
    pattern_grid_type         the_pattern;
    bivector<flat_grid_type>  grids;
  public:
    hgrid_cartesian();
    hgrid_cartesian(flat_grid_type    const& root, // const_ptr<grid_type> root,
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
    size_type     num_of_levels() const { return levels.size();}
    //@}
    
    flat_grid_type const& FlatGrid(level_handle lev) const { cv(lev); return grids(lev);}
    bool  empty() const { return num_of_levels()==0;}



    /*! \name Modifying operations
     */
    //@{
    /*! \brief Add a finer level.
      
    \pre There is at least one level 
    \post <tt> level_handle lev = add_finer_level(); lev == finest_level(); </tt>
    \post No cells are active in the newly created level level.
    */
    level_handle add_finer_level  () {
      grids.push_back(flat_grid_type(index_type(1) + product(grids.back().cell_size(), the_pattern.cell_size())));
      return grids.back_index();
    }

    /*! \brief add a coarser level
      
    \pre The number of cells in each direction of \c coarsest_level() is even.
    \post All cells of the new coarsest level are active.
    */
    level_handle add_coarser_level() { 
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

    void cv(level_handle lev) const 
    { REQUIRE((grids.begin_index() <= lev && lev < grids.end_index()), "lev=" << lev, 1); }

    // index_type parent_cell_index(index_type);
    HierCell  Parent(HierCell const& ch) const 
    { return HierCell(*this, 
		      FlatCell(FlatGrid(next_coarser_level(ch.level())), 
			       quotient(ch.Flat().index(), the_pattern.cell_size())), 
		      next_coarser_level(ch.level()));
    }
    // hier_cell_handle parent(HierCell const& c) const { return quotient(c.Flat().index(), the_pattern.size());}
    // hier_cell_handle child(HCell const& c, pattern_cell_handle h) const;
  }; // class hgrid


  template<class HGrid>
  class h_cell {
    typedef h_cell<HGrid> self;
    typedef HGrid    hier_grid_type;
    typedef typename HGrid::level_handle level_handle;
    typedef typename HGrid::FlatCell FlatCell;
    typedef typename HGrid::HierCell HierCell;

    typedef typename HGrid::hier_cell_handle hier_cell_handle;

    hier_grid_type const* g;
    hier_cell_handle      h;
  public:
    h_cell(hier_grid_type const& gg, hier_cell_handle hh) : g(&gg), h(hh) {}
    h_cell(hier_grid_type const& gg, FlatCell f, level_handle lev) : g(&gg), h(f.handle(), lev) {}

    FlatCell Flat() const { return FlatCell(g->FlatGrid(level()), h.flat_handle());}
    
    level_handle level() const { return h.level();}
    HierCell ParentCell() { return g->Parent(*this);}
    /*
    ChildIterator FirstChild() const;
    ChildIterator EndChild  () const;
    */ 
  };

  /*
  template<class Grid>
  class h_cell_child_iterator {
  private:
    pattern_cell_handle ch; // current child
    HCell               p;  // parent
  public:
    
    HCell operator*()      const { return HCell(g,handle());} 
    h_cell_handle handle() const { return TheHGrid().child_cell(ch,p);}
    // index_type index() const { return product(c.index(), pattern.size()) + pattern_index(h);}
  };
  */

} // namespace hierarchical

#endif
