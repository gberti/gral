#include "Gral/IO/complex2d-format-input-base.h"

IstreamComplex2DFmt_base::IstreamComplex2DFmt_base()    
  : offset(0),
    in(0),
    checked_in(in),
    is_in_owned(false),
    nv_nc_read(false),
    cell_iter_instance(false)
{}

IstreamComplex2DFmt_base::IstreamComplex2DFmt_base(std::string const& file, int off)    
  : offset(off),
    in(new std::ifstream(file.c_str())),
    checked_in(in),
    is_in_owned(true),
    nv_nc_read(false),
    cell_iter_instance(false)

{
 
}

IstreamComplex2DFmt_base::IstreamComplex2DFmt_base(std::istream& is, int off)
  : offset(off),
    in(&is),
    checked_in(&is),
    is_in_owned(false),
    nv_nc_read(false),
    cell_iter_instance(false)

{
 
}

IstreamComplex2DFmt_base::~IstreamComplex2DFmt_base() 
{ if (is_in_owned) delete in;}

void IstreamComplex2DFmt_base::copy(IstreamComplex2DFmt_base const& rhs)
  { 
    REQUIRE( !rhs.is_in_owned , "cannot copy istream!",1);
    in = rhs.in;
    checked_in = checked_istream(in);
    is_in_owned = false;
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
  std::ifstream * fin = new std::ifstream(file.c_str());
  REQUIRE( fin->is_open(), "Could not open file " << file << '\n',1);
  in = fin;
  checked_in = checked_istream(in);
  is_in_owned = true;
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
