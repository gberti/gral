#ifndef NMWR_GB_CONSTRUCT_COMPOSITE_H
#define NMWR_GB_CONSTRUCT_COMPOSITE_H

//----------------------------------------------------------------
//   (c) Guntram Berti, 1998
//   Chair for Numerical Mathematics & Scientific Computing (NMWR)
//   TU Cottbus - Germany
//   http://math-s.math.tu-cottbus.de/NMWR
//   
//----------------------------------------------------------------


//----------------------------------------------------------------
//
// Construct a composite grid from a partition of a master grid.
// typical parameter types:
// Composite: CompositeGrid<CoarseG,FineG>
// Partition: partitioning<FineG>
// Geometry:  geometry type for FineG
// OVPattern: queue<grid_element_tag>
// VCorr:     grid_function<CoarseCell, mapping<fine_vhandle,fine_vhandle>
// CCorr:     grid_function<CoarseCell, mapping<fine_chandle,fine_chandle>
//----------------------------------------------------------------

template<class CompositeG, class Partition, class Geometry, 
         class OVPattern, class VCorr, class CCorr>
extern void 
ConstructComposite(CompositeG& CG,          // out
		   const Partition& part,   // in
		   const Geometry& Geom,    // in
		   const OVPattern& ovpatt, // in
		   VCorr& v_corr,           // out
		   CCorr& c_corr);          // out


#ifdef NMWR_INCLUDE_TEMPLATE_DEFS
#include "Gral/Distributed/internal/construct-composite.C"
#endif

#endif
