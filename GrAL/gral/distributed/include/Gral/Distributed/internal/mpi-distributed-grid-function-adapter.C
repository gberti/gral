#ifndef NMWR_GB_MPI_DISTRIBUTED_GRID_FUNCTION_ADAPTER_C
#define NMWR_GB_MPI_DISTRIBUTED_GRID_FUNCTION_ADAPTER_C

// $LICENSE


#include "Gral/Distributed/mpi-distributed-grid-function-adapter.h"

#include <algorithm>  // STL

#include "Utility/assignment-operators.h"
#include "Gral/Base/restricted-grid-function-adapter.h"



template<class GF, class DistributedG>
distributed_grid_function_adapter<GF,DistributedG>
::distributed_grid_function_adapter() : 
  added_on_shared(false), 
  sync_on_shared(false), 
  initialized(false)
{}


template<class GF, class DistributedG>
distributed_grid_function_adapter<GF,DistributedG>
::distributed_grid_function_adapter(const DistributedG&  dg)
				    
  : the_distributed(&dg),
    the_local_gf(dg.TheOvrlpGrid()),
    added_on_shared(false),
    sync_on_shared(false)
{
  init();
}

template<class GF, class DistributedG>
distributed_grid_function_adapter<GF,DistributedG>
::distributed_grid_function_adapter(DistributedG            const& dg,
				    typename GF::value_type const& t)
  : the_distributed(&dg),
    the_local_gf(dg.TheOvrlpGrid(), t),
    added_on_shared(false),
    sync_on_shared(false)
{
  init();
}

template<class GF, class DistributedG>
distributed_grid_function_adapter<GF,DistributedG>
::distributed_grid_function_adapter(GF const& ogf,
				    const DistributedG&  cg)
  : the_distributed(&cg),
    the_local_gf(ogf),
    added_on_shared(false),
    sync_on_shared(false)
{
  init();
}

template<class GF, class DistributedG>
distributed_grid_function_adapter<GF,DistributedG>
::distributed_grid_function_adapter
(distributed_grid_function_adapter<GF,DistributedG> const& rhs)
{
  if(rhs.initialized) {
    init(rhs);
  }
  else {
    initialized     = false;
    added_on_shared = false;
    sync_on_shared  = false;
  }
}

template<class GF, class DistributedG>
distributed_grid_function_adapter<GF,DistributedG> &
distributed_grid_function_adapter<GF,DistributedG>::
operator=
(distributed_grid_function_adapter<GF,DistributedG> const& rhs)
{
  if(this != &rhs) {
    init(rhs);
  }
  return *this;
}

template<class GF, class DistributedG>
void
distributed_grid_function_adapter<GF,DistributedG>
::init(distributed_grid_function_adapter<GF,DistributedG> const& rhs)
{
  if(rhs.initialized) {
    clear();
    the_distributed = rhs.the_distributed;
    the_local_gf    = rhs.the_local_gf;
    added_on_shared = rhs.added_on_shared;
    sync_on_shared  = rhs.sync_on_shared;
    
    init();
  }
}

template<class GF, class DistributedG>
void
distributed_grid_function_adapter<GF,DistributedG>
::init() 
{
    // default: connectors only between copied and exposed.
    do_exported_ranges();
    do_shared_ranges();

    initialized = true;
}

template<class CONT>
inline void erase_list(CONT & c) 
{ c.clear();}
 
template<class GF, class DistributedG>
void distributed_grid_function_adapter<GF,DistributedG>::clear()
{
  if(initialized) {
    erase_list(shared_senders);
    erase_list(shared_receivers);
    erase_list(exposed_senders);
    erase_list(copied_receivers);
  }
}


template<class GF, class DistributedG>
void distributed_grid_function_adapter<GF,DistributedG>::set_grid(const DistributedG & cg) 
{ 
    the_distributed = &cg;
    the_local_gf.set_grid(cg.TheOvrlpGrid());

    init();
} 

template<class GF, class DistributedG>
void distributed_grid_function_adapter<GF,DistributedG>::do_exported_ranges()
{
  // here could come in the CommScheduler of DistributedGrid() and give
  // a optimized and more structured iteration over the neighbors,
  // resulting in a CommPattern instead of just a list of send/receives.
  for(NeighbourIterator Nb = TheDistributedGrid().FirstNeighbour(); ! Nb.IsDone(); ++Nb) {
    if(TheOverlap().elements(tpE(),*Nb).exposed().size() > 0)
      exposed_senders.push_back
	(GetSendConnector(Restriction(TheGridFunction(), 
				      TheOverlap().elements(tpE(),*Nb).exposed()),
			  TheOverlap().elements(tpE(),*Nb).exposed().size(),
			  TheDistributedGrid().Proc(*Nb)));
    if(TheOverlap().elements(tpE(),*Nb).copied().size() > 0)
      copied_receivers.push_back
	(GetRecvConnector(Restriction(TheGridFunction(), 
				      TheOverlap().elements(tpE(),*Nb).copied()),
			  TheOverlap().elements(tpE(),*Nb).copied().size(),
			  TheDistributedGrid().Proc(*Nb)));
  }
}

template<class GF, class DistributedG>
void distributed_grid_function_adapter<GF,DistributedG>::set_added_on_shared()
{ 
  if (!added_on_shared) { 
    added_on_shared =true;
    sync_on_shared  =true;
    do_shared_ranges();
  }
}

template<class GF, class DistributedG>
void distributed_grid_function_adapter<GF,DistributedG>::set_no_sync_on_shared()
{
  sync_on_shared = false;
}

template<class GF, class DistributedG>
void distributed_grid_function_adapter<GF,DistributedG>::do_shared_ranges()
{
  // No tags necessary to distinguish messages, as the order of sends/receives
  // between pairs of processes is uniquely determined.
  // CAUTION when interleaving {begin|emnd}_{send|receive} of different gf's!!
  if(added_on_shared) {
    for(NeighbourIterator Nb = TheDistributedGrid().FirstNeighbour(); ! Nb.IsDone(); ++Nb) {
      if(TheOverlap().elements(tpE(),*Nb).shared().size() > 0) {
	shared_senders.push_back
	  (GetSendConnector(Restriction(TheGridFunction(), 
					TheOverlap().elements(tpE(),*Nb).shared()),
			    TheOverlap().elements(tpE(),*Nb).shared().size(),
			    TheDistributedGrid().Proc(*Nb)));
	shared_receivers.push_back
	  (GetRecvConnector(Restriction(TheGridFunction(), 
					TheOverlap().elements(tpE(),*Nb).shared()),
			    TheOverlap().elements(tpE(),*Nb).shared().size(),
			    TheDistributedGrid().Proc(*Nb),
			    assignment_operators::add_assign()));
      }
    }
  }
}


// this scheduling is necessary, to ensure correct handling in case of
// mixed shared / exposed<->copied exchange.
// a shared value that is copied at a third process must be synchronized
// before being sent to that third process.
template<class GF, class DistributedG>
void distributed_grid_function_adapter<GF,DistributedG>::begin_synchronize() 
{ 
  if(sync_on_shared)
    if(! shared_senders.empty()) {
      for_each(shared_senders.begin(), shared_senders.end(),   ConnectorSendBegin());
      for_each(shared_receivers.begin(), shared_receivers.end(), ConnectorRecvBegin());
    }
  // can do that now if no problems with shared ...
  if(! added_on_shared && ! exposed_senders.empty()) {
    for_each(exposed_senders.begin(),   exposed_senders.end(),   ConnectorSendBegin());
    for_each(copied_receivers.begin(), copied_receivers.end(), ConnectorRecvBegin());
  }
}

template<class GF, class DistributedG>
void distributed_grid_function_adapter<GF,DistributedG>::end_synchronize() 
{
  if(sync_on_shared) 
    if( ! shared_senders.empty()) {
      for_each(shared_senders.begin(), shared_senders.end(),     ConnectorSendEnd());
      for_each(shared_receivers.begin(), shared_receivers.end(), ConnectorRecvEnd());
    }
  // must do that after finishing with shared if  problems with shared ...
  if( added_on_shared && ! exposed_senders.empty()) {
    for_each(exposed_senders.begin(),   exposed_senders.end(), ConnectorSendBegin());
    for_each(copied_receivers.begin(), copied_receivers.end(), ConnectorRecvBegin());
  }

  if(! exposed_senders.empty()) {
    for_each(exposed_senders.begin(),   exposed_senders.end(), ConnectorSendEnd());
    for_each(copied_receivers.begin(), copied_receivers.end(), ConnectorRecvEnd());
  }
}


#endif
