#ifndef GRAL_GB_HIERARCHICAL_OCTREE_C
#define GRAL_GB_HIERARCHICAL_OCTREE_C

// $LICENSE_NEC

#include "Gral/Hierarchical/octree.h"
#include "Utility/pre-post-conditions.h"


namespace octree {
  template<class Grid, class GT>
  Octree<Grid, GT>::Octree() {}

  template<class Grid, class GT>
  Octree<Grid, GT>::Octree(typename Octree<Grid, GT>::flat_grid_type const& C0,
			   typename Octree<Grid, GT>::pattern_grid_type   const& refpat)
  { init(C0,refpat);}
  template<class Grid, class GT>
  Octree<Grid, GT>::Octree(ref_ptr<typename Octree<Grid, GT>::flat_grid_type const> C0,
			   typename Octree<Grid, GT>::pattern_grid_type   const& refpat)
  { init(*C0,refpat);}

  template<class Grid, class GT>
  Octree<Grid, GT>::Octree(typename Octree<Grid, GT>::hier_grid_type const& H)
    : levels(H)
  {
   active_range.init(levels,false);
   active_range.set_default(false);
  }

 template<class Grid, class GT>
 void Octree<Grid, GT>::init(typename Octree<Grid, GT>::flat_grid_type const& C0,
			     typename Octree<Grid, GT>::pattern_grid_type   const& refpat)
 {
   levels.init(C0,refpat);
   active_range.init(levels,false);
   active_range.set_default(false);
 }

  template<class Grid, class GT>
  void Octree<Grid, GT>::join_cells(oct_cell_type const& newLeaf)
  {
    join_cells_rec(newLeaf);

    // remove possible empty fine levels.

    while(empty(finest_level())) {
      levels      .remove_finest_level();
      // active_range.remove_finest_level();
    }
  }

  template<class Grid, class GT>
  void Octree<Grid, GT>::join_cells_rec(oct_cell_type const& newLeaf)
  {
    if(! isLeaf(newLeaf)) {
      for(OctCellChildIterator ch(newLeaf.FirstChild()); !ch.IsDone(); ++ch) {
	join_cells_rec(*ch);
	deactivate(*ch); // could involve deactivating vertices ...
      }
    }
  }

  template<class Grid, class GT>
  void Octree<Grid, GT>::split_cell(oct_cell_type const& oldLeaf) 
  {
    if(isLeaf(oldLeaf)) {
      make_branch(oldLeaf);
      if(level(oldLeaf) == finest_level()) {
	levels      .add_finer_level();
	// active_range.add_finer_level(false); // by default, all cells are deactivated
	active_range[levels.finest_level()].set_default(false);
      }
      // use the more general H<XXX>ChildIterator, because there are no octree children yet
      for(HierCellChildIterator ch(oldLeaf.FirstChild()); !ch.IsDone(); ++ch) {
	make_leaf(*ch); // involves activiting ch
      }
    }
  }
  template<class Grid, class GT>
  void Octree<Grid, GT>::add_coarser_level() 
  {
    REQUIRE_ALWAYS( ! empty(), "", 1);
    levels.add_coarser_level();
    // all coarse cells deactivated by default
    // active_range.add_coarser_level(false);
    active_range[levels.coarsest_level()].set_default(false);
    // activate all cells having at least one active child
    for(flat_cell_iterator c(*LevelGrid(coarsest_level())); !c.IsDone(); ++c) {
      hier_cell_type hc(*TheHierGrid(), *c, coarsest_level());
      if(sequence::exists(hc.FirstChild(), hc.EndChild(), isActivePred(*this)))
	activate(hc);
    }
  }

  template<class Grid, class GT>
  typename Octree<Grid,GT>::oct_cell_type
  Octree<Grid,GT>::leaf_ancestor(typename Octree<Grid,GT>::oct_cell_type subLeaf) const
  {
    hier_cell_type c=subLeaf;
    while(! isActive(c)) {
      REQUIRE(level(c) > coarsest_level(), "", 1);
      c = c.Parent();
    }
    // This need not be the case, cf. definition of isLeaf()
    // ENSURE( isLeaf(c), "c=" << c.Flat().index() << " subLeaf=" << subLeaf.Flat().index(), 1);
    return c;
  }

  template<class Grid, class GT>
  typename Octree<Grid,GT>::size_type
  Octree<Grid,GT>::NumOfCells() const 
  {
    size_type n = 0;
    for(level_handle lev = coarsest_level(); lev <= finest_level(); ++lev)
      n += ActiveRange(lev)->NumOfCells();
    return n;
  }

  template<class Grid, class GT>
  typename Octree<Grid,GT>::size_type
  Octree<Grid,GT>::NumOfLeafCells() const 
  {
    size_type n = 0;
    LeafCellIterator c(*this);
    while(! c.IsDone()) {
      ++c;
      ++n;
    }
    return n;
  }



} // namespace octree

#endif
