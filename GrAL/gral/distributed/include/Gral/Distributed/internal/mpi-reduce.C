#ifndef NMWR_GB_DISTR_MPI_REDUCE_C
#define NMWR_GB_DISTR_MPI_REDUCE_C

// $LICENSE


#include <mpi.h>
#include "Gral/Distributed/mpi-reduce.h"

template<class T, class CTXT> 
T mpi_global_max(const T& t, const CTXT& Ctxt)
{
  T* sendbuf = new T(t);
  T* recvbuf = new T;
  MPI_Allreduce(sendbuf,recvbuf,1,mpi_types<T>::tag(),MPI_MAX,Ctxt.TheCommunicator());
  
  T the_max = *recvbuf;
  delete recvbuf;
  delete sendbuf;
  return the_max;
}


#endif
