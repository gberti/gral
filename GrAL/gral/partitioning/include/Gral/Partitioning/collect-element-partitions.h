#ifndef NMWR_GB_COLLECT_ELEMENT_PARTITIONS_H
#define NMWR_GB_COLLECT_ELEMENT_PARTITIONS_H

//----------------------------------------------------------------
//   (c) Guntram Berti, 1998
//   Chair for Numerical Mathematics & Scientific Computing (NMWR)
//   TU Cottbus - Germany
//   http://math-s.math.tu-cottbus.de/NMWR
//   
//----------------------------------------------------------------


//----------------------------------------------------------------
// Collect partitions for each vertex.
// p_of_v : Vertex --> { pnum } \cup {-1}
// 
// This mapping will be calculated for all vertices of
// P.TheGrid(). For vertices v on the boundary of P.TheGrid(),
// {-1} \subset partitions_of_vertex(v).
//----------------------------------------------------------------

template<class Partition, class Vtx2PartMap>
extern void 
collect_vertex_partitions(const Partition& P,            // in
			  Vtx2PartMap    & p_of_v,       // out 
			  bool mark_on_boundary);  // int

template<class Partition, class Vtx2PartMap>
inline void 
collect_vertex_partitions(const Partition& P,            // in
			  Vtx2PartMap    & p_of_v)       // out 
{ collect_vertex_partitions(P, p_of_v, (bool)true); }

//----------------------------------------------------------------
// produce mapping Facet --> { pnum } where
// partitions_of_facet(f) = { p, q } <=> f separates p and q
//----------------------------------------------------------------

template<class Partition, class Fac2PartMap>
extern void 
collect_facet_partitions(const Partition& P,                    // in
			 Fac2PartMap    & partitions_of_facet,  // out 
			 bool mark_on_boundary /* = true */);         // in

template<class Partition, class Fac2PartMap>
inline void 
collect_facet_partitions(const Partition& P,                    // in
			 Fac2PartMap    & partitions_of_facet)  // out 
{ collect_facet_partitions(P,partitions_of_facet, true);}

#ifdef NMWR_INCLUDE_TEMPLATE_DEFS
#include "Grids/Algorithms/collect-element-partitions.C"
#endif

#endif
