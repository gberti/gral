#ifndef NMWR_GB_STREAM_GRID_ADAPTER_H
#define NMWR_GB_STREAM_GRID_ADAPTER_H


// $LICENSE

#include "Gral/Base/common-grid-basics.h"
#include "Gral/Base/grid-function-vector.h"
#include "Gral/Base/polygon.h"

#include "Utility/ref-ptr.h"

#include <iterator>
#include <vector>

namespace GrAL {

/*! \defgroup streamgrid Adapter for serial 2D grid representation
    \ingroup gridio
    \see Module gridio

   \brief  Grid-like iterator from a flat integer data stream.

  The format of the serial representation is as follows:
  - Cells / vertices are implicitely numbered 0 ... nc-1 / nv-1
    (nc = number of cells, nv = number of vertices)
  - The integer sequence contains the values
    - NV_0                  ( # vertices of cell 0)
    - LV1_0  ... LV1_NV_0   ( global numbers of the NV_0 vertices of cell 0) <BR>
      ....    <BR>
      NV_nc_1               ( # vertices of cell nc-1)
    - LVnc_1 ... LVnc_NV_nc ( global numbers of the NV_nc vertices of cell nc-1)
  
  This can be used to construct grids from files, message passing
  buffers, literal arrays and other sequentialized contexts.
 
  Tested in \ref test-stream-grid-adapter.C 
*/


template<class It> class stream_grid_mask;

template<class It> class stream_grid_mask_vertex;
template<class It> class stream_grid_mask_cell;
template<class It> class stream_grid_mask_vertex_on_cell;

template<class It> class stream_geom_mask;


template<class It> 
struct grid_types_stream_grid_mask
{
  typedef stream_grid_mask<It> grid_type;
  typedef stream_grid_mask_vertex<It> Vertex;
  typedef stream_grid_mask_vertex<It> VertexIterator;
  typedef stream_grid_mask_cell<It>   Cell;
  typedef stream_grid_mask_cell<It>   CellIterator;
  typedef stream_grid_mask_vertex_on_cell<It>   VertexOnCellIterator;

  typedef int vertex_handle;
  typedef int cell_handle;

  typedef grid_dim_tag<2>   dimension_tag;

  typedef polygon1d::polygon                          archetype_type;
  typedef std::vector<archetype_type>::const_iterator archetype_iterator;
  typedef int                                         archetype_handle;

};
//----------------------------------------------------------------
//                 Vertex / VertexIterator
//----------------------------------------------------------------

/*! \internal
    \ingroup  streamgrid

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


/*! \internal
    \ingroup  streamgrid

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
     ::std::advance(vertices,numv+1);
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

/*! \internal
    \ingroup  streamgrid

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

/*! \brief Grid defined by an integer stream

    \ingroup  streamgrid
    \see \ref streamgrid module
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

  typedef grid_types_stream_grid_mask<It> gt;

public:
  stream_grid_mask(int nv, int nc, It i) : numv(nv), numc(nc), it(i), off(0) { init(); }
  stream_grid_mask(int nv, int nc, It i, int of) : numv(nv), numc(nc), it(i), off(of) { init(); }

  void init() { init_archetypes();}
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

  typedef typename gt::archetype_type     archetype_type;
  typedef typename gt::archetype_iterator archetype_iterator;
  typedef typename gt::archetype_handle   archetype_handle;
private:
  std::vector<archetype_type> archetypes;
  std::vector<int>            arch_for_n_vertices;
  void init_archetypes();
public:
  archetype_iterator BeginArchetype()  const { return archetypes.begin();}
  archetype_iterator EndArchetype()    const { return archetypes.end();}
  unsigned           NumOfArchetypes() const { return archetypes.size(); }
  archetype_handle   handle(archetype_iterator it) const { return it - BeginArchetype();}

  archetype_type const& Archetype(archetype_handle a) const { return archetypes[a];}
  archetype_type      & Archetype(archetype_handle a)       { return archetypes[a];}
  archetype_type const& ArchetypeOf (Cell const& c) const 
  { return Archetype(archetype_of(c));}
  /* these cannot be handled - no way to efficiently construct cell out of handle
  archetype_type   const& ArchetypeOf (cell_handle c) const 
  { return ArchetypeOf(Cell(*this, c));}
  archetype_handle        archetype_of(cell_handle c) const 
  { return archetype_of(Cell(*this, c)); }
  */
  archetype_handle        archetype_of(Cell const& c) const 
  { return arch_for_n_vertices[c.NumOfVertices()];}

};


/*! \brief Creator function for stream_grid_mask

   \ingroup  streamgrid
 */
template<class It>
inline
stream_grid_mask<It>
StreamGridMask(int nv, int nc, It it)
{ return stream_grid_mask<It>(nv,nc,it);}

/*!  \brief Creator function for stream_grid_mask

    \ingroup  streamgrid
 */
template<class It>
inline
stream_grid_mask<It>
StreamGridMask(int nv, int nc, It it, int off)
{ return stream_grid_mask<It>(nv,nc,it,off);}




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


/*! \brief Geometry for stream_grid_mask 

   \ingroup streamgrid
   \see \ref streamgrid module

 */
template<class It>
class stream_geom_mask {
private:
  It begin;
public:
  typedef std::iterator_traits<It>    traits;
  typedef typename traits::value_type coord_type;

  stream_geom_mask(It b) : begin(b) {}

  template<class It2>
  coord_type const& coord(stream_grid_mask_vertex<It2> const& v) const 
  { return begin[v.handle() - v.TheGrid().offset()];}
};

/*! \brief Creator function for stream_geom_mask 

   \ingroup streamgrid

 */
template<class It>
inline
stream_geom_mask<It>
StreamGeomMask(It it) { return stream_geom_mask<It>(it);}



  //----------------------------------------------------------------------
  //     specializations of traits classes & grid functions 
  //----------------------------------------------------------------------

template<class It>
struct grid_types<stream_grid_mask<It> >
  : public grid_types_base<grid_types_stream_grid_mask<It> >
{};

template<class It>
struct element_traits<stream_grid_mask_vertex<It> >
  : public element_traits_vertex_base<stream_grid_mask<It> >
{
  typedef element_traits_vertex_base<stream_grid_mask<It> > base;
  
  struct hasher_type : public base::hasher_type_elem_base {
    
    typedef typename base::hasher_type_elem_base base_hash;
    typename base_hash::result_type operator()(typename base::element_type const& e) const { return e.handle();}
  };
  typedef consecutive_integer_tag<0> consecutive_tag;
};

template<class It>
struct element_traits<stream_grid_mask_cell<It> >
  : public element_traits_cell_base<stream_grid_mask<It> >
{
  typedef element_traits_cell_base<stream_grid_mask<It> > base;
  
  struct hasher_type : public base::hasher_type_elem_base {
    
    typedef typename base::hasher_type_elem_base base_hash;
    typename base_hash::result_type operator()(typename base::element_type const& e) const { return e.handle();}
  };
  typedef consecutive_integer_tag<0> consecutive_tag;
};

  template<class It, class T>
  class grid_function<stream_grid_mask_vertex<It>, T>
    : public grid_function_vector<stream_grid_mask_vertex<It>, T>
  {
    typedef grid_function_vector<stream_grid_mask_vertex<It>, T> base;
  public:
    typedef typename base::grid_type grid_type;

    grid_function() {}
    grid_function(grid_type const& gg) : base(gg) {}
    grid_function(grid_type const& gg, T const& t) : base(gg,t) {}
    grid_function(ref_ptr<grid_type const> gg) : base(gg) {}
    grid_function(ref_ptr<grid_type const> gg, T const& t) : base(gg,t) {}
  };

  template<class It, class T>
  class grid_function<stream_grid_mask_cell<It>, T>
    : public grid_function_vector<stream_grid_mask_cell<It>, T>
  {
    typedef grid_function_vector<stream_grid_mask_cell<It>, T> base;
  public:
    typedef typename base::grid_type grid_type;

    grid_function() {}
    grid_function(grid_type const& gg) : base(gg) {}
    grid_function(grid_type const& gg, T const& t) : base(gg,t) {}
    grid_function(ref_ptr<grid_type const> gg) : base(gg) {}
    grid_function(ref_ptr<grid_type const> gg, T const& t) : base(gg,t) {}
  };


  template<class It>
  void stream_grid_mask<It>::init_archetypes() 
  {
    archetypes         .clear();
    arch_for_n_vertices.clear();
    for(CellIterator c(*this); ! c.IsDone(); ++c) {
      int nv     = c.NumOfVertices();
      // size() may be 0!
      int      max_nv = arch_for_n_vertices.size() -1;
      if(max_nv < nv)
	arch_for_n_vertices.resize(nv+1, -1);
      if( arch_for_n_vertices[nv] == -1)  {
	archetypes.push_back(archetype_type(nv));
	arch_for_n_vertices[nv] = archetypes.size()-1;
      }
    }
  }
} // namespace GrAL 

#endif
