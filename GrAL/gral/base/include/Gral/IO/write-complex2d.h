#ifndef GRAL_BASE_GB_WRITE_COMPLEX2D_H
#define GRAL_BASE_GB_WRITE_COMPLEX2D_H



// $LICENSE

#include "Gral/Base/common-grid-basics.h"

//-------------------------------------------------------------------------
//
/*! \defgroup writecomplex2d write_complex2d() functions
   \brief set of routines to write a grid in complex2d format.
   \ingroup complex2dformat
   \note
   User should use \ref OstreamComplex2DFmt.
 
   \see Module \ref complex2dformat
   \see \ref OstreamComplex2DFmt
*/
//-------------------------------------------------------------------------


/*! \ingroup writecomplex2d
    Sets offset to 0. <BR>
    Does not write any vertex coordinates

    \see Module \ref  writecomplex2d
*/
template<class GRID>
void write_complex2d(GRID const& G,  ostream& out, int offset = 0);

/*! \ingroup writecomplex2d
    
  Sets offset to 0.
    \see Module \ref writecomplex2d
*/
template<class GRID, class GEOM>
void write_complex2d(GRID const& G, GEOM const& Geo, ostream& out);

/*! \ingroup writecomplex2d

    \see Module \ref writecomplex2d
*/
template<class GRID, class GEOM>
void write_complex2d(GRID const& G, GEOM const& Geo, ostream& out, int offset);


#ifdef NMWR_INCLUDE_TEMPLATE_DEFS
#include "write-complex2d.C"
#endif

#endif
