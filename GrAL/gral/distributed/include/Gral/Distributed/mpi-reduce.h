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
#include "Gral/Distributed/mpi-types.h"

//----------------------------------------------------------------
/*! \brief Wrapper for MPI Global reduce function
    \ingroup mpidistributedgrids
    \todo  Implement other reduce functions or even a generic version,
    using arbitrary  (commutative and associative) operators.
  
   \templateparams
    - CTXT: Has member <tt> mpicomm CTXT::TheCommunicator() </tt>
*/
//----------------------------------------------------------------


template<class T, class CTXT> 
T mpi_global_max(const T& t, const CTXT& Ctxt);

#ifdef NMWR_INCLUDE_TEMPLATE_DEFS
#include "Gral/Distributed/internal/mpi-reduce.C"
#endif


#endif
