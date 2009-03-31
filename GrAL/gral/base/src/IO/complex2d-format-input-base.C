
/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include "Gral/IO/complex2d-format-input-base.h"

#include "Utility/file-utils.h"

namespace GrAL {

IstreamComplex2DFmt_base::IstreamComplex2DFmt_base()    
  : offset(0),
    in(0),
    nv_nc_read(false),
    cell_iter_instance(false)
{}

IstreamComplex2DFmt_base::IstreamComplex2DFmt_base(std::string const& file, int off)    
  : offset(off),
    nv_nc_read(false),
    cell_iter_instance(false)

{
  init(file);
}

IstreamComplex2DFmt_base::IstreamComplex2DFmt_base(std::istream& is, int off)
  : offset(off),
    in(is),
    checked_in(*in),
    nv_nc_read(false),
    cell_iter_instance(false)
{ }

IstreamComplex2DFmt_base::IstreamComplex2DFmt_base(ref_ptr<std::istream> is, int off)
  : offset(off),
    in(is),
    checked_in(*in),
    nv_nc_read(false),
    cell_iter_instance(false)
{ }




IstreamComplex2DFmt_base::~IstreamComplex2DFmt_base() 
{ }

void IstreamComplex2DFmt_base::copy(IstreamComplex2DFmt_base const& rhs)
  { 
    in = rhs.in;
    if(in != 0)
      checked_in = checked_istream(*in);
    offset = rhs.offset;
    nv = rhs.nv;
    nc = rhs.nc;
    cell_iter_instance = rhs.cell_iter_instance;
    // no call to init() here - either rhs has already been initialized  using in,
    // in which case the information is ok, or not - then in is not a valid stream,
    // and there must be a call to init(string) later on.
  }

void IstreamComplex2DFmt_base::init(std::string const& file) 
{
  in = get_file_or_stdin(file);
  checked_in = checked_istream(*in);
  init();
}  

void IstreamComplex2DFmt_base::init() const
{
  if(! nv_nc_read) {
    In() >> nv >> nc;
    nv_nc_read = true;
  }
}

bool IstreamComplex2DFmt_base::initialized() const
{ return nv_nc_read; }

} // namespace GrAL 
