#ifndef GRAL_BASE_GB_ISTREAM_GRID_COMPLEX2D_FMT
#define GRAL_BASE_GB_ISTREAM_GRID_COMPLEX2D_FMT


// $LICENSE

// STD
#include <iostream>
#include <fstream>
#include <string>

// STL
#include <iterator.h>
#include <vector.h>

// GrAL Base
#include "Gral/Base/common-grid-basics.h"
#include "Gral/IO/write-complex2d.h"

#include "Geometry/coords.h"
#include "Utility/pre-post-conditions.h"

//----------------------------------------------------------------
/*! \defgroup complex2dformat Complex2D-Format I/O Adapter 
  \brief A grid interface for a file in complex2d-format.
  \ingroup io

 \example
 \code
 ifstream gridfile("my.complexd");
 IstreamComplex2DFmt Gfile(gridfile,1);  // 1 == offset
 ConstructGrid(G,Gfile,Gfile);
 \endcode 

 \warning
 The constructor modifies the stream, therefore \e never
 do the following:
 \code
 ConstructGrid(G, IstreamComplex2DFmt(in),
                  IstreamComplex2DFmt(in));
 
 \endcode

 <b> The complex2d format: </b>
 \code
 nv // number of vertices
 nc // number of cells

 x_1  y_1  // coords of vertex 1 
 ....
 x_nv y_nv // coords of vertex nv

 nvc1 //  number of vertices of cell 1
    vc1_1  ....  vc1_nvc1   // indices of vertices of cell 1 ( starting with 1)
 ....
 nvc_nc //  number of vertices of cell nc
    vc1_1  ....  vc1_nvc_nc // indices of vertices of cell nc ( starting with 1)
 \endcode
*/
//----------------------------------------------------------------

/*! \brief Output adapter for Complex2D Format
    \ingroup complex2dformat

    \see Module \ref complex2dformat
 */
class OstreamComplex2DFmt {
protected:
  std::ostream * out;
  bool      owned;
public: 
  OstreamComplex2DFmt(std::ostream& ot) 
    : out(&ot), owned(false) {}
  OstreamComplex2DFmt(std::string const& nm) 
    : out(new std::ofstream(nm.c_str())), owned(true) {}
  ~OstreamComplex2DFmt()
  { if(owned) delete out;}

  std::ostream& Out() { return *out;}
};

/*! \brief ConstructGrid overload for OstreamComplex2DFmt
  \ingroup complex2dformat
  \relates OstreamComplex2DFmt
  
  \see Module \ref complex2dformat
  \see Module \ref mutatingoperations
  \see \ref ConstructGrid
 */
template<class GRID>
void ConstructGrid0(OstreamComplex2DFmt& Out, 
		    GRID const& G)
{
  write_complex2d(G, Out.Out(), 0);
}
/*! \brief ConstructGrid overload for OstreamComplex2DFmt
  \ingroup complex2dformat
  \relates OstreamComplex2DFmt
  
  \see Module \ref complex2dformat
  \see Module \ref mutatingoperations
  \see \ref ConstructGrid
 */
template<class GRID, class GEOM>
void ConstructGrid(OstreamComplex2DFmt& Out, 
                   GRID const& G, GEOM const& Geom)
{
  write_complex2d(G, Geom, Out.Out(), 0);
}

/*! \brief ConstructGrid overload for OstreamComplex2DFmt
  \ingroup complex2dformat
  \relates OstreamComplex2DFmt
  
  Dummy output geometry variable.
  \see Module \ref complex2dformat
  \see Module \ref mutatingoperations
  \see \ref ConstructGrid
 */
template<class GRID, class GEOM>
void ConstructGrid(OstreamComplex2DFmt& Out, 
                   OstreamComplex2DFmt&, // dummy output geom
                   GRID const& G, GEOM const& Geom)
{
  write_complex2d(G, Geom, Out.Out(), 0);
}



class VertexIterator_istream_complex2d;
class CellIterator_istream_complex2d;
class VertexOnCellIterator_istream_complex2d;

/*! \brief Input adapter for Complex2D Format
    \ingroup complex2dformat
 
    Model of $GrAL Cell-VertexInputGridRange
   
    \see Module \ref complex2dformat
 */

class IstreamComplex2DFmt 
{
  friend class VertexIterator_istream_complex2d;
  friend class CellIterator_istream_complex2d;
  friend class VertexOnCellIterator_istream_complex2d;

  typedef istream_iterator<int, ptrdiff_t> conn_iter;
  typedef IstreamComplex2DFmt self;

public:
  typedef coordN<2>                              coord_type;

  typedef int                                    vertex_handle;
  typedef int                                    cell_handle;
  typedef VertexIterator_istream_complex2d       Vertex;
  typedef VertexIterator_istream_complex2d       VertexIterator;
  typedef CellIterator_istream_complex2d         Cell;
  typedef CellIterator_istream_complex2d         CellIterator;
  typedef VertexOnCellIterator_istream_complex2d VertexOnCellIterator;

private:
  //------------- DATA -------------
  std::istream * in;
  int   nv;
  int   nc;
  int   offset;
  std::vector<coord_type> coords;
public:

  //  IstreamComplex2DFmt(string const& file, int off = 0);
  IstreamComplex2DFmt(std::istream& is, int off = 0)
   : offset(off)
    {
      in = &is;
      (*in) >> nv >> nc;
      coords = std::vector<coord_type>(nv);
      for(int v = 0; v < nv; ++v)
	(*in) >> coords[v];
   }

  //void init();

  coord_type const& coord(Vertex const& v) const;

  unsigned NumOfVertices() const { return nv;}
  unsigned NumOfCells()    const { return nc;}

  VertexIterator FirstVertex() const;
  CellIterator   FirstCell()   const;
};

/*! \brief Vertex iterator for IstreamComplex2DFmt
    \ingroup complex2dformat

    Model of $GrAL GridVertexIterator

    \see Module \ref complex2dformat
    \see IstreamComplex2DFmt
 */
class VertexIterator_istream_complex2d 
{
  typedef  IstreamComplex2DFmt grid_type;
  typedef  VertexIterator_istream_complex2d   self;
  typedef  grid_type::coord_type              coord_type;
private:
  IstreamComplex2DFmt* g;
  int v;
  //  coord_type p;
 
public:
  VertexIterator_istream_complex2d() : g(0), v(-1) {}
  VertexIterator_istream_complex2d(grid_type const& gg) 
    : g(const_cast<grid_type *>(&gg)), v(0) 
    { 
      //  if( !IsDone())
      //	*(g->in) >> p;
    }
  VertexIterator_istream_complex2d(grid_type & gg, int vv)
    : g(const_cast<grid_type *>(&gg)), v(vv) 
    {
      assure_valid();
    } 
  

  self& operator++() {
    assure_valid();
    ++v;
    //    if( ! IsDone())
    //  *(g->in) >> p;
    return *this;
  }

  self& operator*()  { return *this;}
  bool IsDone() const { return (v >= (int)g->NumOfVertices());}
  operator bool() const { return !IsDone();}
  int  handle() const { return v;}
  coord_type const& coord() const { return (g->coords[v]);}

  grid_type const& TheGrid() const { return *g;}

  void assure_valid() {
    ENSURE((( g != 0) && ( v >= 0) && (v < g->nv)),
	   "invalid vertex iterator!",1);
  }

};


/*! \brief Cell iterator for IstreamComplex2DFmt
    \ingroup complex2dformat

    Model of $GrAL GridCellIterator

    \see Module \ref complex2dformat
    \see IstreamComplex2DFmt
 */
class CellIterator_istream_complex2d {
  typedef  IstreamComplex2DFmt grid_type;
  typedef  CellIterator_istream_complex2d     self;
private:
  IstreamComplex2DFmt* g;
  int  c;
  int nv;
public:
  CellIterator_istream_complex2d()               : g(0), c(-1) {}
  CellIterator_istream_complex2d(grid_type const& gg) 
    : g(const_cast<grid_type*>(&gg)), c(0)
    {
      if(!IsDone())
	*(g->in) >> nv;
    }

  self& operator++() { 
   ++c;
   if(!IsDone())
     *(g->in) >> nv;
   return *this;
  }
  self& operator*() { return *this;}
  bool IsDone() const { return (c >= (int)g->NumOfCells());}
  operator bool() const { return !IsDone();}
  grid_type const& TheGrid() const { return *g;}
  int NumOfVertices() const { return nv;}
  int handle()  const { return c;}

  VertexOnCellIterator_istream_complex2d FirstVertex() const;
};




/*! \brief VertexOnCell iterator for IstreamComplex2DFmt
    \ingroup complex2dformat

    Model of $GrAL Vertex-On-CellIterator

    \see Module \ref complex2dformat
    \see IstreamComplex2DFmt
 */
class VertexOnCellIterator_istream_complex2d {
  typedef  IstreamComplex2DFmt      grid_type;
  typedef  VertexIterator_istream_complex2d        Vertex;
  typedef  VertexOnCellIterator_istream_complex2d  self;
private:
  grid_type *g;
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
    : g(const_cast<grid_type *>(&gg)),
      nv(NV),
      c(C),
      lv(0)
    {
      if(!IsDone())
	*(g->in) >> vc;
    }
  
  self& operator++() {
    ++lv;
    if(! IsDone())
      *(g->in) >> vc;
    return *this;
  }

  bool    IsDone()       const { return (lv >= nv);}
  operator bool() const { return !IsDone();}
  Vertex  operator*()    const { assure_valid(); return Vertex(*g,vc-g->offset);}
  int     handle()       const { assure_valid(); return vc - g->offset;}
  grid_type const& TheGrid() const { return *g;}

  void assure_valid() const {
    ENSURE((( g != 0) && ( vc - g->offset >= 0) && (vc - g->offset < g->nv)),
	   " invalid vertex-on-cell iterator: vc =" << vc 
           << ", offset = " << g->offset << ", nv = " << g->nv << '\n',1);
  }

};



inline
IstreamComplex2DFmt::coord_type const& 
IstreamComplex2DFmt::coord(VertexIterator_istream_complex2d const& v) const 
{ return v.coord();}

inline 
VertexIterator_istream_complex2d
IstreamComplex2DFmt::FirstVertex() const
{ return VertexIterator( (self&)*this);}

inline 
CellIterator_istream_complex2d
IstreamComplex2DFmt::FirstCell()  const
{ return CellIterator( (self &)*this);}

inline 
VertexOnCellIterator_istream_complex2d
CellIterator_istream_complex2d::FirstVertex() const 
{ return VertexOnCellIterator_istream_complex2d(*g,nv,c); }


/*! \brief Specialization of  \c grid_types  for IstreamComplex2DFmt
    \ingroup complex2dformat


    \see Module \ref complex2dformat
    \see Module \ref traits (\c grid_types)
    \see IstreamComplex2DFmt
 */
struct grid_types<IstreamComplex2DFmt>
{
  typedef IstreamComplex2DFmt             grid_type;

  typedef  grid_type::Vertex               Vertex;
  typedef  grid_type::VertexIterator       VertexIterator;
  typedef  grid_type::VertexOnCellIterator VertexOnCellIterator;
  typedef  grid_type::Cell                 Cell;
  typedef  grid_type::CellIterator         CellIterator;
  typedef  grid_type::vertex_handle        vertex_handle;
  typedef  grid_type::cell_handle          cell_handle;
};

#endif
