#ifndef GRAL_GB_BASE_CELL_AS_VOLUME_H
#define GRAL_GB_BASE_CELL_AS_VOLUME_H

// $LICENSE_NEC_2003

#include "Gral/Geometries/geometric-types-nd.h"
#include "Geometry/intersections.h"
#include "Geometry/geometric-shapes.h"


/*! \brief View a cell as spatial volume
 
    \test test-cell-as-volume.C
*/
template<class GEOM, class CELL, class GT = grid_types<typename CELL::grid_type> >
class cell_as_volume : public geometric_shapes::geom_base
{
public:
  typedef GEOM geometry_type;
  typedef CELL Cell;
  typedef GT   gt;
  typedef typename geometry_type::coord_type coord_type;


private:
  typedef polytope<geometry_type, Cell, gt> polytope_type;
  polytope_type poly;
public:
  cell_as_volume() {}
  cell_as_volume(geometry_type const& geom, Cell c) : poly(geom,c) {}

  /*! \brief A boolean intersection test.

       \c X is | is not inside this volume.
   */
  template<class COORD>
  bool is_inside(COORD const& X) const;

  /*! brief A ternary intersection test.

      \c p intersects | is inside| is outside of this volume.

     \todo Does return \c intersection even if test polytope \c p is completely inside.
         Must test the boundary for intersection.
   */
  template<class POLY>
  intersection_result intersection_check(POLY const& p) const; 

  template<class ELEM2, class GEOM2> 
  intersection_result intersection_check(ELEM2 const& e2, GEOM2 const& geom2) const 
  {
    polytope<GEOM2, ELEM2> p(geom2, e2);
    return intersection_check(p);
  }

}; // cell_as_volume<>


template<class GEOM, class CELL, class GT>
template<class COORD>
bool cell_as_volume<GEOM,CELL,GT>::is_inside(COORD const& X) const
{
  typedef space_point<COORD> point_type;
  point_type pX(X);
  return intersection_by_separating_axes<point_type, polytope_type>::intersects(pX, poly);
}


template<class GEOM, class CELL, class GT>
template<class POLY>
typename cell_as_volume<GEOM,CELL,GT>::intersection_result
cell_as_volume<GEOM,CELL,GT>::intersection_check(POLY const& p) const
{
  bool intersects = intersection_by_separating_axes<POLY, polytope_type>::intersects(p,poly);
  intersection_result res;
  if(intersects)
    res = geometric_shapes::geom_base::intersection;
  else {
    if(is_inside(p(*p.FirstVertex())))
      res = geometric_shapes::geom_base::inside;
    else
      res = geometric_shapes::geom_base::outside;
  }
  return res;   
}


#endif
