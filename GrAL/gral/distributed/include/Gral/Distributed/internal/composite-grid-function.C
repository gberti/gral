#ifndef NMWR_GB_COMPOSITE_GRID_FUNCTION_C
#define NMWR_GB_COMPOSITE_GRID_FUNCTION_C

// $LICENSE


#include <algorithm>  // STL

#include "Utility/pre-post-conditions.h"

#include "Gral/Base/restricted-grid-function-adapter.h"
#include "Gral/Distributed/composite-grid-function.h"

namespace GrAL {

template<class E, class T, class CompositeG>
composite_grid_function<E,T,CompositeG>::composite_grid_function(const CompositeG&  cg)
  : the_composite(&cg),
    local_gfs(cg.TheCoarseGrid())
{
  for(CoarseCellIterator C = TheCoarseGrid().FirstCell(); ! C.IsDone(); ++C) {
    local_gfs[*C].set_grid(TheCompositeGrid().OvrlpGrid(*C));
  }

  // default: connectors only between copied and exposed.
  for(CoarseCellIterator C = TheCoarseGrid().FirstCell(); ! C.IsDone(); ++C)
    for(NeighbourIterator Nb = TheCompositeGrid().FirstNeighbour(*C); ! Nb.IsDone(); ++Nb) {
      REQUIRE( (    Overlap(*C ).elements(tpE(),*Nb).exposed().size()
		 == Overlap(*Nb).elements(tpE(),*C ).copied() .size()),
	       "ranges of unequal size!\n",1);
      if(Overlap(*C ).elements(tpE(),*Nb).exposed().size()  > 0)
	the_senders.push_back
	  (CopyConnector(Restriction(GridFunction(*C), Overlap(*C ).elements(tpE(),*Nb).exposed()),
			 Restriction(GridFunction(*Nb),Overlap(*Nb).elements(tpE(),*C).copied())));
    }
  
  /* shared with add-semantics: more difficult. */
}

template<class E, class T, class CompositeG>
void composite_grid_function<E,T,CompositeG>::synchronize() 
{ 
  for_each(the_senders.begin(),   the_senders.end(),   ConnectorSendData());
  for_each(the_receivers.begin(), the_receivers.end(), ConnectorRecvData());
}

} // namespace GrAL 

#endif
