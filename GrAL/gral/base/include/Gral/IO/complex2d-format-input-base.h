#ifndef GRAL_BASE_GB_ISTREAM_GRID_COMPLEX2D_FMT
#define GRAL_BASE_GB_ISTREAM_GRID_COMPLEX2D_FMT


// $LICENSE

// STD
#include <iostream>
#include <fstream>
#include <string>


// GrAL Base
#include "Gral/Base/common-grid-basics.h"
#include "Gral/Base/grid-function-vector.h"

// Misc
#include "Utility/pre-post-conditions.h"
#include "Utility/checked-istream.h"




class VertexIterator_istream_complex2d;
class CellIterator_istream_complex2d;
class VertexOnCellIterator_istream_complex2d;



class IstreamComplex2DFmt_base 
{
  typedef IstreamComplex2DFmt_base self;

  friend class VertexIterator_istream_complex2d;
  friend class CellIterator_istream_complex2d;
  friend class VertexOnCellIterator_istream_complex2d;

public:
  typedef int                                    vertex_handle;
  typedef int                                    cell_handle;
  typedef VertexIterator_istream_complex2d       Vertex;
  typedef VertexIterator_istream_complex2d       VertexIterator;
  typedef CellIterator_istream_complex2d         Cell;
  typedef CellIterator_istream_complex2d         CellIterator;
  typedef VertexOnCellIterator_istream_complex2d VertexOnCellIterator;

private:
  //------------- DATA -------------
  int               offset; // start of vertex numbering 
  mutable std::istream    * in;
  mutable checked_istream   checked_in;       
  bool              is_in_owned;

  int   nv; // number of vertices
  int   nc; // number of cells
public:
  IstreamComplex2DFmt_base();
  IstreamComplex2DFmt_base(std::string const& file, int off = 0);
  IstreamComplex2DFmt_base(std::istream     & is,   int off = 0);

  IstreamComplex2DFmt_base(self const& rhs) {                  copy(rhs); }
  self & operator=        (self const& rhs) { if(this != &rhs) copy(rhs); return *this;}

  virtual ~IstreamComplex2DFmt_base();


  void init(std::string const& file); 
protected:
  void copy(self const& rhs);
private:
  void init();

protected:
  checked_istream & In() const { return checked_in;}
public:
  unsigned NumOfVertices() const { return nv;}
  unsigned NumOfCells()    const { return nc;}

  VertexIterator FirstVertex() const;
  CellIterator   FirstCell()   const;

  Vertex vertex(vertex_handle v) const;
  // this would require random access to cells which is not possible!
  //  Cell   cell  (cell_handle   c) const;
};

/*! \brief Vertex iterator for IstreamComplex2DFmt_base
    \ingroup complex2dformat

    Model of $GrAL GridVertexIterator

    \see Module \ref complex2dformat
    \see IstreamComplex2DFmt_base
 */
class VertexIterator_istream_complex2d 
{
  typedef  IstreamComplex2DFmt_base grid_type;
  typedef  VertexIterator_istream_complex2d   self;
private:
  grid_type const* g;
  int v; 
 
public:
  VertexIterator_istream_complex2d() : g(0), v(-1) {}
  VertexIterator_istream_complex2d(grid_type const& gg) 
    : g(&gg), v(0) 
    { }
  VertexIterator_istream_complex2d(grid_type const& gg, int vv)
    : g(&gg), v(vv) 
    { } 
  

  self& operator++() {
    assure_valid();
    ++v;
    return *this;
  }

  self const& operator*() const  { assure_valid(); return *this;}
  bool IsDone() const { return (v >= (int)g->NumOfVertices());}
  int  handle() const { return v;}

  grid_type const& TheGrid() const { return *g;}

  void assure_valid() const {
    ENSURE((( g != 0) && ( v >= 0) && (v < g->nv)),
	   "invalid vertex iterator!",1);
  }

};


/*! \brief Cell iterator for IstreamComplex2DFmt_base
    \ingroup complex2dformat

    Model of $GrAL GridCellIterator

    \see Module \ref complex2dformat
    \see IstreamComplex2DFmt_base
 */
class CellIterator_istream_complex2d {
  typedef  IstreamComplex2DFmt_base              grid_type;
  typedef  CellIterator_istream_complex2d   self;
private:
  grid_type const* g;
  int  c; // current cell number
  int nv; // number of vertices
public:
  CellIterator_istream_complex2d()               : g(0), c(-1) {}
  CellIterator_istream_complex2d(grid_type const& gg) 
    : g(&gg), c(0)
    {
      if(!IsDone())
	g->In() >> nv;
    }

  self& operator++() { 
   ++c;
   if(!IsDone())
     g->In() >> nv;
   return *this;
  }
  self const& operator*() const { return *this;}
  bool IsDone() const { return (c >= (int)g->NumOfCells());}
  grid_type const& TheGrid() const { return *g;}
  int NumOfVertices() const { return nv;}
  int handle()  const { return c;}

  VertexOnCellIterator_istream_complex2d FirstVertex() const;
};




/*! \brief VertexOnCell iterator for IstreamComplex2DFmt_base
    \ingroup complex2dformat

    Model of $GrAL Vertex-On-CellIterator

    \see Module \ref complex2dformat
    \see IstreamComplex2DFmt_base
 */
class VertexOnCellIterator_istream_complex2d {
  typedef  IstreamComplex2DFmt_base      grid_type;
  typedef  VertexIterator_istream_complex2d        Vertex;
  typedef  VertexOnCellIterator_istream_complex2d  self;
private:
  grid_type const* g;
  int nv;
  int  c;
  int lv; // local vertex  number
  int vc; // global vertex number of lv;
public:
  VertexOnCellIterator_istream_complex2d() 
    : g(0), nv(-1), c(-1) 
    {}
  VertexOnCellIterator_istream_complex2d(CellIterator_istream_complex2d const& cc)
  { *this = cc.FirstVertex();}

  VertexOnCellIterator_istream_complex2d(grid_type const& gg,
					 int NV, int C)
    : g(&gg),
      nv(NV),
      c(C),
      lv(0)
    {
      if(!IsDone())
	g->In() >> vc;
    }
  
  self& operator++() {
    ++lv;
    if(! IsDone())
      g->In() >> vc;
    return *this;
  }

  bool    IsDone()       const { return (lv >= nv);}
  Vertex  operator*()    const { assure_valid(); return Vertex(*g,vc-g->offset);}
  int     handle()       const { assure_valid(); return vc - g->offset;}
  grid_type const& TheGrid() const { return *g;}

  void assure_valid() const {
    ENSURE((( g != 0) && ( vc - g->offset >= 0) && (vc - g->offset < g->nv)),
	   " invalid vertex-on-cell iterator: vc =" << vc 
           << ", offset = " << g->offset << ", nv = " << g->nv << '\n',1);
  }

};


/*
inline
IstreamComplex2DFmt_base::coord_type const& 
IstreamComplex2DFmt_base::coord(VertexIterator_istream_complex2d const& v) const 
{ return v.coord();}
*/
inline 
VertexIterator_istream_complex2d
IstreamComplex2DFmt_base::FirstVertex() const
{ return VertexIterator( (self&)*this);}

inline 
CellIterator_istream_complex2d
IstreamComplex2DFmt_base::FirstCell()  const
{ return CellIterator( (self &)*this);}

inline 
IstreamComplex2DFmt_base::Vertex
IstreamComplex2DFmt_base::vertex(IstreamComplex2DFmt_base::vertex_handle v) const
{ return Vertex( (self&)*this, v);}

/*
inline 
IstreamComplex2DFmt_base::Cell
IstreamComplex2DFmt_base::cell(IstreamComplex2DFmt_base::cell_handle c) const
{ return Cell( (self&)*this, c);}
*/


inline 
VertexOnCellIterator_istream_complex2d
CellIterator_istream_complex2d::FirstVertex() const 
{ return VertexOnCellIterator_istream_complex2d(*g,nv,c); }


/*! \brief Specialization of  \c grid_types  for IstreamComplex2DFmt_base
    \ingroup complex2dformat


    \see Module \ref complex2dformat
    \see Module \ref traits (\c grid_types)
    \see IstreamComplex2DFmt_base
 */
struct grid_types<IstreamComplex2DFmt_base>
{
  typedef IstreamComplex2DFmt_base             grid_type;

  typedef  grid_type::Vertex               Vertex;
  typedef  grid_type::VertexIterator       VertexIterator;
  typedef  grid_type::VertexOnCellIterator VertexOnCellIterator;
  typedef  grid_type::Cell                 Cell;
  typedef  grid_type::CellIterator         CellIterator;
  typedef  grid_type::vertex_handle        vertex_handle;
  typedef  grid_type::cell_handle          cell_handle;
};

template<>
struct element_traits<VertexIterator_istream_complex2d>
  : public element_traits_vertex_base<IstreamComplex2DFmt_base> 
{
  struct hasher_type {
    unsigned operator()(element_type const& e) const { return e.handle();}
  };
};

template<>
struct element_traits<CellIterator_istream_complex2d>
  : public element_traits_cell_base<IstreamComplex2DFmt_base> 
{
  struct hasher_type {
    unsigned operator()(element_type const& e) const { return e.handle();}
  };
};

template<class T>
class grid_function<VertexIterator_istream_complex2d,T>
  : public grid_function_vector<VertexIterator_istream_complex2d,T>
{
  typedef grid_function_vector<VertexIterator_istream_complex2d,T> base;
public: 
  grid_function() {}
  grid_function(grid_type const& gg) : base(gg) {}
  grid_function(grid_type const& gg, T const& tt) : base(gg,tt) {}
};

template<class T>
class grid_function<CellIterator_istream_complex2d,T>
  : public grid_function_vector<CellIterator_istream_complex2d,T>
{
  typedef grid_function_vector<CellIterator_istream_complex2d,T> base;
public: 
  grid_function() {}
  grid_function(grid_type const& gg) : base(gg) {}
  grid_function(grid_type const& gg, T const& tt) : base(gg,tt) {}
};



#endif
