#ifndef GRAL_BASE_GB_WRITE_COMPLEX2D_H
#define GRAL_BASE_GB_WRITE_COMPLEX2D_H


//----------------------------------------------------------------
//   (c) Guntram Berti, 1998
//   Chair for Numerical Mathematics & Scientific Computing (NMWR)
//   TU Cottbus - Germany
//   http://math-s.math.tu-cottbus.de/NMWR
//   
//----------------------------------------------------------------

#include "Gral/Base/common-grid-basics.h"

//-------------------------------------------------------------------------
//
// write_complex2d(GRID, GEOM, ostream&) - write a grid in complex2d format.
//
//
//-------------------------------------------------------------------------


// offset = 1
template<class GRID>
void write_complex2d(GRID const& G,  ostream& out);

template<class GRID, class GEOM>
void write_complex2d(GRID const& G, GEOM const& Geo, ostream& out);

template<class GRID, class GEOM>
void write_complex2d(GRID const& G, GEOM const& Geo, ostream& out, int offset);


#ifdef NMWR_INCLUDE_TEMPLATE_DEFS
#include "generic/write-complex2d.C"
#endif

#endif
