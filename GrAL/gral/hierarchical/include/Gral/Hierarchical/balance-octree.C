#ifndef GRAL_GB_BALANCE_OCTREE_C
#define GRAL_GB_BALANCE_OCTREE_C

// $LICENSE_NEC 

#include "Gral/Hierarchical/refine-octree-cell-towards.C"

/*! \addtogroup octreealgorithms Octree Algorithms
   
    Algorithms operating on octrees.     
*/

/*! \brief Balance an octree

    \author Guntram Berti    
    \ingroup  octreealgorithms

    \todo Add argument to balance only sub-octrees
    \todo Inbalance across edges in 3D do not get considered.
       Should this be an additional option? What should be considered a neighbor of a cell?
*/

template<class OCTREE>
void balance_octree(OCTREE& Tree, unsigned balance)
{
  typedef OCTREE oct;
  typename oct::level_handle k = Tree.finest_level();
  while(k >= Tree.coarsest_level()) {
    for(typename oct::ActiveLevelCellIterator c = Tree.ActiveRange(k)->FirstCell(); ! c.IsDone(); ++c) {
      typename oct::hier_cell_type hc(Tree.TheHierGrid(), *c, k);
      if(Tree.isLeaf(hc)) {
	typedef typename oct::flatgt flatgt;
	// FIXME: need also neighbors across edges!
	for(typename flatgt::CellOnCellIterator nc(*c); ! nc.IsDone(); ++nc) {
	  typename oct::hier_cell_type hnc(Tree.TheHierGrid(), *nc, k);
	  if(! Tree.isActive(hnc)) {
	    typename oct::hier_cell_type pnc = Tree.leaf_ancestor(hnc);
	    int level_diff = hnc.level() - pnc.level();
	    if(level_diff > balance)
	      refine_towards(Tree, pnc, hnc, level_diff - balance);
	  }
	}
      }
    }
    --k;
  } // while(k >= Tree.coarsest_level())
}

#endif
