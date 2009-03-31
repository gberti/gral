#ifndef NMWR_GB_DISTR_MPI_REDUCE_H
#define NMWR_GB_DISTR_MPI_REDUCE_H




/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include <mpi.h>
#include "Gral/Distributed/mpi-types.h"

namespace GrAL {

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

} // namespace GrAL 

#ifdef NMWR_INCLUDE_TEMPLATE_DEFS
#include "Gral/Distributed/internal/mpi-reduce.C"
#endif


#endif
