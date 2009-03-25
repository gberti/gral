#ifndef NMWR_GB_MPI_GLOBAL_REDUCE_C
#define NMWR_GB_MPI_GLOBAL_REDUCE_C



/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti, 
    Contact: guntram AT users DOT berlios DOT de

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include "Gral/Distributed/mpi-distributed-grid-traits.h"

#include <mpi.h>
#include "algorithm"

#include "Gral/Distributed/mpi-reduce.h"

namespace GrAL {

//----------------------------------------------------------------
//
// global reduce methods for mpi-distributed grid functions.
// currently, only max is implemented.
//
//----------------------------------------------------------------

template<class R, class E, class T, class CG, class FG, class F>
typename F::result_type
global_max(R const&  range, 
	   distr_grid_function<E,T, MPIDistributedGrid<CG,FG> > const& gf, F const& f)
{
  // local_max = global_max(r,gf.TheGridFunction(),f);
  typedef typename F::result_type res_type;
    res_type res = f(*(range.FirstElement()), gf);
  typedef typename R::ElementIterator eiter;
  for(typename R::ElementIterator e = range.FirstElement(); ! e.IsDone(); ++e)
    res = max(res,f(*e,gf));
  
  return mpi_global_max(res, gf.TheDistributedGrid());
}

template<class T, class CG, class FG>
T global_max(T const& t, MPIDistributedGrid<CG,FG> const& MG)
{ return  mpi_global_max(t,MG); }

} // namespace GrAL 
 
#endif
