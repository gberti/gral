#ifndef GRAL_GB_GEOMVIEW_FORMAT_OUTPUT_H
#define GRAL_GB_GEOMVIEW_FORMAT_OUTPUT_H

// $LICENSE

// STD
#include <iostream>
#include <fstream>
#include <string>


// GrAL Base
#include "Gral/Base/common-grid-basics.h"
#include "Gral/Base/grid-function-vector.h"
#include "Gral/Base/vertex-iterator-int.h"
#include "Gral/Base/cell-iterator-int.h"



/*! \brief Output adapter for OFF/geomview file format
    \ingroup output-adapter

    \see Module \ref output-adapter
 */


class OstreamOFF2DFmt {
  typedef OstreamOFF2DFmt self;
protected:
  std::ostream * out;
  bool           owned; // is out owned?
  int            offset;

private:
  void copy(self const& rhs);

public: 
  OstreamOFF2DFmt();
  OstreamOFF2DFmt(std::ostream& ot, int off = 0);
  OstreamOFF2DFmt(std::string const& nm, int off = 0);

  OstreamOFF2DFmt(self const& rhs) { copy(rhs);}
  self & operator=   (self const& rhs) { if(this != &rhs) copy(rhs); return *this; }

  ~OstreamOFF2DFmt();

  void init(std::string const& nm);

  std::ostream& Out() { return *out;}

  int Offset() const { return offset;}

};


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


#ifdef NMWR_INCLUDE_TEMPLATE_DEFS
#include "Gral/IO/geomview-format-output.tt.C"
#endif

#endif
