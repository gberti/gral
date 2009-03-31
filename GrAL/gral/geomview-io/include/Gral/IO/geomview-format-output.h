#ifndef GRAL_GB_GEOMVIEW_FORMAT_OUTPUT_H
#define GRAL_GB_GEOMVIEW_FORMAT_OUTPUT_H


/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


/*! \file
 */

// STD
#include <iostream>
#include <fstream>
#include <string>


// GrAL Base
#include "Gral/Base/common-grid-basics.h"
#include "Gral/Base/grid-function-vector.h"
#include "Gral/Base/vertex-iterator-int.h"
#include "Gral/Base/cell-iterator-int.h"


/*! \defgroup outputadapter Output adapter for generic file I/O
*/

namespace GrAL {

/*! \brief Output adapter for OFF/geomview file format
    
    \ingroup outputadapter

    \see Module \ref outputadapter
    \see GrAL base module \ref copyoperations
    \see Test in \ref test-geomview-output.C

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

} // namespace GrAL 

#include "Gral/IO/geomview-format-output-construct.h"

#endif
