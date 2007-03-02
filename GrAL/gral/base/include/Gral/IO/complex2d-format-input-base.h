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
#include "Utility/ref-ptr.h"

namespace GrAL {


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
  mutable ref_ptr<std::istream>     in;
  mutable checked_istream           checked_in;       

  mutable int   nv; // number of vertices
  mutable int   nc; // number of cells
  mutable bool  nv_nc_read;
  // mutable  bool  vertex_iter_instance;
  mutable  bool  cell_iter_instance;

public:
  IstreamComplex2DFmt_base();
  IstreamComplex2DFmt_base(std::string const& file, int off = 0);
  IstreamComplex2DFmt_base(std::istream        & is,   int off = 0);
  IstreamComplex2DFmt_base(ref_ptr<std::istream> is,   int off = 0);

  IstreamComplex2DFmt_base(self const& rhs) {                  copy(rhs); }
  self & operator=        (self const& rhs) { if(this != &rhs) copy(rhs); return *this;}

  virtual ~IstreamComplex2DFmt_base();


  virtual void init(std::string const& file); 
protected:
  void copy(self const& rhs);
  virtual void init()        const;
  virtual bool initialized() const;


  checked_istream & In() const { return checked_in;}

  /*
  void set_vertex_iter_initialized() const
    {
      REQUIRE(! vertex_iter_instance, 
	      "Can pass only once trough vertices!\n",1);
      vertex_iter_instance = true;
    }
  */
  void set_cell_iter_initialized() const
    {
      REQUIRE(! cell_iter_instance, 
	      "Can pass only once trough cells!\n",1);
      cell_iter_instance = true;
    }

public:
  unsigned NumOfVertices() const { self::init(); return nv;}
  unsigned NumOfCells()    const { self::init(); return nc;}

  VertexIterator FirstVertex() const;
  VertexIterator EndVertex()   const;
  CellIterator   FirstCell()   const;
  CellIterator   EndCell()     const;

  Vertex vertex(vertex_handle v) const;
  // this would require random access to cells which is not possible!
  //  Cell   cell  (cell_handle   c) const;
};

/*! \brief Vertex iterator for IstreamComplex2DFmt_base
    \ingroup complex2dformat

    Model of $GrAL VertexIterator

    \see Module \ref complex2dformat
    \see IstreamComplex2DFmt_base
 */
class VertexIterator_istream_complex2d 
{
  friend class  IstreamComplex2DFmt_base;
  friend class  VertexOnCellIterator_istream_complex2d; 
 typedef  VertexIterator_istream_complex2d   self;
public:
  typedef  IstreamComplex2DFmt_base grid_type;

  typedef grid_type anchor_type;
  typedef self      value_type;
private:
  grid_type const* g;
  int v; 
 
public:
  VertexIterator_istream_complex2d() : g(0), v(-1) {}
  VertexIterator_istream_complex2d(grid_type const& gg) 
    : g(&gg), v(0) 
    { 
      g->init(); 
    }
  // private:
  // jumping into the middle is in general not possible, only exception: past-the-end iterator
  // however, vertices are read completely by derived grid class ...
  VertexIterator_istream_complex2d(grid_type const& gg, int vv) : g(&gg), v(vv) { }
public:

  self& operator++() {
    cv();
    ++v;
    return *this;
  }

  self const& operator*() const  { cv(); return *this;}
  bool IsDone() const { cb(); return (v >= (int)g->NumOfVertices());}
  int  handle() const { cv(); return v;}

  bool operator==(self const& rhs) const { return v == rhs.v;}
  bool operator!=(self const& rhs) const { return v != rhs.v;}
  bool operator< (self const& rhs) const { return v <  rhs.v;}

  grid_type   const& TheGrid()   const { return *g;}
  anchor_type const& TheAnchor() const { return *g;}

  bool bound() const { return g!=0;}
  bool valid() const { return bound() && !IsDone();}
  void cb()    const { REQUIRE(bound(), "", 1);}
  void cv()    const { REQUIRE(valid(), "", 1);}
};


/*! \brief Cell iterator for IstreamComplex2DFmt_base
    \ingroup complex2dformat

    Model of $GrAL CellIterator

    \see Module \ref complex2dformat
    \see IstreamComplex2DFmt_base
 */
class CellIterator_istream_complex2d {
  friend class  IstreamComplex2DFmt_base;
  typedef       CellIterator_istream_complex2d   self;
public:
  typedef  IstreamComplex2DFmt_base  grid_type;

  typedef grid_type anchor_type;
  typedef self      value_type;
private:
  grid_type const* g;
  int  c; // current cell number
  int nv; // number of vertices
public:
  CellIterator_istream_complex2d()               : g(0), c(-1) {}
  CellIterator_istream_complex2d(grid_type const& gg) 
    : g(&gg), c(0)
    {
      g->init();
      g->set_cell_iter_initialized();
      if(!IsDone())
	g->In() >> nv;
    }
private:
  // jumping into the middle is in general not possible, only exception: past-the-end iterator
  CellIterator_istream_complex2d(grid_type const& gg, int cc) : g(&gg), c(cc), nv(-1) {}
public:

  self& operator++() { 
   ++c;
   if(!IsDone())
     g->In() >> nv;
   return *this;
  }
  self const& operator*() const { return *this;}
  bool IsDone() const { return (c >= (int)g->NumOfCells());}
  int  NumOfVertices() const { return nv;}
  int  handle()  const { return c;}

  grid_type   const& TheGrid()   const { return *g;}
  anchor_type const& TheAnchor() const { return *g;}

  VertexOnCellIterator_istream_complex2d FirstVertex() const;
  VertexOnCellIterator_istream_complex2d EndVertex()   const;

  bool operator==(self const& rhs) const { return c == rhs.c;}
  bool operator!=(self const& rhs) const { return c != rhs.c;}
  bool operator< (self const& rhs) const { return c <  rhs.c;}

  bool bound() const { return g!=0;}
  bool valid() const { return bound() && !IsDone();}
  void cb()    const { REQUIRE(bound(), "", 1);}
  void cv()    const { REQUIRE(valid(), "", 1);}

};




/*! \brief VertexOnCell iterator for IstreamComplex2DFmt_base
    \ingroup complex2dformat

    Model of $GrAL Vertex-On-CellIterator

    \see Module \ref complex2dformat
    \see IstreamComplex2DFmt_base
 */
class VertexOnCellIterator_istream_complex2d {
  typedef  VertexOnCellIterator_istream_complex2d  self;
  friend   class  CellIterator_istream_complex2d;
public:
  typedef  IstreamComplex2DFmt_base           grid_type;
  typedef  VertexIterator_istream_complex2d   Vertex;
  typedef  CellIterator_istream_complex2d     Cell;

  typedef Vertex value_type;
  typedef Cell   anchor_type;
private:
  /*
  grid_type const* g;
  int nv;
  int  c;
  */
  Cell C;
  int lv; // local vertex  number
  int vc; // global vertex number of lv;
public:
  VertexOnCellIterator_istream_complex2d()  {}
  VertexOnCellIterator_istream_complex2d(Cell const& cc) : C(cc), lv(0)
  { read();}
private:
  VertexOnCellIterator_istream_complex2d(Cell const& cc, int LV) : C(cc), lv(LV) {}

public:

  self& operator++() {
    ++lv;
    read();
    return *this;
  }

  bool    IsDone()       const { return (lv >= C.NumOfVertices());}
  Vertex  operator*()    const { cv(); return Vertex(TheGrid(), handle());}
  int     handle()       const { cv(); return vc - TheGrid().offset;}

  grid_type   const& TheGrid()   const { cb(); return C.TheGrid();}
  anchor_type const& TheAnchor() const { cb(); return C;}

  bool operator==(self const& rhs) const { return lv == rhs.lv;}
  bool operator!=(self const& rhs) const { return lv != rhs.lv;}
  bool operator< (self const& rhs) const { return lv <  rhs.lv;}

  bool bound() const { return C.valid();}
  bool valid() const { return bound() && !IsDone();}
  void cb()    const { REQUIRE(bound(), "", 1);}
  void cv()    const { REQUIRE(valid(), "", 1);}
private:
  void read() { 
    if(!IsDone())
      TheGrid().In() >> vc;
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
{ return VertexIterator(*this);}

inline 
VertexIterator_istream_complex2d
IstreamComplex2DFmt_base::EndVertex() const
{ return VertexIterator(*this, NumOfVertices());}


inline 
CellIterator_istream_complex2d
IstreamComplex2DFmt_base::FirstCell()  const
{ return CellIterator(*this);}

inline 
CellIterator_istream_complex2d
IstreamComplex2DFmt_base::EndCell()  const
{ return CellIterator(*this,NumOfCells());}


inline 
IstreamComplex2DFmt_base::Vertex
IstreamComplex2DFmt_base::vertex(IstreamComplex2DFmt_base::vertex_handle v) const
{ return Vertex(*this, v);}


inline 
VertexOnCellIterator_istream_complex2d
CellIterator_istream_complex2d::FirstVertex() const 
{ return VertexOnCellIterator_istream_complex2d(*this);}

inline 
VertexOnCellIterator_istream_complex2d
CellIterator_istream_complex2d::EndVertex() const 
{ return VertexOnCellIterator_istream_complex2d(*this,nv);}



struct grid_types_IstreamComplex2DFmt_base : public grid_types_detail::grid_types_root
{
  typedef IstreamComplex2DFmt_base       grid_type;
  typedef GrAL::signed_size_t            size_type;
  typedef grid_dim_tag<2>                dimension_tag;

  typedef  grid_type::Vertex               Vertex;
  typedef  grid_type::VertexIterator       VertexIterator;
  typedef  grid_type::VertexOnCellIterator VertexOnCellIterator;
  typedef  grid_type::Cell                 Cell;
  typedef  grid_type::CellIterator         CellIterator;
  typedef  grid_type::vertex_handle        vertex_handle;
  typedef  grid_type::cell_handle          cell_handle;
};


/*! \brief Specialization of  \c grid_types  for IstreamComplex2DFmt_base
    \ingroup complex2dformat


    \see Module \ref complex2dformat
    \see Module \ref traits (\c grid_types)
    \see IstreamComplex2DFmt_base
 */
template<>
struct grid_types<IstreamComplex2DFmt_base> 
  : public grid_types_base<grid_types_IstreamComplex2DFmt_base>
{};


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
  grid_function(typename base::grid_type const& gg) : base(gg) {}
  grid_function(typename base::grid_type const& gg, T const& tt) : base(gg,tt) {}
};

template<class T>
class grid_function<CellIterator_istream_complex2d,T>
  : public grid_function_vector<CellIterator_istream_complex2d,T>
{
  typedef grid_function_vector<CellIterator_istream_complex2d,T> base;
public: 
  grid_function() {}
  grid_function(typename base::grid_type const& gg) : base(gg) {}
  grid_function(typename base::grid_type const& gg, T const& tt) : base(gg,tt) {}
};

} // namespace GrAL 


namespace STDEXT {
  template<class T> struct hash;

  template<>
  struct hash<GrAL::VertexIterator_istream_complex2d>
  {
    typedef GrAL::VertexIterator_istream_complex2d key_type;
    typedef key_type                               argument_type;
    typedef size_t                                 result_type;
    typedef GrAL::element_traits<key_type> et;
    typedef et::hasher_type hasher_type;

    result_type operator()(key_type v) const { hasher_type h; return h(v);}
  };

} // namespace STDEXT
#endif
