#include "Gral/IO/gmv-format-output.h"
#include "Utility/pre-post-conditions.h"

#include <fstream>
  
OstreamGMVFmt_base::OstreamGMVFmt_base()
  : out(0), owned(false) {}

OstreamGMVFmt_base::OstreamGMVFmt_base(std::ostream& ot)
  : out(&ot), owned(false), num_vars(0) {}

OstreamGMVFmt_base::OstreamGMVFmt_base(std::string const& nm)
  : out(new std::ofstream(nm.c_str())), owned(true), num_vars(0) {}

OstreamGMVFmt_base::~OstreamGMVFmt_base() 
{ if(owned) delete out;}

void OstreamGMVFmt_base::copy(OstreamGMVFmt_base const& rhs)
{
  REQUIRE( !rhs.owned , "cannot copy ostream!",1);
  out = rhs.out;
  num_vars = rhs.num_vars;
  owned = false;
  // offset = rhs.offset;
  //  nv = rhs.nv;
  // nc = rhs.nc;
}


void OstreamGMVFmt_base::init(std::string const& nm)
{
  if(owned) delete out;
  owned = true;
  out = new std::ofstream(nm.c_str());
}

