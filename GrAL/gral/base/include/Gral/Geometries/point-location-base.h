#ifndef GRAL_GB_BASE_GEOMETRIES_POINT_LOCATION_BASE_H
#define GRAL_GB_BASE_GEOMETRIES_POINT_LOCATION_BASE_H

// $LICENSE_NEC_2003

#include "Gral/Base/common-grid-basics.h"
#include <string>

namespace GrAL {

/*! \brief Result type for point location algorithms

    \ingroup geometricgridtools
    \see \ref geometricgridtools module 
 */
template<class GT, class COORD>
class point_location_result {
public:
  enum result_tag { inside_tag, outside_tag, projection_tag, failure_tag, num_of_tags };
  static  ::std::string tag_names[num_of_tags];

   ::std::string tagname() const { return tag_names[t];}

  typedef COORD                    coord_type;
  typedef typename GT::Cell        Cell;
  typedef typename GT::cell_handle cell_handle;
private:
  Cell         c; // cell of p 
  coord_type   p; // original point or projection
  result_tag   t; // inside_tag     => p is original point, inside c
                  // outside_tag    => original point outside grid, p and c undefined
                  // projection_tag => original point outside grid, p is projected to nearest point in grid, p on bd. of c
                  // failure_tag    => error occured.
  
public:
  point_location_result() : t(outside_tag) {}
  point_location_result(Cell cc, coord_type pp) : c(cc), p(pp), t(inside_tag) {}
  point_location_result(Cell cc, coord_type pp, result_tag tt) : c(cc), p(pp), t(tt) {}
  point_location_result(result_tag tt) : t(tt) {}

  cell_handle handle()   const { return c.handle();}
  Cell        TheCell()  const { return c;}
  coord_type  TheCoord() const { return p;}
  result_tag  tag   ()   const { return t;}
};


template<class GT, class COORD>
::std::string point_location_result<GT,COORD>::tag_names[point_location_result<GT,COORD>::num_of_tags]
= { "inside", "outside", "projection", "failure" };

} // namespace GrAL 

#endif
