
// $LICENSE_NEC

#include "Gral/IO/gmv-format-output2d.h"
#include <fstream>
  
OstreamGMV2DFmt::OstreamGMV2DFmt()
  : out(0), owned(false) {}

OstreamGMV2DFmt::OstreamGMV2DFmt(std::ostream& ot)
  : out(&ot), owned(false), num_vars(0) {}

OstreamGMV2DFmt::OstreamGMV2DFmt(std::string const& nm)
  : out(new std::ofstream(nm.c_str())), owned(true), num_vars(0) {}

OstreamGMV2DFmt::~OstreamGMV2DFmt() 
{ if(owned) delete out;}

void OstreamGMV2DFmt::copy(OstreamGMV2DFmt const& rhs)
{
  REQUIRE( !rhs.owned , "cannot copy ostream!",1);
  out = rhs.out;
  num_vars = rhs.num_vars;
  owned = false;
  // offset = rhs.offset;
  //  nv = rhs.nv;
  // nc = rhs.nc;
}


void OstreamGMV2DFmt::init(std::string const& nm)
{
  if(owned) delete out;
  owned = true;
  out = new std::ofstream(nm.c_str());
}


