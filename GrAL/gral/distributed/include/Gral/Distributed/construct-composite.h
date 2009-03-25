#ifndef NMWR_GB_CONSTRUCT_COMPOSITE_H  // -*- c++ -*-
#define NMWR_GB_CONSTRUCT_COMPOSITE_H



/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti, 
    Contact: guntram AT users DOT berlios DOT de

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


namespace GrAL {

//----------------------------------------------------------------
/*! \brief Construct a composite grid from a partition of a master grid.
  \ingroup compositegrids

 \templateparams
  - \c Composite: e.g. \c CompositeGrid<CoarseG,FineG>
  - \c Partition: e.g. \c partitioning<FineG>
  - \c Geometry:  geometry type for \c FineG
  - \c OVPattern: queue<grid_element_tag>
  - \c VCorr: e. g. \c grid_function<CoarseCell,mapping<fine_vhandle,fine_vhandle>
  - \c CCorr: e. g. \c grid_function<CoarseCell, mapping<fine_chandle,fine_chandle>
*/
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

} // namespace GrAL 

#ifdef NMWR_INCLUDE_TEMPLATE_DEFS
#include "Gral/Distributed/internal/construct-composite.C"
#endif

#endif
