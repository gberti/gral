#ifndef NMWR_GB_DISTR_MPI_REDUCE_H
#define NMWR_GB_DISTR_MPI_REDUCE_H



// $LICENSE

#include <mpi.h>
#include "Gral/Distributed/mpi-types.h"

///////////////////////////////////////////
//
// Some experimental/provisoric implementations
// of global reduce functions like max,min, ...
// This explicitly uses MPI at the moment.
//
///////////////////////////////////////////

namespace GrAL {

template<class T, class CTXT> 
T global_max(const T& t, const CTXT& Ctxt)
{
  T* sendbuf = new T(t);
  T* recvbuf = new T;
  MPI_Allreduce(sendbuf,recvbuf,1,mpi_types<T>::tag(),MPI_MAX,Ctxt.TheCommunicator());
  
  T the_max = *recvbuf;
  delete recvbuf;
  delete sendbuf;
  return the_max;
}

} // namespace GrAL 

#endif
