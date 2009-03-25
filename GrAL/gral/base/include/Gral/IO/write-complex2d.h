#ifndef GRAL_BASE_GB_WRITE_COMPLEX2D_H
#define GRAL_BASE_GB_WRITE_COMPLEX2D_H




/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti, 
    Contact: guntram AT users DOT berlios DOT de

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include <iostream>
#include "Gral/Base/common-grid-basics.h"

namespace GrAL {

//-------------------------------------------------------------------------
//
/*! \internal
   \defgroup writecomplex2d write_complex2d() functions
   \brief set of routines to write a grid in complex2d format.
   \ingroup complex2dformat
   \note
   User should use \ref OstreamComplex2DFmt.
 
   \see Module \ref complex2dformat
   \see \ref OstreamComplex2DFmt
*/
//-------------------------------------------------------------------------


/*! \internal 

    \ingroup writecomplex2d
    Sets offset to 0. <BR>
    Does not write any vertex coordinates

    \see Module \ref  writecomplex2d
*/
template<class GRID>
void write_complex2d(GRID const& G,   ::std::ostream& out, int offset = 0);

/*! \ingroup writecomplex2d
    Does not write any vertex coordinates
    Procuces a mapping from G to integer numbers, starting from 0 
    (that is, essentially a enumeration of the vertices and cells of G).

    \see Module \ref  writecomplex2d
*/
template<class GRID, class VCORR, class CCORR>
void write_complex2d(GRID const& G,   ::std::ostream& out, int offset, 
                     VCORR & G2Out_v, CCORR & G2Out_c);


/*! \internal
    \ingroup writecomplex2d
    Sets offset to 0. <BR>
    Does not write any vertex coordinates. <BR>
    Procuces a mapping from G to integer numbers, starting from 0 
    (that is, essentially a enumeration of the vertices and cells of G).

    \see Module \ref  writecomplex2d
*/
template<class GRID, class VCORR, class CCORR>
void write_complex2d(GRID const& G,   ::std::ostream& out, 
                     VCORR & G2Out_v, CCORR & G2Out_c);


/*! \internal 
   \ingroup writecomplex2d
    
    Sets offset to 0.
    \see Module \ref writecomplex2d
*/
template<class GRID, class GEOM>
void write_complex2d(GRID const& G, GEOM const& Geo,  ::std::ostream& out);

/*! \internal
    \ingroup writecomplex2d

    \see Module \ref writecomplex2d
*/
template<class GRID, class GEOM>
void write_complex2d(GRID const& G, GEOM const& Geo,  ::std::ostream& out, int offset);

} // namespace GrAL 

#ifdef NMWR_INCLUDE_TEMPLATE_DEFS
#include "write-complex2d.C"
#endif

#endif
