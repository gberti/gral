
// $LICENSE_NEC

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


