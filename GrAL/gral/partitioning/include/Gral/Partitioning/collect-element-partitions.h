#ifndef NMWR_GB_COLLECT_ELEMENT_PARTITIONS_H
#define NMWR_GB_COLLECT_ELEMENT_PARTITIONS_H


// $LICENSE

/*! \file
 */

/*! \defgroup partitioncollect  Mapping partitionings from cells to other elements
 
     \ingroup partitioningmodule
 */

/*! \brief Collect partitions for each vertex.
    \ingroup partitiontools

    \post 
      For each vertex \c v of \c P.TheGrid(),  \c p_of_v(v)
      is the set of partitions of the incident cells.
      If <tt> mark_on_boundary == true </tt> then 
      for each \c v on the boundary of P.TheGrid(), 
      \c p_of_v(v) also contains the value \c -1.
*/

template<class Partition, class Vtx2PartMap>
extern void 
collect_vertex_partitions(const Partition& P,            // in
			  Vtx2PartMap    & p_of_v,       // out 
			  bool mark_on_boundary);  // int

/*! \brief Collect partition for each vertex
       \ingroup partitiontools 
     Convenience wrapper setting <tt>mark_on_boundary == true</tt>.
 */
template<class Partition, class Vtx2PartMap>
inline void 
collect_vertex_partitions(const Partition& P,            // in
			  Vtx2PartMap    & p_of_v)       // out 
{ collect_vertex_partitions(P, p_of_v, (bool)true); }

//----------------------------------------------------------------
// produce mapping Facet --> { pnum } where
// partitions_of_facet(f) = { p, q } <=> f separates p and q
//----------------------------------------------------------------

/*! \brief Collect partition for each facet
      \ingroup partitiontools 
    \post 
      For each facet \c f of \c P.TheGrid(), 
      the result \c partitions_of_facet(f) contains the set of partitions of
      incident cells. <br>
      If <tt> mark_on_boundary == true </tt> then 
      for each boundary facet \c f, \c partitions_of_facet(f) also contains the value \c -1.
 */
template<class Partition, class Fac2PartMap>
extern void 
collect_facet_partitions(const Partition& P,                    // in
			 Fac2PartMap    & partitions_of_facet,  // out 
			 bool mark_on_boundary /* = true */);         // in

/*! \brief Collect partition for each facet
    \ingroup partitiontools

    Convenience wrapper setting <tt>mark_on_boundary == true</tt>.
 */ 
template<class Partition, class Fac2PartMap>
inline void 
collect_facet_partitions(const Partition& P,                    // in
			 Fac2PartMap    & partitions_of_facet)  // out 
{ collect_facet_partitions(P,partitions_of_facet, true);}

#ifdef NMWR_INCLUDE_TEMPLATE_DEFS
#include "Gral/Partitioning/internal/collect-element-partitions.C"
#endif

#endif
