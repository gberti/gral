#ifndef GRAL_GB_BASE_CELL_ARCH_MAP_H
#define GRAL_GB_BASE_CELL_ARCH_MAP_H

// $LICENSE_NEC

#include "Gral/Base/common-grid-basics.h"


/* \brief Map back and forth between elements and its archetype 
   counterparts in a given cell.

   Mappings from grid elements to archetype elements typically
   involve  a local search over the cell. Mappings from
   ElementOnCell incidence iterators to archetype elements should
   avoid this searching. 
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
    { return vc.local_handle();}
  
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

#endif
