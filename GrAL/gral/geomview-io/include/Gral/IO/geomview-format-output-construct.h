#ifndef GRAL_GB_GEOMVIEW_FORMAT_OUTPUT_CONSTRUCT_H
#define GRAL_GB_GEOMVIEW_FORMAT_OUTPUT_CONSTRUCT_H

// $LICENSE_NEC

#ifdef NMWR_INCLUDE_TEMPLATE_DEFS
#include "Gral/IO/geomview-format-output.tt.C"
#else

/*! \brief ConstructGrid overload for OstreamOFF2DFmt
  \ingroup complex2dformat
  \relates OstreamOFF2DFmt
  
  \see Module \ref complex2dformat
  \see Module \ref mutatingoperations
  \see \ref ConstructGrid
 */
template<class GRID, class GEOM>
extern 
void ConstructGrid(OstreamOFF2DFmt& Out, 
                   GRID const& G, GEOM const& Geom);

#endif


/*! \brief ConstructGrid overload for OstreamOFF2DFmt
  \ingroup complex2dformat
  \relates OstreamOFF2DFmt
  
  Dummy output geometry variable.
  \see Module \ref complex2dformat
  \see Module \ref mutatingoperations
  \see \ref ConstructGrid
 */
template<class GRID, class GEOM>
inline void 
ConstructGrid(OstreamOFF2DFmt& Out, 
	      OstreamOFF2DFmt&, // dummy output geom
	      GRID const& G, GEOM const& Geom)
{
  ConstructGrid(Out,G,Geom);
}


#endif
