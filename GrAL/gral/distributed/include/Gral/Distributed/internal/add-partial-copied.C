#ifndef NMWR_GB_ADD_PARTIAL_COPIED_C
#define NMWR_GB_ADD_PARTIAL_COPIED_C


// $LICENSE

#include "Gral/Distributed/internal/add-partial-copied.h"
#include "Gral/Partitioning/partitioning.h"

template<class Overlap,     class Partition, 
         class Part2Cell,
         class VertexRange, class CellRange,
         class VtxCorr,     class CellCorr>
void AddPartialRanges(Overlap          &    ovrlp, // out
		      Partition   const&    Prtng,
		      int                   p,
		      Part2Cell   const&    P2C,
		      VertexRange const&    copied_v,
		      CellRange   const&    copied_c,
		      VtxCorr     const&    vtx_corr,
		      CellCorr    const&    cell_corr)
{

  //-------- make synchronous range pairs : local copied -> remote exposed -----
  
  //  for(int p = 0 ; p <= Prtng.NumOfPartitions(); ++p) {
  typename Part2Cell::result_type P(P2C(p));
    
    //---- (I)  cell ranges -------
    
    typedef typename CellRange::ElementIterator    RgeCellIterator;
    for(RgeCellIterator c = copied_c.FirstElement(); !c.IsDone(); ++c) {
      int q = Prtng.partition(*c);
      typename Part2Cell::result_type Q(P2C(q));
      ovrlp[P].cells(Q).copied ().push_back(*c);            // local
	// ad hoc: we must distinguish remote and local pairs!
      if(q < 0)
	ovrlp[Q].cells(P).exposed().push_back(cell_corr(*c)); // "remote"
      else
	ovrlp[Q].cells(P).exposed().push_back(*c);            // "local"
    }
    
    //---- (II) vertex ranges ------
    
    // copied vertices that are shared by some other parts 
    // are recorded for each of the shared-owners,
    // Cp^P(Q_1) \cap Cp^P(Q_2) \neq \emptyset is possible for vertices.

    typedef typename VertexRange::ElementIterator    RgeVertexIterator;
    PartitionsByVertex<Partition>  PV(Prtng);
    typedef typename PartitionsByVertex<Partition>::PartitionOfVertexIterator VtxPartIterator;
    for(RgeVertexIterator v = ovrlp[P].vertices().copied().FirstElement(); !v.IsDone(); ++v) {
      for(VtxPartIterator qi = PV.begin(*v); qi != PV.end(*v); ++qi) {
	int  q= *qi;
	typename Part2Cell::result_type Q(P2C(q));
	ovrlp[P].vertices(Q).copied ().push_back(*v);           // local
	// ad hoc: we must distinguish remote and local pairs!
	if(q < 0)
	  ovrlp[Q].vertices(P).exposed().push_back(vtx_corr(*v)); // "remote"
	else
	  ovrlp[Q].vertices(P).exposed().push_back(*v);           // "remote"
      }
    }
    //}
}


#endif
