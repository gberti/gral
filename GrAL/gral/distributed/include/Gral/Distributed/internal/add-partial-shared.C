#ifndef NMWR_GB_ADD_PARTIAL_SHARED_C
#define NMWR_GB_ADD_PARTIAL_SHARED_C

//----------------------------------------------------------------
//   (c) Guntram Berti, 1998
//   Chair for Numerical Mathematics & Scientific Computing (NMWR)
//   TU Cottbus - Germany
//   http://math-s.math.tu-cottbus.de/NMWR
//   
//----------------------------------------------------------------

#include "Gral/Distributed/internal/add-partial-shared.h"
#include "Gral/partitioning.h"

template<class Partition, class Overlap,
         class Part2Cell,
         class VertexRange, class FacetRange,
         class VtxCorr,     class FacetCorr>
void AddPartialSharedRanges(Overlap          &  ovrlp,  // out
			    Partition   const&  Prtng,
			    int                 p,
			    Part2Cell   const&  P2C,
			    VertexRange const&  shared_v,
			    FacetRange  const&  shared_f,
			    VtxCorr     const&  vtx_corr,
			    FacetCorr   const&  facet_corr)
{
  // for(int p = 0; p < (int) Prtng.NumOfPartitions(); ++p) {
  typedef typename FacetRange::ElementIterator RgeFacetIterator;
  for(RgeFacetIterator f = shared_f.FirstElement(); ! f.IsDone(); ++f) {
    int q = Prtng.other_partition(*f,p);
    //      if(p > q) { // <---- unsymmetric
    if( (P2C(p) > P2C(q)) || (P2C(p) == P2C(q) && p > q)) { // <---- unsymmetric
      ovrlp[P2C(p)].facets(P2C(q)).shared().push_back(*f);             // local
      if( q < 0)
	ovrlp[P2C(q)].facets(P2C(p)).shared().push_back(facet_corr(*f)); // "remote"
      else
	ovrlp[P2C(q)].facets(P2C(p)).shared().push_back(*f);             // "local"
    }
  }
  // }
  
  PartitionsByVertex<Partition>  PV(Prtng);
  typedef typename PartitionsByVertex<Partition>::PartitionOfVertexIterator VtxPartIterator;
  // for(int p = 0; p < (int) Prtng.NumOfPartitions(); ++p) {
  // CoarseCell P(Prtng.PartCell());
  typedef typename VertexRange::ElementIterator  RgeVertexIterator;
  for(RgeVertexIterator v = shared_v.FirstElement(); ! v.IsDone(); ++v) {
    for(VtxPartIterator qi = PV.begin(*v); qi != PV.end(*v); ++qi) {
      int q = *qi;
      //	if(p > q) { // <---- unsymmetric
      if( (P2C(p) > P2C(q)) || (P2C(p) == P2C(q) && p > q)) { // <---- unsymmetric
	ovrlp[P2C(p)].vertices(P2C(q)).shared().push_back(*v);             // local
	if( q < 0)
	  ovrlp[P2C(q)].vertices(P2C(p)).shared().push_back(vtx_corr(*v));   // "remote"
	else
	  ovrlp[P2C(q)].vertices(P2C(p)).shared().push_back(*v);             // "local"
      }
    }
  }
  // }
}

#endif
