#include "Gral/IO/complex2d-format-output.h"
 
OstreamComplex2DFmt::OstreamComplex2DFmt()
  : out(0), owned(false), offset(0), nv(0), nc(0) {}

OstreamComplex2DFmt::OstreamComplex2DFmt(std::ostream& ot, int off)
  : out(&ot), owned(false), offset(off), nv(0), nc(0) {}

OstreamComplex2DFmt::OstreamComplex2DFmt(std::string const& nm, int off)
  : out(new std::ofstream(nm.c_str())), owned(true), offset(off), nv(0), nc(0) {}

OstreamComplex2DFmt::~OstreamComplex2DFmt()
{ if(owned) delete out;}


void OstreamComplex2DFmt::copy(OstreamComplex2DFmt const& rhs)  
{
  REQUIRE( !rhs.owned , "cannot copy ostream!",1);
  out = rhs.out;
  owned = false;
  offset = rhs.offset;
  nv = rhs.nv;
  nc = rhs.nc;
}

void OstreamComplex2DFmt::init(std::string const& nm)
{
  if(owned) delete out;
  owned = true;
  out = new std::ofstream(nm.c_str());
}
