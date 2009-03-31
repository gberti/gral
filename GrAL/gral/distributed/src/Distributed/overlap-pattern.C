


/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include "Gral/Distributed/overlap-pattern.h"

#include <string>

#include "Utility/pre-post-conditions.h"

namespace GrAL {

overlap_pattern::overlap_pattern(std::string const& pat)
{
  for(unsigned i = 0; i < pat.size(); ++i)
    if(pat[i] == 'C')
      q.push(cell_tag);
    else if  (pat[i] == 'V')
      q.push(vertex_tag);
    else
      ENSURE_ALWAYS(false, "unknown adjacency pattern: " << pat[i] << '\n',1);
}

overlap_pattern::overlap_pattern() {}

} // namespace GrAL 
