#include "Gral/IO/geomview-format-output.h"

namespace GrAL {
 
OstreamOFF2DFmt::OstreamOFF2DFmt()
  : out(0), owned(false), offset(0) {}

OstreamOFF2DFmt::OstreamOFF2DFmt(std::ostream& ot, int off)
  : out(&ot), owned(false), offset(off) {}

OstreamOFF2DFmt::OstreamOFF2DFmt(std::string const& nm, int off)
  : out(new std::ofstream(nm.c_str())), owned(true), offset(off)
{}

OstreamOFF2DFmt::~OstreamOFF2DFmt()
{ if(owned) delete out;}


void OstreamOFF2DFmt::copy(OstreamOFF2DFmt const& rhs)  
{
  REQUIRE( !rhs.owned , "cannot copy ostream!",1);
  out = rhs.out;
  owned = false;
  offset = rhs.offset;
}

void OstreamOFF2DFmt::init(std::string const& nm)
{
  if(owned) delete out;
  owned = true;
  out = new std::ofstream(nm.c_str());
}


} // namespace GrAL 
