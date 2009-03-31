

/* ------------------------------------------------------------

    Copyright (C) 2001 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include "Gral/IO/gmv-format-output2d.h"
#include <fstream>

namespace GrAL {
  
OstreamGMV2DFmt::OstreamGMV2DFmt() {}


OstreamGMV2DFmt::OstreamGMV2DFmt(std::ostream& ot)
  : base(ot) {}

OstreamGMV2DFmt::OstreamGMV2DFmt(std::string const& nm)
  : base(nm) {}

OstreamGMV2DFmt::~OstreamGMV2DFmt() 
{}

void OstreamGMV2DFmt::copy(OstreamGMV2DFmt const& rhs)
{
  base::copy(rhs);
}


} // namespace GrAL 


