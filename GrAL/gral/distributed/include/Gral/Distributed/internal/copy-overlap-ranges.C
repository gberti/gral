#ifndef NMWR_GB_GRAL_DISTRIBUTED_COPY_OVERLAP_RANGES_C
#define NMWR_GB_GRAL_DISTRIBUTED_COPY_OVERLAP_RANGES_C

// $LICENSE

#include "Gral/Distributed/copy-overlap-ranges.h"
#include "Container/sequence-algorithms.h"

template<class DestRange, class SrcRange,  class Filter>
void CopyOverlapRanges(DestRange     & dest,
                       SrcRange const& src, 
                       Filter   const& f)
{
  dest.TheContainer().reserve(src.size());
  CopyOverlapRanges(dest,
                    src.privee(), src.exposed(), src.shared(), src.copied(),
                    f);
}


template<class DestRange,class SrcRange,  class Filter>
void CopyOverlapRanges(DestRange& dest,
                       SrcRange const& priv, SrcRange const& exp,
                       SrcRange const& shd,  SrcRange const& cop,
                       Filter   const& f)
{
  sequence::copy_filter(priv.begin(),priv.end(), std::back_inserter(dest), f);
  dest.set_exposed_begin();
  sequence::copy_filter(exp.begin(),exp.end(), std::back_inserter(dest), f);
  dest.set_shared_begin();
  sequence::copy_filter(shd.begin(),shd.end(), std::back_inserter(dest), f);
  dest.set_copied_begin();
  sequence::copy_filter(cop.begin(),cop.end(), std::back_inserter(dest), f);
  // dest.set_copied_end();
}

/*! \overload 

template<class DestRange,class SrcRange,  class Filter>
void CopyOverlapRanges(DestRange& dest,
                                             SrcRange const& exp,
                       SrcRange const& shd,  SrcRange const& cop,
                       Filter   const& f)
{ 
  SrcRange priv(exp.TheGrid());
  ENSURE( priv.empty() , " priv is not empty!",1);
  CopyOverlapRanges(dest, priv,exp,shd,cop, f);
}

*/

#endif
