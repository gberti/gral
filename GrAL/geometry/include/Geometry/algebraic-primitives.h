#ifndef NMWR_GB_ALGEBRAIC_PRIMITIVES
#define NMWR_GB_ALGEBRAIC_PRIMITIVES




/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include <math.h>
#include "Utility/pre-post-conditions.h"

#include "Geometry/point-traits.h"
#include "Geometry/basic-algebraic-primitives.h"
#include "Geometry/primitives2d.h"
#include "Geometry/primitives3d.h"

#include <vector>

/*! \file 
*/

namespace GrAL {

/*! \file 

  This file contains some generic implementations for 
  elementary algebraic & geometric operations,
  such as determinants or area-calculations.

  class structure:
 <pre> 
                + - - - ddp 2d - - - +
                |                    |
                + - - - ddp 3d - - - +
                |                    |
 ap ---- ddp -- + - - - - - - - - - -+--- bap
</pre>
 where:
 - ap  - algebraic_primitives:            user-interface class
 - bap - basic_algebraic_primitives:      non-dimension-dependent functions
 - ddp - dimension_dependent_primitives : bap + possible extensions for 2D/3D. 
         This class provides a hook to be specialized for concrete POINT types.
 - ddp{2d,3d} - dimension_dependent_primitives_{2d,3d} : special functions for 2D or 3D. 
       This can be used to specialize ddp, e.g.:
\code
       dimension_dependent_primitives<mypoint> : 
          public dimension_dependent_primitives_2d<mypoint> {};
\endcode
*/


/*! \defgroup algebraicprimitives_impl

    \ingroup algebraicprimitives  

    Hook for introducing special functionality into algebraic_primitives 
 */
template<class POINT>
struct dimension_dependent_primitives<POINT, tag2D>
  : public virtual dimension_dependent_primitives_2d<POINT>  {};

template<class POINT>
struct dimension_dependent_primitives<POINT, tag3D>
  : public virtual dimension_dependent_primitives_3d<POINT>  {};

template<class POINT>
struct dimension_dependent_primitives<POINT, variable_dimension_tag>
   : public basic_algebraic_primitives<POINT> 
  {
    typedef dimension_dependent_primitives_2d<POINT> ap2d;
    typedef dimension_dependent_primitives_3d<POINT> ap3d;
  };
 

/*! \defgroup algebraicprimitives Primitive algebraic and geometric operations
    
    The class is intended for general use is <code> algebraic_primitives </code>.
    If the point type \c P is  known at compile-time to be of dimension 2 or 3,
    extended functionality is automatically defined beyond the basic dimension-independent
    stuff in \c basic_algebraic_primitives.
    Otherwise, specific 2D or 3D functionality is available via nested types ap2d and ap3d.

    Example:
    \code
    typedef algebraic_primitives<coord_type> ap;

    coord_type P, Q;
    ... 
    if(ap::distance(P,Q) > eps) {
      ...
    }
    \endcode
 */

/*! \brief Collection of primitive algebraic and geometric operations on point types

    \ingroup algebraicprimitives   

    This class contains various primitive operations as static member functions,
    see the base class \c basic_algebraic_primitives.
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



/*! \brief Calculate centroid and area of a simple polygon in 2D
  
    \ingroup algebraicprimitives   
 
    The polygon is given by a sequence of vertices \c [begin, end).
    
    The centroid (or center of inertia) c of a domain D is defined by
    \f$ \int_{D} x_i dx dy / \mbox{area}(D) \f$
    
    This code has been adapted from GraphicsGems IV, file \c centroid.c

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
