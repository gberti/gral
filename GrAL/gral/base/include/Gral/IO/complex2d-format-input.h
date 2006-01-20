#ifndef NMWR_GB_BASE_IO_COMPLEX2D_FORMAT_INPUT_H
#define NMWR_GB_BASE_IO_COMPLEX2D_FORMAT_INPUT_H

// $LICENSE

/*! \file
 */

#include <vector>

#include "Gral/IO/complex2d-format-input-base.h"

#include "Geometry/point.h"

namespace GrAL {

/*! \brief Input adapter for Complex2D Format
    \ingroup complex2dformat
 
    Model of $GrAL Cell-VertexInputGridRange 
    and  $GrAL VertexGridGeometry.
   
    \see Module \ref complex2dformat
    \todo Test
 */

class IstreamComplex2DFmt : public IstreamComplex2DFmt_base
{
  typedef IstreamComplex2DFmt      self;
  typedef IstreamComplex2DFmt_base base;
public:
  // typedef coordN<2>       coord_type;
  typedef point           coord_type;
private:
  unsigned                        spacedim;
  mutable  ::std::vector<coord_type> coords;
  mutable bool                    coords_read;
public:
  IstreamComplex2DFmt() {}
  IstreamComplex2DFmt( ::std::istream      & in, int off = 0) 
    : base(in,off), spacedim(2), coords_read(false) 
  { }
  IstreamComplex2DFmt( ::std::string  const& fn, int off = 0) 
    : base(fn,off), spacedim(2), coords_read(false)
  { }

  IstreamComplex2DFmt(self const& rhs) { copy(rhs);}
  self& operator=    (self const& rhs) { 
    if(this != & rhs)
      copy(rhs); 
    return *this;
  }

  void set_spacedim(unsigned sd) 
    { 
      REQUIRE(! coords_read, "coords already read!\n",1);
      spacedim = sd;
    }
protected:
  void copy(self const& rhs) 
  {
    base::copy(rhs);
    copy_coords(rhs);
  }
  virtual void init()        const;
  virtual void init( ::std::string const& file) { base::init(file);}
  virtual bool initialized() const;
  virtual void read_coords() const;
  virtual void copy_coords(self const& rhs)
  {
    coords = rhs.coords;
  }

public:
  coord_type const& coord(Vertex const& v) const {
    if(! coords_read){
      // should not occur, init() should have been called before.
      IstreamComplex2DFmt * slf = const_cast<IstreamComplex2DFmt *>(this);
      slf->read_coords();
      slf->coords_read = true;
    }
    return coords[v.handle()];}
};

  struct grid_types_IstreamComplex2DFmt 
    : public grid_types_IstreamComplex2DFmt_base
  {
    typedef IstreamComplex2DFmt grid_type;
  };

template<>
struct grid_types<IstreamComplex2DFmt>
  : public grid_types_base<grid_types_IstreamComplex2DFmt>
{};



  // overloads of begin/end/size for IstreamComplex2DFmt
#ifdef gt
#error gt already in use (missing previous undef ?)
#else
#define gt grid_types<IstreamComplex2DFmt>

template<class It>
inline typename gt::VertexIterator 
gral_begin(IstreamComplex2DFmt const& g, typename gt::VertexIterator)
{ return g.FirstVertex();}

template<class It>
inline typename gt::VertexIterator 
gral_end(IstreamComplex2DFmt const& g, typename gt::VertexIterator)
{ return g.EndVertex();}

template<class It>
inline typename gt::size_type
gral_size(IstreamComplex2DFmt const& g, typename gt::VertexIterator)
{ return g.NumOfVertices();}


template<class It>
inline typename gt::CellIterator 
gral_begin(IstreamComplex2DFmt const& g, typename gt::CellIterator)
{ return g.FirstCell();}

template<class It>
inline typename gt::CellIterator 
gral_end(IstreamComplex2DFmt const& g, typename gt::CellIterator)
{ return g.EndCell();}

template<class It>
inline typename gt::size_type
gral_size(IstreamComplex2DFmt const& g, typename gt::CellIterator)
{ return g.NumOfCells();}



template<class It>
inline typename gt::VertexOnCellIterator 
gral_begin(CellIterator_istream_complex2d const& g, typename gt::VertexOnCellIterator)
{ return g.FirstVertex();}

template<class It>
inline typename gt::VertexOnCellIterator 
gral_end  (CellIterator_istream_complex2d const& g, typename gt::VertexOnCellIterator)
{ return g.EndVertex();}

template<class It>
inline typename gt::size_type
gral_size (CellIterator_istream_complex2d const& g, typename gt::VertexOnCellIterator)
{ return g.NumOfVertices();}


#undef gt
#endif



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
  IstreamComplex2DCombFmt( ::std::istream      & in, int off = 0) : base(in,off) 
  { }
  IstreamComplex2DCombFmt( ::std::string  const& fn, int off = 0) : base(fn,off)
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


  // overloads of begin/end/size for IstreamComplex2DFmt


#ifdef gt
#error gt already in use (missing previous undef ?)
#else
#define gt grid_types<IstreamComplex2DCombFmt>

template<class It>
inline typename gt::VertexIterator 
gral_begin(IstreamComplex2DCombFmt const& g, typename gt::VertexIterator)
{ return g.FirstVertex();}

template<class It>
inline typename gt::VertexIterator 
gral_end(IstreamComplex2DCombFmt const& g, typename gt::VertexIterator)
{ return g.EndVertex();}

template<class It>
inline typename gt::size_type
gral_size(IstreamComplex2DCombFmt const& g, typename gt::VertexIterator)
{ return g.NumOfVertices();}


template<class It>
inline typename gt::CellIterator 
gral_begin(IstreamComplex2DCombFmt const& g, typename gt::CellIterator)
{ return g.FirstCell();}

template<class It>
inline typename gt::CellIterator 
gral_end(IstreamComplex2DCombFmt const& g, typename gt::CellIterator)
{ return g.EndCell();}

template<class It>
inline typename gt::size_type
gral_size(IstreamComplex2DCombFmt const& g, typename gt::CellIterator)
{ return g.NumOfCells();}

#undef gt
#endif

} // namespace GrAL 

#endif
