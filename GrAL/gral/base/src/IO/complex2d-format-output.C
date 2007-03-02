#include "Gral/IO/complex2d-format-output.h"

#include "Utility/file-utils.h"

namespace GrAL {


 

OstreamComplex2DFmt::OstreamComplex2DFmt()
  : out(0), offset(0), nv(0), nc(0) {}

OstreamComplex2DFmt::OstreamComplex2DFmt(ref_ptr<std::ostream> ot, int off)
  : out(ot),  offset(off), nv(0), nc(0) {}

OstreamComplex2DFmt::OstreamComplex2DFmt(std::ostream& ot, int off)
  : out(ot),  offset(off), nv(0), nc(0) {}

OstreamComplex2DFmt::OstreamComplex2DFmt(std::string const& nm, int off)
  :  offset(off), nv(0), nc(0) 
{ init(nm);}

OstreamComplex2DFmt::~OstreamComplex2DFmt()
{ }


void OstreamComplex2DFmt::copy(OstreamComplex2DFmt const& rhs)  
{
  out = rhs.out;
  offset = rhs.offset;
  nv = rhs.nv;
  nc = rhs.nc;
}

void OstreamComplex2DFmt::init(std::string const& nm)
{
  out = get_file_or_stdout(nm);
}

} // namespace GrAL 
