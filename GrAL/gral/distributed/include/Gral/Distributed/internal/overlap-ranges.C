#ifndef NMWR_GB_RANGE_LAYERS_C
#define NMWR_GB_RANGE_LAYERS_C



/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include "Gral/Distributed/overlap-ranges.h"

#include "Container/range.h"

namespace GrAL {

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

} // namespace GrAL 

#endif
