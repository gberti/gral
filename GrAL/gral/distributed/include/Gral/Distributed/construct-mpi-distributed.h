#ifndef NMWR_GB_CONSTRUCT_MPI_DISTRIBUTED_H
#define NMWR_GB_CONSTRUCT_MPI_DISTRIBUTED_H

//----------------------------------------------------------------
//
//   (c) Guntram Berti, 1999
//   Chair for Numerical Mathematics & Scientific Computing (NMWR)
//   TU Cottbus - Germany
//   http://math-s.math.tu-cottbus.de/NMWR
//   
//----------------------------------------------------------------

#include "Container/bijective-mapping.h"
#include "Gral/partitioning.h"
#include "Gral/Distributed/overlap-pattern.h"
#include "Gral/Distributed/mpi-distributed-grid.h"

//----------------------------------------------------------------
// ConstructMPIDistributedFromMaster -- build a mpi-distributed grid
//    via an intermediary composite grid.
//    
//----------------------------------------------------------------

template<class CG, class FG, class FGEOM, class MG, class GEOM, class TRAFO>
void
ConstructMPIDistributedFromMaster
(
 MPIDistributedGrid<CG,FG>          & DistrG,          // OUT: to be constructed
 FGEOM                              & DistrGeom,       // [OUT] to date, implicitely constructed
 partitioning<MG>                   & Prtng,           // IN:  contains partitioning & master
 GEOM                         const & MasterGeom,      // IN:  geometry for master grid
 overlap_pattern              const & ovlp_pattern,    // IN:  logical dscr. of overlap
 bijective_mapping<int,int>         & per_v_1,         // IN:  mapping of periodic bd 
                                                       //  (optional, may be empty)
 bijective_mapping<int,int>         & per_v_2,         //  inverse of per_v_1
 TRAFO                        const & T1,              // IN: geometric transf. corr. to per_v_1
 TRAFO                        const & T2,              //  inverse of T1
 bijective_mapping<int, int>        & master2distr_v,  //  map vertices of master to those of DistrG
 bijective_mapping<int, int>        & master2distr_c   //  same for cells
);


#ifdef NMWR_INCLUDE_TEMPLATE_DEFS
#include "Gral/Distributed/internal/construct-mpi-distributed.C"
#endif

#endif
