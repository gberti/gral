#ifndef NMWR_GB_BASE_IO_COMPLEX2D_FORMAT_INPUT_H
#define NMWR_GB_BASE_IO_COMPLEX2D_FORMAT_INPUT_H

// $LICENSE

#include <vector>

#include "Gral/IO/complex2d-format-input-base.h"

#include "Geometry/coords.h"




/*! \brief Input adapter for Complex2D Format
    \ingroup complex2dformat
 
    Model of $GrAL Cell-VertexInputGridRange 
    and  $GrAL VertexGeometry.
   
    \see Module \ref complex2dformat
 */

class IstreamComplex2DFmt : public IstreamComplex2DFmt_base
{
  typedef IstreamComplex2DFmt      self;
  typedef IstreamComplex2DFmt_base base;
public:
  typedef coordN<2>       coord_type;
private:
  std::vector<coord_type> coords;
public:
  IstreamComplex2DFmt() {}
  IstreamComplex2DFmt(std::istream      & in, int off = 0) : base(in,off) 
  { read_coords();}
  IstreamComplex2DFmt(std::string  const& fn, int off = 0) : base(fn,off)
  { read_coords();}

  IstreamComplex2DFmt(self const& rhs) { copy(rhs);}
  self& operator=    (self const& rhs) { 
    if(this != & rhs)
      copy(rhs); 
    return *this;
  }
protected:
  void copy(self const& rhs) 
  {
    base::copy(rhs);
    copy_coords(rhs);
  }
  virtual void read_coords() 
  {
    coords = std::vector<coord_type>(NumOfVertices());
    for(unsigned v = 0; v < NumOfVertices(); ++v)
      In() >> coords[v];
  }
  virtual void copy_coords(self const& rhs)
  {
    coords = rhs.coords;
  }

public:
  coord_type const& coord(Vertex const& v) const { return coords[v.handle()];}
};


template<>
struct grid_types<IstreamComplex2DFmt>
  : public grid_types<IstreamComplex2DFmt_base>
{
  typedef IstreamComplex2DFmt grid_type;
};


/*! \brief Input adapter for combinatorial Complex2D Format
    \ingroup complex2dformat
 
    This class assumes a modified format without geometry information.
    Model of $GrAL Cell-VertexInputGridRange.
   
    \see Module \ref complex2dformat
    \todo Iterator classes refer to IstreamComplex2DFmt_base.
 */

class IstreamComplex2DCombFmt : public IstreamComplex2DFmt_base
{
  typedef IstreamComplex2DCombFmt  self;
  typedef IstreamComplex2DFmt_base base;
public:
  IstreamComplex2DCombFmt() {}
  IstreamComplex2DCombFmt(std::istream      & in, int off = 0) : base(in,off) 
  { }
  IstreamComplex2DCombFmt(std::string  const& fn, int off = 0) : base(fn,off)
  { }

  IstreamComplex2DCombFmt(self const& rhs) { copy(rhs);}
  self& operator=    (self const& rhs) { 
    if(this != & rhs)
      copy(rhs); 
    return *this;
  }
};


template<>
struct grid_types<IstreamComplex2DCombFmt>
  : public grid_types<IstreamComplex2DFmt_base>
{
  typedef IstreamComplex2DCombFmt grid_type;
};

#endif
