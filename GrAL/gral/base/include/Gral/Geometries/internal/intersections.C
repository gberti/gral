#ifndef GRAL_GB_GEOMETRIES_INTERSECTIONS_C
#define GRAL_GB_GEOMETRIES_INTERSECTIONS_C

// $LICENSE 

#include "Gral/Geometries/intersections.h"
#include "Container/tuple.h"

namespace GrAL {

template<class GEOM, class GT>
bool 
edge_facet_intersection<GEOM,GT>::operator()
(typename edge_facet_intersection<GEOM,GT>::Edge  const& e,
 typename edge_facet_intersection<GEOM,GT>::Facet const& f) 
{
  REQUIRE((f.NumOfVertices() == 3 || f.NumOfVertices() == 4),
	  "Sorry, intersection of facet and edge is implemented "
	  <<"only for 3 or 4 vertices!",1);
  bool res = false;
  segment_t s(coo(e.V1()),
	      coo(e.V2()));
  if(f.NumOfVertices() == 3) {
    tuple<Vertex,3> v(f.FirstVertex(),f.EndVertex());
    sti_type sti(s, // segment_t (coo(e.v1()), coo(e.v2())),
		 triangle_t(coo(v[0]),coo(v[1]),coo(v[2])));
    if(sti.segment_intersects_triangle()) {
      res =  true;
      is_point = sti.intersection();
    }
  }
  else if(f.NumOfVertices() == 4) {
    // form all sides of the tetrahedron which is the convex hull of f.
    // This approach would generalize to an arbitrary number of vertices.
    tuple<Vertex,4> v(f.FirstVertex(),f.EndVertex());
    for(int i = 0; i < 4; ++i) {
      int k = (i+1) % 4;
      int j = (i+2) % 4;
      triangle_t t(coo(v[i]), coo(v[j]), coo(v[k]));

      sti_type sti(s,t);
      bool found = sti.segment_intersects_triangle();
      if(found) {
	is_point = sti.intersection();
      }
      res |= found;
    }
  }
  else { // should never arrive here
    ;
  }
  return res;
}

} // namespace GrAL 

#endif
