#ifndef GRAL_GB_HIERARCHICAL_OCTREE_H
#define GRAL_GB_HIERARCHICAL_OCTREE_H

// $LICENSE_NEC


#include "Gral/Hierarchical/hierarchical-grid.h"
#include "Gral/Hierarchical/hierarchical-grid-table.h"
#include "Gral/Hierarchical/hierarchical-partial-grid-function.h"

#include "Utility/const-ptr.h"

#include "Container/sequence-algorithms.h"

#include <algorithm>  // find
#include <functional> // mem_fun_ref


/*! \brief Implementation of a generalized octree
    \author Guntram Berti 

    A generalized octree has as root subdivision which is not necessarily a single
    cube, but can be a Cartesian grid \f${\cal C}_0\f$. 
    In the special case that \f${\cal C}_0\f$ has only one cell, this reduces
    to a conventional octree.

    \note It is also possible to add coarser layers (refining the initial root layer) afterwards
*/

namespace octree {
  
 
  template<class PREDCLASS>
  struct isActive_pred {
    PREDCLASS const* pred;
    isActive_pred(PREDCLASS const& pr) : pred(&pr) {}
    template<class T>
    bool operator()(T const& t) const { return pred->isActive(t);}
  };

  template<class PREDCLASS>
  inline
  isActive_pred<PREDCLASS> isActivePred(PREDCLASS const& pr) { return isActive_pred<PREDCLASS>(pr);}


/*! \brief Generalized octree

    \author Guntram Berti

    A generalized octree need not have a single root cell, but may
    have a Cartesian <em> root grid </em> instead. 
    This root can be accessed by using the \c coarsest_level() member.
 */

  namespace hier = hierarchical;

template<class Grid, class GT = grid_types<Grid> >
class Octree {
  typedef Octree self;
public:
  typedef Grid                                     flat_grid_type;
  typedef GT                                       flatgt;
  typedef typename flatgt::index_type              index_type;
  typedef hierarchical::hgrid_cartesian<flat_grid_type>  hier_grid_type;

  typedef hier_grid_type                           hgt;
  typedef typename hgt::pattern_grid_type          pattern_grid_type;
  typedef typename hgt::level_handle               level_handle;
  typedef typename hgt::HierCell                   HierCell;
  typedef typename hgt::CellChildIterator          HierCellChildIterator;
  typedef typename flatgt::CellIterator            FlatCellIterator;

  //FIXME: these should be separate types instead of typedefs
  typedef HierCell              OctCell;
  typedef HierCellChildIterator OctCellChildIterator;

  typedef size_t size_type;

  //  typedef marked_subrange<cartesian_grid_type>     grid_range_type;
  //typedef hier_grid_table<active_grid_range_type>  subrange_table_type;

  // FIXME: partial_gf is no good impl. of a cell range. ElementIterator
  // is not guaranteed to traverse only Cells with value 'true'. (see deactivate(OctCell)).
  typedef hier::hier_partial_grid_function<HierCell, bool>  subrange_table_type;
  typedef typename subrange_table_type::flat_gf_type          grid_range_type;

  typedef typename grid_range_type::CellIterator ActiveRangeCellIterator;
private:  
  hier_grid_type      levels;
  subrange_table_type active_range;

  

  // Forbidden for the moment
  Octree(Octree const&);
  Octree& operator=(Octree const&);
public:
  /*! \name Constructors
   */
  //@{ 
  //! Construct empty octree
  Octree();
  //! Construct with initial root layer \c C0
  Octree(flat_grid_type const& C0,
	 pattern_grid_type   const& refpat);
  // Octree(int nx_, int ny_, int nz_);

  /*! \brief Add the initial level.
      All  cells are active by default (ensuring that everything is covered by active cells)
   */
  void init(flat_grid_type const& C0,
	    pattern_grid_type   const& refpat);
  //@}


  const_ptr<hier_grid_type>     TheHGrid() const { return const_ptr<hier_grid_type>(&levels);}
  const_ptr<flat_grid_type>     LevelGrid(level_handle lev) const 
  { return const_ptr<flat_grid_type>(&(levels(lev)));}
  const_ptr<grid_range_type>    ActiveRange(level_handle lev) const
  { return const_ptr<grid_range_type>(&(active_range(lev)));}

  /*! \name Modifying operations
   */
  //@{
  /*! \brief join the cells (i.e. subtree) beneath \c newLeaf, such that \newLeaf becomes a leaf.
      
      \pre  <tt> is_leaf(c) == false </tt>
      \post <tt> is_leaf(c) == true  </tt> 
  */
  void join_cells(OctCell const& newLeaf);

  /*! \brief split the leaf cell \c oldLeaf, and add new layer if necessary.

    Inverse to join_cells().
   
     \pre <tt> is_leaf(c) == true </tt>
     \invariant <tt> split_cell(c); join_cells(c) </tt> does not change the state of the octree.
     
   */ 

  void split_cell(OctCell const& oldLeaf); 

  /*! \brief add a new coarser level
      \pre the current coarsest level can be coarsened with the refinement pattern
   */
  void add_coarser_level(); 

  //@}


  /*! \name Operations on OctCells
   */
  //@{
  /*! \brief true if \c is part of the octree
   */
  bool isActive(HierCell const& c) const { return active_range(c.level())(c);}
  /*! A cell is a leaf if there are no sons, that is, 
    all child cells are inactive.
 
    \note If only some of the children are inactive, the cell is not a leaf.
    */
  bool isLeaf(HierCell const& c)   const 
    { 
      return isActive(c) 
	&& (c.level() == finest_level() 
	    ||  ! sequence::exists(c.FirstChild(), c.EndChild(), isActivePred(*this)));
    }
  /*! \brief True if \c is active but not a leaf.
  */
  bool isBranch(HierCell const& c) const
  {
    return isActive(c) && ! isLeaf(c);
  }

  //! return the level of \c c
  level_handle  level(HierCell c)   const { return c.level();}
  //! return the youngest ancestor of \c subLeaf which is in the octree (i.e. is a leaf).
  OctCell leaf_ancestor(HierCell subLeaf) const;
  //@}

  /*! \name Level navigation
   */
  //@{
  //! get the coarsest (root) level
  level_handle  coarsest_level()   const { return TheHGrid()->coarsest_level();}
  //! get the finest level
  level_handle  finest_level() const { return TheHGrid()->finest_level();}
  /*! \brief get handle of next finer level
      \pre <tt> lev != finest_level() </tt>
   */
  level_handle  next_finer_level  (level_handle lev) const { return TheHGrid()->next_finer_level(lev);}
  /*! \brief  get handle of next coarser level
      \pre <tt> lev != coarsest_level() </tt>
  */
  level_handle  next_coarser_level(level_handle lev) const { return TheHGrid()->next_coarser_level(lev);}
  //! get the number of levels
  size_type     num_of_levels() const { return levels.num_of_levels();}
  //@}

  /*! \brief Check whether to octree is empty.
     To fill an empty octree, one has to use the \c init() member, and
     subsequent calls to \c split_cell() / \c join_cells().
   */
  bool          empty()         const { return num_of_levels()==0;}

  /*! \brief true if level \c lev does not contain active cells.
      This means that also all finer levels are empty.
   */
  bool empty(level_handle lev) const { return active_range(lev).empty();}


  bool valid(level_handle lev) const { return (levels.coarsest_level() <= lev && lev <= levels.finest_level());}
  void cv(level_handle lev) const 
  { REQUIRE(valid(lev), "lev=" << lev, 1); }
private:

  /*! \brief mark the cell as active
   */
  void activate (OctCell c) { active_range[c.level()][c] = true;}
  /*! \brief unmark the cell as active
      \todo This works only if the default value is false!
   */
  void deactivate (OctCell c) { active_range[c.level()].undefine(c); }
  //! mark the cell as active and leaf
  void make_leaf  (OctCell c) { active_range[c.level()][c] = true;}
  //! mark the cell as an internal octcell (active, but no leaf)
  void make_branch(OctCell c) { active_range[c.level()][c] = true;}

  void join_cells_rec(OctCell const& newLeaf);

}; // class Octree



} // namespace octree


#ifdef NMWR_INCLUDE_TEMPLATE_DEFS
#include "Gral/Hierarchical/octree.C"
#endif

#endif
