

// $LICENSE

#include "Gral/Distributed/overlap-pattern.h"

#include <string>

#include "Utility/pre-post-conditions.h"

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
