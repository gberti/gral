#ifndef GRAL_GB_GEOMVIEW_FORMAT_OUTPUT_CONSTRUCT_H
#define GRAL_GB_GEOMVIEW_FORMAT_OUTPUT_CONSTRUCT_H


/* ------------------------------------------------------------

    Copyright (C) 2001 - 2009 Guntram Berti, 
    Contact: guntram AT users DOT berlios DOT de

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


/*! \file
 */


#ifdef NMWR_INCLUDE_TEMPLATE_DEFS
#include "Gral/IO/geomview-format-output.tt.C"
#else

namespace GrAL {

/*! \brief ConstructGrid overload for OstreamOFF2DFmt

  \relates OstreamOFF2DFmt
  
  \see GrAL base module \ref complex2dformat
  \see GrAL base module \ref copyoperations
  \see Test in \ref test-geomview-output.C
 */
template<class GRID, class GEOM>
extern 
void ConstructGrid(OstreamOFF2DFmt& Out, 
                   GRID const& G, GEOM const& Geom);

} // namespace GrAL 

#endif


namespace GrAL {

/*! \brief ConstructGrid overload for OstreamOFF2DFmt

  \relates OstreamOFF2DFmt
  
  Dummy output geometry variable.
  \see GrAL base module \ref complex2dformat
  \see GrAL base module \ref copyoperations
  \see Test in \ref test-geomview-output.C

 */
template<class GRID, class GEOM>
inline void 
ConstructGrid(OstreamOFF2DFmt& Out, 
	      OstreamOFF2DFmt&, // dummy output geom
	      GRID const& G, GEOM const& Geom)
{
  ConstructGrid(Out,G,Geom);
}

} // namespace GrAL 

#endif
