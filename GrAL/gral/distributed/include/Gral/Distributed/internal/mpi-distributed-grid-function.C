#ifndef NMWR_GB_MPI_DISTRIBUTED_GRID_FUNCTION_C
#define NMWR_GB_MPI_DISTRIBUTED_GRID_FUNCTION_C

#include "algorithm"  // STL

#include "Container/operators.h"
#include "Gral/restricted-grid-function.h"
#include "Gral/Distributed/mpi-distributed-grid-function.h"


template<class E, class T, class DistributedG>
distributed_grid_function<E,T,DistributedG>::distributed_grid_function(const DistributedG&  cg)
  : the_distributed(&cg),
    the_local_gf(cg.TheOvrlpGrid()),
    added_on_shared(false),
    sync_on_shared(false)
{
  // default: connectors only between copied and exposed.
  do_exported_ranges();
  do_shared_ranges();
}

template<class E, class T, class DistributedG>
distributed_grid_function<E,T,DistributedG>::distributed_grid_function(const DistributedG&  cg,
								       const T& t)
  : the_distributed(&cg),
    the_local_gf(cg.TheOvrlpGrid(),t),
    added_on_shared(false),
    sync_on_shared(false)

{
  // default: connectors only between copied and exposed.
  do_exported_ranges();
  do_shared_ranges();
}

template<class E, class T, class DistributedG>
void distributed_grid_function<E,T,DistributedG>::set_grid(const DistributedG & cg) 
{ 
    the_distributed = &cg;
    the_local_gf.set_grid(cg.TheOvrlpGrid());
    do_exported_ranges();
    do_shared_ranges();
} 

template<class E, class T, class DistributedG>
void distributed_grid_function<E,T,DistributedG>::do_exported_ranges()
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

template<class E, class T, class DistributedG>
void distributed_grid_function<E,T,DistributedG>::set_added_on_shared()
{ 
  if (!added_on_shared) { 
    added_on_shared =true;
    do_shared_ranges();
  }
}

template<class E, class T, class DistributedG>
void distributed_grid_function<E,T,DistributedG>::set_no_sync_on_shared()
{
  sync_on_shared = false;
}

template<class E, class T, class DistributedG>
void distributed_grid_function<E,T,DistributedG>::do_shared_ranges()
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
			    add_assign<T>()));
      }
    }
  }
}


// this scheduling is necessary, to ensure correct handling in case of
// mixed shared / exposed<->copied exchange.
// a shared value that is copied at a third process must be synchronized
// before being sent to that third process.
template<class E, class T, class DistributedG>
void distributed_grid_function<E,T,DistributedG>::begin_synchronize() 
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

template<class E, class T, class DistributedG>
void distributed_grid_function<E,T,DistributedG>::end_synchronize() 
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
