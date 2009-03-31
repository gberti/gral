#ifndef GRAL_GB_BASE_GEOMETRIES_POINT_LOCATION_BASE_H
#define GRAL_GB_BASE_GEOMETRIES_POINT_LOCATION_BASE_H


/* ------------------------------------------------------------

    Copyright (C) 2001 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


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
  /*! \brief Result type
   */
  enum result_tag { inside_tag,      //!< point is inside geometric structure
                    outside_tag,     //!< point is outside geometric structure
		    projection_tag,  //!< point is outside geometric structure, but could be projected to nearest point inside
		    failure_tag,     //!< point location failed
		    num_of_tags };
  static  std::string tag_names[num_of_tags];

   std::string tagname() const { return tag_names[t];}

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

  /*! \brief Return the located cell

      This is guaranteed to be a valid cell iff <tt> tag() == inside_tag || tag() == projection_tag </tt>
  */
  Cell        TheCell()  const { return c;}

  /*! \brief Return the handle of the located cell.

     Equivalent to \c TheCell().handle()
  */
  cell_handle handle()   const { return c.handle();}


  /*! \brief Return the coordinate of the original point (if <tt> tag() == inside_tag)
       or its projection (if <tt> tag() == projection_tag </tt>)
   */ 
  coord_type  TheCoord() const { return p;}

  /*! \brief Return the result tag
   */
  result_tag  tag   ()   const { return t;}

  /*! \brief True iff <tt> tag() == inside_tag </tt>
   */
  bool        found()    const { return t == inside_tag;}
};


template<class GT, class COORD>
::std::string point_location_result<GT,COORD>::tag_names[point_location_result<GT,COORD>::num_of_tags]
= { "inside", "outside", "projection", "failure" };

} // namespace GrAL 

#endif
