#ifndef NMWR_GB_MPI_DISTR_GRID_TRAITS_H
#define NMWR_GB_MPI_DISTR_GRID_TRAITS_H

//----------------------------------------------------------------
//   (c) Guntram Berti, 1999
//   Chair for Numerical Mathematics & Scientific Computing (NMWR)
//   TU Cottbus - Germany
//   http://math-s.math.tu-cottbus.de/NMWR
//   
//----------------------------------------------------------------
#include "compiler-config.h"
#include "Grids/Distributed/distr-grid-traits.h"
#include "Grids/Distributed/mpi-distributed-grid.h"
#include "Grids/Distributed/mpi-distributed-grid-function.h"


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

// __STL_FULL_SPECIALIZATION 
// partial specialization
#define SPECIALIZATION_MPI_DISTR_GRIDFUNCTION(E,T,CG,FG)\
class distr_grid_function<grid_types<FG>::##E, T, MPIDistributedGrid<CG,FG> >\
  : public distributed_grid_function<grid_types<FG>::##E,T, MPIDistributedGrid<CG,FG> >\
{\
private:\
  typedef grid_types<FG>::##E E_type;\
  typedef distributed_grid_function<E_type ,T, MPIDistributedGrid<CG,FG> > base;\
  typedef distr_grid_function<E_type,T, MPIDistributedGrid<CG,FG> > self;\
public:\
  MAKE_DEFAULT_OPS_SELF;\
  distr_grid_function(MPIDistributedGrid<CG,FG> const& DG)\
     : distributed_grid_function<E_type ,T, MPIDistributedGrid<CG,FG> >(DG) {}\
  distr_grid_function(MPIDistributedGrid<CG,FG> const& DG, T const& t)\
     : distributed_grid_function<E_type ,T, MPIDistributedGrid<CG,FG> >(DG,t) {}\
  distr_grid_function()  {}\
};


template<class E, class T, class CG, class FG>
inline void synchronize(distr_grid_function<E,T, MPIDistributedGrid<CG,FG> > & gf)
{ gf.synchronize(); }

template<class R, class E, class T, class CG, class FG, class F>
typename F::result_type
global_max(R const&  r, 
	   distr_grid_function<E,T, MPIDistributedGrid<CG,FG> > const& gf, F const& f);

template<class T, class CG, class FG>
T global_max(T const& t, MPIDistributedGrid<CG,FG> const& MG);

/*
template<class E, class T, class CG, class FG, class OP>
T global_reduce(distr_grid_function<E,T, MPIDistributedGrid<CG,FG> > & gf, OP const& op);
*/

#ifdef NMWR_INCLUDE_TEMPLATE_DEFS
#include "generic/mpi-global-reduce.C"
#endif

#endif
