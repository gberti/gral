#ifndef GRAL_GB_GEOMETRY_CELL_INTERSECTS_H
#define GRAL_GB_GEOMETRY_CELL_INTERSECTS_H

// $LICENSE_NEC

#include "Gral/Base/common-grid-basics.h"


/*! Predicate to decide whether a cell intersects a geometric
    body (volume) or not.

    The predicate checks if any vertex of the cell is contained
    in the given volume.
    In principle, a much more sophisticated test is possible.
    This could be driven by a policy parameter.
    For example, we could use cell subdivision in order to get
    a more accurate answer, or check intersections of the boundaries.

    \todo We should also look into the 
    <A HREF="http://www.emis.de/journals/AMUC/_contributed/algo2000/schmelzer.ps.gz"> cogeometry concept </A>
    
 */

template<class VOLUME, 
         class GRIDGEOMETRY, 
         class GT = grid_types<typename GRIDGEOMETRY::grid_type > >
class cell_is_contained {
  typedef GRIDGEOMETRY  grid_geom_type;
  typedef VOLUME        volume_type;
  typedef typename GT::grid_type grid_type;
  typedef typename GT::Cell      Cell;
  typedef typename GT::VertexOnCellIterator VertexOnCellIterator;


  grid_geom_type const* geom;
  volume_type           volume;

public:

  cell_is_contained(volume_type           vol,
		    grid_geom_type const& geo)
	      
    : geom(&geo), volume(vol) {}

  bool operator()(Cell const& c) const
    {
      bool inside = false;
      //      cerr << "Testing cell " << c.handle() << " ";
      for(VertexOnCellIterator vc(c); ! vc.IsDone(); ++vc) {
	// cerr << "v (" << geom->coord(*vc) << ") ";
	inside = inside || volume.is_inside(geom->coord(*vc));
	if(inside) break;
      }
      // cerr << "\n";
      return inside;
    }
};

/*!

*/
template<class VOLUME, 
         class GRIDGEOMETRY, 
         class GT = grid_types<typename GRIDGEOMETRY::grid_type > >
class cell_intersects {
  typedef cell_intersects<VOLUME,GRIDGEOMETRY,GT> self;

  typedef GRIDGEOMETRY  grid_geom_type;
  typedef VOLUME        volume_type;
  typedef typename GT::grid_type grid_type;
  typedef typename GT::Cell      Cell;
  typedef typename GT::VertexOnCellIterator VertexOnCellIterator;


  grid_geom_type const* geom;
  volume_type           volume;

public:
  cell_intersects() {}
  cell_intersects(volume_type           vol,
		  grid_geom_type const& geo)
	      
    : geom(&geo), volume(vol) {}

  bool operator()(Cell const& c) const
    {
      bool intersects = false;
      //      cerr << "Testing cell " << c.handle() << " ";
      typename volume_type::intersection_result res = volume.intersection_check(c, *geom);
      intersects = intersects || (res == volume_type::intersection || res == volume_type::inside);
      return intersects;
    }
};


#endif
