#ifndef GRAL_GB_HIERARCHICAL_REFINE_OCTREE_CELL_TOWARDS_C
#define GRAL_GB_HIERARCHICAL_REFINE_OCTREE_CELL_TOWARDS_C

// $LICENSE_NEC

#include <stack>

/*! \addtogroup octreealgorithms Octree Algorithms
   
    Algorithms operating on octrees.     
*/

namespace GrAL {

/*! \brief Refine an octree cell towards a descendent cell

    \author Guntram Berti    
    \ingroup  octreealgorithms

    The cell \c Leaf of Octree \c tree is refined \c nl times towards the cell \c subLeaf,
    that is, we refine always that descendent of \c Leaf which contains \c subLeaf.

    \pre <tt> Tree.isLeaf(Leaf) == true </tt>
    \pre <tt> Tree.leaf_ancestor(tree, subLeaf) == Leaf </tt>
    \pre <tt> nl <=  subLeaf.level() - Leaf.level()  </tt> 

*/
template<class OCTREE, class OCTCELL> // = typename OCTREE::oct_cell_type>
void refine_towards(OCTREE & Tree, OCTCELL const& Leaf, OCTCELL const& subLeaf, int nl)
{
  REQUIRE(Tree.leaf_ancestor(subLeaf) == Leaf, "subLeaf is not a descendant of Leaf", 1);
  int k = subLeaf.level() - Leaf.level(); 
  REQUIRE(k >= (int)nl, "",1);
  // Leaf = parent^k(subLeaf)
  // we need the nl oldest ancestors of subLeaf:  
  // parent^k(subLeaf) = Leaf, ..., parent^{k-nl+1}(subLeaf);
  typename OCTREE::hier_cell_type p = subLeaf;
  // skip the first k-nl ancestors 
  for(int i = 1; i <= k-nl; ++i)
    p = p.Parent(); // p = parent^i(subLeaf)
  // now p = parent^{k-nl}(subLeaf)
  std::stack<typename OCTREE::hier_cell_type> ancestors;
  for(int i = k-nl+1; i <= k; ++i) {
    p = p.Parent(); // p = parent^i(subLeaf)
    ancestors.push(p);
  }
  REQUIRE( (p == Leaf), "p=" << p.level() << ":" << p.Flat().index()
	   << " Leaf=" << Leaf.level() << ":" << Leaf.Flat().index(), 1);
  while(!ancestors.empty()) {
    Tree.split_cell(ancestors.top());
    ancestors.pop();
  }

}

} // namespace GrAL 

#endif
