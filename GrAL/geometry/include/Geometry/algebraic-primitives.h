#ifndef NMWR_GB_ALGEBRAIC_PRIMITIVES
#define NMWR_GB_ALGEBRAIC_PRIMITIVES



// $LICENSE

#include <math.h>
#include "Utility/pre-post-conditions.h"

#include "Geometry/point-traits.h"
#include "Geometry/basic-algebraic-primitives.h"
#include "Geometry/primitives2d.h"
#include "Geometry/primitives3d.h"

#include <vector>


namespace GrAL {

//----------------------------------------------------------------
//
//  this file contains some generic implementations for 
//  elementary algebraic & geometric operations,
//  such as determinants or area-calculations.
//
//  class structure:
//  
//                + - - - ddp 2d - - - +
//                |                    |
//                + - - - ddp 3d - - - +
//                |                    |
// ap ---- ddp -- + - - - - - - - - - -+--- bap
//
// where:
// ap  - algebraic_primitives:            user-interface class
// bap - basic_algebraic_primitives:      non-dimension-dependent functions
// ddp - dimension_dependent_primitives : bap + possible extensions for 2D/3D. 
//       This class may be specialized for concrete POINT types.
// ddp{2d,3d} - dimension_dependent_primitives_{2d,3d} : special functions
//       for 2D or 3D. 
//       This can be used to specialize ddp, e.g.:
//       dimension_dependent_primitives<mypoint> : 
//         public dimension_dependent_primitives_2d<mypoint> {};
//
//----------------------------------------------------------------




////////////////////////////////////////////////////////////////
//
// dimension dependent functions 
//
////////////////////////////////////////////////////////////////






template<class POINT>
struct dimension_dependent_primitives<POINT, tag2D>
  : public dimension_dependent_primitives_2d<POINT>  {};

template<class POINT>
struct dimension_dependent_primitives<POINT, tag3D>
  : public  dimension_dependent_primitives_3d<POINT>  {};

template<class POINT>
struct dimension_dependent_primitives<POINT, variable_dimension_tag>
  : public dimension_dependent_primitives_2d<POINT>,
    public dimension_dependent_primitives_3d<POINT>
  {};

/*! \defgroup algebraicprimitives Primitive algebraic and geometric operations
 */

/*! \brief Collection of primitive algebraic and geometric operations on point types

    \ingroup algebraicprimitives   

    This class contains various primitive operations as static member functions. 
    For 2D and 3D points, some more primitives are automatically generated,
    see \c dimension_dependent_primitives_2d and \c dimension_dependent_primitives_3d
    for details on these primitives.

    For using these function for your own point class, 
    specialize the point_traits<> template for this class.

    For examples of how to use algebraic_primitives, see \ref test-primitives.C,
    \ref test-primitives2d.C, and \ref test-primitives3d.C.
    
*/  
template<class POINT, class PT = point_traits<POINT> >
struct algebraic_primitives 
  : public dimension_dependent_primitives
    <
     POINT,
     typename  PT::dimension_tag
    > 
{};



/*! \brief calculate centroid and area of a simple polygon
 
 The polygon is given by a sequence of vertices \c [begin, end).

 The centroid (or center of inertia) c of a domain D is defined by
 \f$ \int_{D} x_i dx dy / \mbox{area}(D) \f$

 This is adapted from GraphicsGems IV, centroid.c

*/

template<class PIt, class Q>
void get_polygon2d_center_and_area(PIt begin, PIt end, // in:  iterator over polygon vertices
				   Q& center,          // out: centroid
				   double& area);      // out: polygon area


} // namespace GrAL 

#ifdef NMWR_INCLUDE_TEMPLATE_DEFS
#include "Geometry/algebraic-primitives.C"
#endif

#endif
