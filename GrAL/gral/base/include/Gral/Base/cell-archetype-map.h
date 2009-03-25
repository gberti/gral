#ifndef GRAL_GB_BASE_CELL_ARCH_MAP_H
#define GRAL_GB_BASE_CELL_ARCH_MAP_H


/* ------------------------------------------------------------

    Copyright (C) 2001 - 2009 Guntram Berti, 
    Contact: guntram AT users DOT berlios DOT de

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include "Gral/Base/common-grid-basics.h"

namespace GrAL {

/* \brief Map back and forth between elements and its archetype 
   counterparts in a given cell.

   \ingroup gridarchetypes

   Mappings from grid elements incident to a cell to corresponding elements
   of the cell archetype elements involve  a local search over the cell. 
   Mappings from ElementOnCell incidence iterators to archetype elements 
   often can avoid this searching. 

   \todo Support other element types than Vertex.
 */

template<class GRID>
class cell_archetype_map 
{

  typedef GRID grid_type;
  typedef grid_types<grid_type>        gt;
  typedef typename  gt::archetype_type archetype_type;
  typedef grid_types<archetype_type>   agt;
  /*
  typedef typename gt::Cell          Cell;
  typedef typename gt::Vertex        Vertex;
  typedef typename gt::vertex_handle vertex_handle;
  */
private:
  typename gt::Cell c;
public:
  cell_archetype_map(typename gt::Cell const& cc) : c(cc) {}

  // Archetype vertices <-> Cell vertices
  typename agt::vertex_handle operator()(typename gt::VertexOnCellIterator vc) const
  { return v(vc);}
  typename agt::vertex_handle v(typename gt::VertexOnCellIterator vc) const 
  { return vc.local_handle();}
  typename agt::Vertex V(typename gt::VertexOnCellIterator vc) const
  { return typename agt::Vertex(c.TheGrid().ArchetypeOf(c), v(vc));}

  
  typename agt::vertex_handle operator()(typename gt::vertex_handle const& v) const
    { 
      typename gt::VertexOnCellIterator vc(c);
      for( ; ! vc.IsDone(); ++vc) {
	if( vc.handle() == v)
	  return typename agt::vertex_handle(vc.local_handle());
      }
      // not found? Error!
      ENSURE(false, "Vertex " << v 
	     << " not incident to cell " << c.handle() << '\n',1); 
      // keep compilers quiet
      return typename agt::vertex_handle(-1);
    }
  typename agt::Vertex operator()(typename gt::Vertex const& v) const
    { 
     REQUIRE(( &v.TheGrid() == & c.TheGrid()), 
	      "v.TheGrid() = " << &v.TheGrid() << " " 
	      << "c.TheGrid() = " << &c.TheGrid() << '\n',1); 
     return typename agt::Vertex(c.TheArchetype(), (*this)(v.handle()));
    }

  typename gt::Vertex operator()(typename agt::Vertex const& av) const
    { return c.V(av);}
  typename gt::vertex_handle operator()(typename agt::vertex_handle av) const
    { return c.v(av);}
};

} // namespace GrAL 

#endif
