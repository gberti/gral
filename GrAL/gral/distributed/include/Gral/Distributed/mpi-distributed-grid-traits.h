#ifndef NMWR_GB_MPI_DISTR_GRID_TRAITS_H
#define NMWR_GB_MPI_DISTR_GRID_TRAITS_H



/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include "Config/compiler-config.h"
#include "Gral/Distributed/distr-grid-traits.h"
#include "Gral/Distributed/mpi-distributed-grid.h"
#include "Gral/Distributed/mpi-distributed-grid-function.h"

namespace GrAL {

/*! For partial specialization of distr_grid_traits
    \ingroup mpidistributedgrids
   
 */
template<class CG, class FG>
struct distr_grid_traits_mpidg {

  typedef MPIDistributedGrid<CG,FG> distributed_grid_type;
  typedef distributed_grid_type     dgt;
  typedef typename dgt::range_type  range_type;
  typedef FG                        base_grid_type;

  static range_type     const& LocalRange(distributed_grid_type const& DG)
    { return DG.LocalRange(); }
  static base_grid_type const& BaseGrid  (distributed_grid_type const& DG)
    { return DG.TheGrid(); }

};
/*! \brief partial specialization of distr_grid_traits
  \ingroup mpidistributedgrids
 */
template<class CG, class FG>
struct distr_grid_traits<MPIDistributedGrid<CG,FG> >
  : public distr_grid_traits_mpidg<CG,FG> 
{};

/*! \brief partial specialization of distr_grid_function<E,T,DG>
    \ingroup mpidistributedgrids
       
*/
template<class E1, class T1, class CG, class FG>
class distr_grid_function<E1,T1,MPIDistributedGrid<CG,FG> >
: public distributed_grid_function<E1,T1, MPIDistributedGrid<CG,FG> >
{
 private:
  typedef distributed_grid_function<E1,T1, MPIDistributedGrid<CG,FG> > base;
  typedef distr_grid_function      <E1,T1, MPIDistributedGrid<CG,FG> > self;
 public:
  distr_grid_function()  {}
  distr_grid_function(MPIDistributedGrid<CG,FG> const& DG)
    : distributed_grid_function<E1, T1, MPIDistributedGrid<CG,FG> >(DG) {}
  distr_grid_function(MPIDistributedGrid<CG,FG> const& DG, T1 const& t)
    : distributed_grid_function<E1, T1, MPIDistributedGrid<CG,FG> >(DG,t) {}
};


template<class E, class T, class CG, class FG>
inline void synchronize(distr_grid_function<E,T, MPIDistributedGrid<CG,FG> > & gf)
{ gf.synchronize(); }

/*! \brief Global maximum over a mpi distributed grid function.
    \ingroup mpidistributedgrid
 */
template<class R, class E, class T, class CG, class FG, class F>
typename F::result_type
global_max(R const&  r, 
	   distr_grid_function<E,T, MPIDistributedGrid<CG,FG> > const& gf, F const& f);

/*! \overload
 */
template<class T, class CG, class FG>
T global_max(T const& t, MPIDistributedGrid<CG,FG> const& MG);

/*
template<class E, class T, class CG, class FG, class OP>
T global_reduce(distr_grid_function<E,T, MPIDistributedGrid<CG,FG> > & gf, OP const& op);
*/

} // namespace GrAL 

#ifdef NMWR_INCLUDE_TEMPLATE_DEFS
#include "Gral/Distributed/internal/mpi-global-reduce.C"
#endif

#endif
