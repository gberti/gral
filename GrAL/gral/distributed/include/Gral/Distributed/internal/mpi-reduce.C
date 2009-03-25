#ifndef NMWR_GB_DISTR_MPI_REDUCE_C
#define NMWR_GB_DISTR_MPI_REDUCE_C


/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti, 
    Contact: guntram AT users DOT berlios DOT de

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */



#include <mpi.h>
#include "Gral/Distributed/mpi-reduce.h"

namespace GrAL { 
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

} // namespace GrAL 

#endif
