#ifndef NMWR_GB_CONSTRUCT_COMPOSITE_PERIODIC_H
#define NMWR_GB_CONSTRUCT_COMPOSITE_PERIODIC_H

//----------------------------------------------------------------
//   (c) Guntram Berti, 1998
//   Chair for Numerical Mathematics & Scientific Computing (NMWR)
//   TU Cottbus - Germany
//   http://math-s.math.tu-cottbus.de/NMWR
//   
//----------------------------------------------------------------

//----------------------------------------------------------------
//  ConstructComposite_per(...)
//  construct a composite grid on a periodic geometry.
//----------------------------------------------------------------


template<class CompositeG, class Partition, class Geometry, class VCorrLoc,
         class OVPattern, class VCorr, class CCorr, class Transform>
void ConstructComposite_per(CompositeG     & CG,          // out
			    Partition      & Prtng,       // in -- writeable for tech. reasons
			    Geometry  const& geom,        
			    OVPattern const& ovlp_pattern,
			    VCorrLoc  const& id_per_v1, // vertices identified due to periodicity
			    VCorrLoc  const& id_per_v2, // = id_per_v1^{-1}
			    Transform const& T1, // geometric transformation (translation) V1 -> V2
			    Transform const& T2, // = T1^{-1}
			    VCorr          & v_corr,    // out: global x coarse cell  --> local
			    CCorr          & c_corr);


#ifdef NMWR_INCLUDE_TEMPLATE_DEFS
#include "Gral/Distributed/internal/construct-composite-periodic.C"
#endif


#endif
