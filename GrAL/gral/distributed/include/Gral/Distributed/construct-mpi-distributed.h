#ifndef NMWR_GB_CONSTRUCT_MPI_DISTRIBUTED_H
#define NMWR_GB_CONSTRUCT_MPI_DISTRIBUTED_H


// $LICENSE

// for identity
#include <functional>

#include "Container/bijective-mapping.h"

// GrAL
#include "Gral/Partitioning/partitioning.h"
#include "Gral/Distributed/overlap-pattern.h"
#include "Gral/Distributed/mpi-distributed-grid.h"

//----------------------------------------------------------------
/*! \brief Builds a mpi-distributed grid via an intermediary 
    composite grid.

    The grid may be periodic.
   \ingroup mpidistributedgrids
   \todo Fix geometry construction!
   \todo Replace args of type bijective_mapping<int,int> with either
        template args or at least bijective_mapping<vertex_handle, vertex_handle>
*/    
//----------------------------------------------------------------

template<class CG, class FG, class FGEOM, class MG, class GEOM, 
         class VCORRLOC, class TRAFO, class VCORR, class CCORR>
void
ConstructMPIDistributedFromMaster
(
 MPIDistributedGrid<CG,FG>          & DistrG,          // OUT: to be constructed
 FGEOM                              & DistrGeom,       // [OUT] to date, implicitely constructed
 partitioning<MG>                   & Prtng,           // IN:  contains partitioning & master
 GEOM                         const & MasterGeom,      // IN:  geometry for master grid
 overlap_pattern              const & ovlp_pattern,    // IN:  logical dscr. of overlap
 VCORRLOC                           & per_v_1,         // IN:  mapping of periodic bd 
                                                       //  (optional, may be empty)
 VCORRLOC                           & per_v_2,         //  inverse of per_v_1
 TRAFO                        const & T1,              // IN: geometric transf. corr. to per_v_1
 TRAFO                        const & T2,              //  inverse of T1
 VCORR                              & master2distr_v,  //  map vertices of master to those of DistrG
 CCORR                              & master2distr_c   //  same for cells
);


//----------------------------------------------------------------
/*! \brief Builds a mpi-distributed grid via an intermediary 
    composite grid.

    Simplified syntax if the grid is not periodic.
   \ingroup mpidistributedgrids
*/    
//----------------------------------------------------------------

template<class CG, class FG, class FGEOM, class MG, class GEOM,
         class VCORR, class CCORR>
inline void
ConstructMPIDistributedFromMaster
(
 MPIDistributedGrid<CG,FG>          & DistrG,          // OUT: to be constructed
 FGEOM                              & DistrGeom,       // [OUT] to date, implicitely constructed
 partitioning<MG>                   & Prtng,           // IN:  contains partitioning & master
 GEOM                         const & MasterGeom,      // IN:  geometry for master grid
 overlap_pattern              const & ovlp_pattern,    // IN:  logical dscr. of overlap
 VCORR                               & master2distr_v,  //  map vertices of master to those of DistrG
 CCORR                               & master2distr_c   //  same for cells
)

{
  typedef grid_types<MG> mgt;
  typedef typename  mgt::vertex_handle master_v_h;
  bijective_mapping<master_v_h,master_v_h> per_v_1, per_v_2;
  typedef typename GEOM::coord_type coord_type;
  std::identity<coord_type> T1, T2;
  ConstructMPIDistributedFromMaster(DistrG, DistrGeom,
				    Prtng,  MasterGeom,
				    ovlp_pattern,
				    per_v_1, per_v_2, T1, T2, // dummies
				    master2distr_v, master2distr_c);				    
}


#ifdef NMWR_INCLUDE_TEMPLATE_DEFS
#include "Gral/Distributed/internal/construct-mpi-distributed.C"
#endif

#endif
