#ifndef NMWR_GB_STREAM_GRID_ADAPTER_H
#define NMWR_GB_STREAM_GRID_ADAPTER_H


// $LICENSE

#include "Gral/Base/common-grid-basics.h"

//----------------------------------------------------------------
/*! \defgroup streamgrid Adapter for serial grid representation
    \ingroup gridio
    \see Module gridio

   \brief  Grid-like iterator from a flat integer data stream.

  The format of the serial representation is as follows:
  - Cells / vertices are implicitely numerated 0 ... nc-1 / nv-1
  - The integer sequence contains the values
    - NV_0                  ( # vertices of cell 0)
    - LV1_0  ... LV1_NV_0   ( global numbers of the NV_0 vertices of cell 0) <BR>
      ....    <BR>
      NV_nc_1               ( # vertices of cell nc-1)
    - LVnc_1 ... LVnc_NV_nc ( global numbers of the NV_nc vertices of cell nc-1)
  
  This can be used to construct grids from files, message passing
  buffers and other sequentialized contexts.
*/
//----------------------------------------------------------------

template<class It>
class stream_grid_mask;

template<class It>
class stream_grid_mask_vertex_on_cell;

template<class It>
class stream_geom_mask;

//----------------------------------------------------------------
//                 Vertex / VertexIterator
//----------------------------------------------------------------

/*! \ingroup  streamgrid

 */
template<class It>
class stream_grid_mask_vertex {
  typedef stream_grid_mask_vertex<It> self;
private:
  typedef stream_grid_mask<It> grid_type;
  friend  class stream_grid_mask<It>;

  const stream_grid_mask<It>* the_grid;
  int v;
public:
  stream_grid_mask_vertex(const grid_type& g)
    { *this = g.FirstVertex();}
  stream_grid_mask_vertex(const grid_type& g, int vv)
    : the_grid(&g), v(vv) {}
  
  // iterator op.
  self& operator++() { ++v; return *this;}
  bool IsDone() const;
  self operator*() const { return *this;}  
  int  handle() const { return v;}
  grid_type const& TheGrid() const { return *the_grid;}
};


//----------------------------------------------------------------
//                    Cell / CellIterator
//----------------------------------------------------------------


/*! \ingroup  streamgrid

 */
template<class It>
class stream_grid_mask_cell {
  typedef stream_grid_mask_vertex_on_cell<It> VertexOnCellIterator;
  typedef stream_grid_mask_cell<It>           self;
  typedef stream_grid_mask<It>                grid_type;
  friend  class  stream_grid_mask<It>;
private:
  const stream_grid_mask<It>* the_grid;
  int c;
  int numv;
  It  vertices;
public:
  stream_grid_mask_cell(const grid_type& g)
    { *this = g.FirstCell();}
  stream_grid_mask_cell(const grid_type& g, int cc, It v) 
    : the_grid(&g), c(cc), numv(*v), vertices(v) {}
  
  // iterator op.
  self& operator++() {
    advance(vertices,numv+1);
    ++c;
    if(!IsDone()) 
      numv = *vertices;
    return *this;
  }

  bool IsDone() const ;
  self operator*() const { return *this;}  
  
  int NumOfVertices() const {  return numv;}
  VertexOnCellIterator FirstVertex() const;
  int handle() const { return c;}
  grid_type const& TheGrid() const { return *the_grid;}
};



//----------------------------------------------------------------
//                 VertexOnCellIterator
//----------------------------------------------------------------

/*! \ingroup  streamgrid

 */
template<class It>
class stream_grid_mask_vertex_on_cell {
  typedef stream_grid_mask<It>         grid_type;
  typedef stream_grid_mask_cell<It>    Cell;
  typedef stream_grid_mask_vertex<It>  Vertex;

private:
  const grid_type* the_grid;
  int c;
  int numv;
  It  vertices;
  int cnt;

  typedef stream_grid_mask_vertex_on_cell<It> self;
public:
  stream_grid_mask_vertex_on_cell(Cell const& cc)
    { *this = cc.FirstVertex();}
  stream_grid_mask_vertex_on_cell(const grid_type& g, int cc, It v) 
    : the_grid(&g), c(cc), numv(*v), vertices(++v), cnt(1) {}
  
  self&  operator++()      { ++vertices; ++cnt; return (*this);  }
  bool   IsDone()    const { return (cnt > numv); }
  Vertex operator*() const { return Vertex(*the_grid,*vertices); }

  int handle() const { return *vertices;}
  //  Cell TheCell()
};


//----------------------------------------------------------------
//                         Grid
//----------------------------------------------------------------

/*! \ingroup  streamgrid

 */
template<class It>
class stream_grid_mask {
private:
  int numv;
  int numc;
  It  it;
  int off;

  friend class stream_grid_mask_vertex<It>;
  friend class stream_grid_mask_cell<It>;
public:
  stream_grid_mask(int nv, int nc, It i) : numv(nv), numc(nc), it(i), off(0) {}
  stream_grid_mask(int nv, int nc, It i, int of) : numv(nv), numc(nc), it(i), off(of) {}
  void set_offset(int o) { off = o;}
  int offset() const { return off;}
  typedef stream_grid_mask_cell<It>   Cell;
  typedef stream_grid_mask_cell<It>   CellIterator;
  typedef stream_grid_mask_vertex<It> Vertex;
  typedef stream_grid_mask_vertex<It> VertexIterator;
  typedef stream_grid_mask_vertex_on_cell<It> VertexOnCellIterator;

  typedef int vertex_handle;
  typedef int cell_handle;

  cell_handle   handle(const Cell& C)   const { return C.c;}
  vertex_handle handle(const Vertex& V) const { return V.v;}
  vertex_handle handle(const VertexOnCellIterator& V) const { return handle(*V);}

  int NumOfVertices() const { return numv;}
  int NumOfCells()    const { return numc;}

  CellIterator   FirstCell()   const { return Cell(*this,offset(),it);}
  VertexIterator FirstVertex() const { return Vertex(*this,offset());}
};


/*! \ingroup  streamgrid

 */
template<class It>
inline
stream_grid_mask<It>
StreamGridMask(int nv, int nc, It it)
{ return stream_grid_mask<It>(nv,nc,it);}

/*! \ingroup  streamgrid

 */
template<class It>
inline
stream_grid_mask<It>
StreamGridMask(int nv, int nc, It it, int off)
{ return stream_grid_mask<It>(nv,nc,it,off);}


//----------------------------------------------------------------
//                     grid_types<>
//----------------------------------------------------------------


template<class It>
struct grid_types_help_stream_grid_mask  {
  typedef stream_grid_mask<It> Grid;

  typedef typename Grid::Vertex               Vertex;
  typedef typename Grid::VertexIterator       VertexIterator;
  typedef typename Grid::VertexOnCellIterator VertexOnCellIterator;
  typedef typename Grid::Cell                 Cell;
  typedef typename Grid::CellIterator         CellIterator;
  typedef typename Grid::vertex_handle        vertex_handle;
  typedef typename Grid::cell_handle          cell_handle;

};

template<class It>
struct grid_types<stream_grid_mask< It > > 
 : public grid_types_help_stream_grid_mask< It > {};


template<class It>
inline bool stream_grid_mask_vertex<It>::IsDone() const 
{ return (v >= the_grid->NumOfVertices() + the_grid->offset());}

template<class It>
inline bool stream_grid_mask_cell<It>::IsDone() const 
{ return (c >= the_grid->NumOfCells() + the_grid->offset());}

template<class It>
inline stream_grid_mask_vertex_on_cell<It>
stream_grid_mask_cell<It>::FirstVertex() const 
{ return VertexOnCellIterator(*the_grid,c,vertices);}


/*! \ingroup streamgrid

 */
template<class It>
class stream_geom_mask {
private:
  It begin;
public:
  typedef iterator_traits<It>         traits;
  typedef typename traits::value_type coord_type;

  stream_geom_mask(It b) : begin(b) {}

  template<class It2>
  coord_type const& coord(stream_grid_mask_vertex<It2> const& v) const 
  { return begin[v.handle() - v.TheGrid().offset()];}
};

/*! \ingroup streamgrid

 */
template<class It>
inline
stream_geom_mask<It>
StreamGeomMask(It it) { return stream_geom_mask<It>(it);}

#endif
