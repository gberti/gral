#ifndef NMWR_GB_DISTR_MPI_REDUCE_H
#define NMWR_GB_DISTR_MPI_REDUCE_H


//----------------------------------------------------------------
//   (c) Guntram Berti, 1998
//   Chair for Numerical Mathematics & Scientific Computing (NMWR)
//   TU Cottbus - Germany
//   http://math-s.math.tu-cottbus.de/NMWR
//   
//----------------------------------------------------------------

#include <mpi.h>
#include "Grids/Distributed/mpi-types.h"

///////////////////////////////////////////
//
// Some experimental/provisoric implementations
// of global reduce functions like max,min, ...
// This explicitly uses MPI at the moment.
//
///////////////////////////////////////////



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


#endif
