#ifndef NMWR_GB_RANGE_LAYERS_C
#define NMWR_GB_RANGE_LAYERS_C


// $LICENSE

#include "Gral/Distributed/overlap-ranges.h"

#include "Container/range.h"

template<class OVRange>
void write_ovrge(OVRange const& rnge, std::ostream& out)
{
  out << "private:\n"
      << range(rnge.privee().begin(),  rnge.privee().end()) << '\n' 
      << "exposed:\n"
      << range(rnge.exposed().begin(), rnge.exposed().end()) << '\n' 
      << "shared:\n"
      << range(rnge.shared().begin(),  rnge.shared().end()) << '\n' 
      << "copied\n"
      << range(rnge.copied().begin(),  rnge.copied().end()) << '\n'; 
}

#endif
