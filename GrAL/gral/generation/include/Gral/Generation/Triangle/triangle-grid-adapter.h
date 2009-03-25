#ifndef  GRAL_MG_TRIANGLE_GRID_ADAPTER_H
#define  GRAL_MG_TRIANGLE_GRID_ADAPTER_H

/* ------------------------------------------------------------

    Copyright (C) 2005 - 2009 Roman Putanowicz

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */

#include "Utility/pre-post-conditions.h"
#include "Gral/Base/common-grid-basics.h"
#include "Gral/Generation/Triangle/triangle-generator.h"

//#include "Gral/Iterators/facet-iterator-of-cell-set.h"
//#include "Gral/Iterators/vertex-on-edge-iterator.h"
#include "Gral/Base/element-handle.h"
#include "Gral/Base/polygon.h"
#include <algorithm>

#define REAL double
extern "C" {
#include "triangle.h"
}

namespace GrAL {

/*! \brief Interface to 'triangle' mesh generator.

   \ingroup trianglemodule 

   Model of $GrAL VertexGridRange, $GrAL EdgeGridRange, $GrAL CellGridRange,
   $GrAL ArchetypedGrid.
*/
class TriangleGridAdapter;

class TriangleGridAdapter {
public:
  typedef TriangleGenerator Generator;
  typedef vertex_handle_int<TriangleGridAdapter>  vertex_handle;
  typedef cell_handle_int<TriangleGridAdapter> cell_handle;
  typedef TriangleGridAdapter grid_type;
  typedef polygon1d::polygon                 archetype_type;
  typedef std::vector<archetype_type>        archetype_sequence;
  typedef archetype_sequence::const_iterator archetype_iterator;
  typedef int                                archetype_handle;
  typedef grid_types<archetype_type>         archgt;
  typedef struct ::triangulateio             Adaptee;  
     
  enum { dim = 2};
  unsigned dimension() const { return dim;}

private:
  struct SD {
    typedef TriangleGridAdapter::archetype_type archetype_type;

    archetype_type the_archetype[1];
    SD();
  };
  static SD sd;
  Adaptee* _a;

public:
  //@{
  /*! \name Construction 
   */
  //! \brief Empty grid
  TriangleGridAdapter()  : _a(0) {}
  TriangleGridAdapter(Adaptee *a) : _a(a) {}
  ~TriangleGridAdapter() {};

  double const* getCoordsList() const { 
    REQUIRE(_a != NULL, "not bound", 1);
    return  _a->pointlist;
  }
  int const* getCellsList() const {
    REQUIRE(_a != NULL, "not bound", 1); 
    return _a->trianglelist;
  }

  int NumOfVertices() const  { 
    REQUIRE(_a != NULL, "not bound", 1); 
    return _a->numberofpoints;
  }

  int NumOfCells() const {
    REQUIRE(_a != NULL, "not bound", 1);
    return _a->numberoftriangles;
  }  

  inline bool is_bound() const {return _a != NULL;}

  const Adaptee* TheAdaptee() const {return _a;}
  //-------------------------------------------------------------------
  // Definition of the local classes
  //-------------------------------------------------------------------
  class Vertex;
  class Cell;
  class VertexOnCellIterator;

  //----------- Vertex / VertexIterator ----------------
  class Vertex {
    typedef Vertex    self;
  public:
    typedef self      value_type;
    typedef TriangleGridAdapter grid_type; 
  private: 
    grid_type const* g;
    vertex_handle    v;

  public:
    Vertex() : g(0), v(-1) {}
    explicit Vertex(grid_type const& gg) : g(&gg), v(0) {}
    Vertex(grid_type const& gg, int vv) : g(&gg), v(vv) {}
    ~Vertex() {}  
  
    bool IsDone()   const { cb(); return (v == g->NumOfVertices()); }
    self const& operator*() const { cv(); return *this;}
    self & operator++() { cv(); ++v; return *this;}
  
    grid_type const& TheGrid() const { cb(); return *g;}
    vertex_handle    handle()  const { cv(); return v;}

    // checking functions
    bool bound() const { return g != 0;}
    bool valid() const 
      { return (bound() &&  (0 <= v) && (v < TheGrid().NumOfVertices()));}
    void cb()    const { REQUIRE (bound(), "",1);}
    void cv()    const { REQUIRE (valid(), "",1);}

    friend bool operator==(self const& lhs, self const& rhs) {
      return lhs.v == rhs.v;
    }
    friend bool operator!=(self const& lhs, self const& rhs) {
      return lhs.v != rhs.v;
    }
  };
   
  //------------ Cell / CellIterator ----------------

  class Cell {
    typedef Cell     self;
    friend class     VertexOnCellIterator;
    public:
      typedef TriangleGridAdapter grid_type; 
    private: 
      grid_type const* g;
      cell_handle      c;
      int const* cellslist;
      void init_cellslist() {
          cellslist = g->getCellsList();
      }
    public:
    Cell() : g(0), c(-1) {}
    explicit
    Cell(grid_type const& gg) : g(&gg), c(0) {
      init_cellslist(); 
    }
    Cell(grid_type const& gg,
		cell_handle      cc) : g(&gg), c(cc) {
      init_cellslist();
    }
    ~Cell() {}  
  
    bool IsDone()   const { return (c == g->NumOfCells()); }
    self const& operator*() const { return *this;}
    self & operator++() { ++c; return *this;}
  
    grid_type const& TheGrid() const { return *g;}
    cell_handle   handle() const { return c;}

    VertexOnCellIterator FirstVertex() const;
    VertexOnCellIterator EndVertex  () const;

    unsigned NumOfVertices() const { return 3;}
    unsigned NumOfEdges()    const { return 3;}
    unsigned NumOfFacets()   const { return 3;}

    vertex_handle v(int lv) const;
    Vertex        V(int lv) const;

    archetype_type const& TheArchetype() const { 
     return TheGrid().ArchetypeOf(*this);
    }

    // checking functions
    bool bound() const { return g != 0;}
    bool valid() const { return (bound() &&  (0 <= c) && (c < g->NumOfCells()));}
    void cb()    const { REQUIRE (bound(), "",1);}
    void cv()    const { REQUIRE (valid(), "",1);}

    friend bool operator==(self const& lhs, self const& rhs) { 
      return lhs.c == rhs.c;
    }
    friend bool operator!=(self const& lhs, self const& rhs) { 
      return !(lhs == rhs);
    }
  };

  //----------------- VertexOnCellIterator ---------------------

  class VertexOnCellIterator {
    typedef  VertexOnCellIterator self;
  public:
    typedef TriangleGridAdapter grid_type; 
  private:
    Cell c;
    int  vc;
  public:

    VertexOnCellIterator() : vc(-1) {}
    explicit
    VertexOnCellIterator(Cell const& cc, int vvc = 0) : c(cc), vc(vvc) {}
  
    self& operator++() { cv();  ++vc; return *this;}
    Vertex operator*() const 
     { cv(); return Vertex(TheGrid(), handle());}
    bool IsDone()   const { cb(); return (vc == 3);}
  
    grid_type const& TheGrid() const { cb(); return c.TheGrid();}
    Cell const& TheCell() const { cb(); return c;}


    vertex_handle handle() const 
      { cv(); return c.cellslist[3*c.handle()+vc];}
    int local_handle()     const { cv(); return vc;}

    // checking functions
    bool bound() const { return c.valid();}
    bool valid() const { return (c.valid() &&  (0 <= vc) && (vc < 3));}
    void cb()    const { REQUIRE (bound(), "",1);}
    void cv()    const { REQUIRE (valid(), "",1);}

    friend bool operator==(self const& lhs, self const& rhs) { 
      return lhs.vc == rhs.vc;
    }
    friend bool operator!=(self const& lhs, self const& rhs) { 
      return lhs.vc != rhs.vc;
    }
  };

  typedef Vertex VertexIterator; 
  typedef Cell CellIterator; 
  //@{
  /*! \name Sequence iteration
      \todo STL-style EndXXX() 
  */

  inline VertexIterator FirstVertex() const;
  inline CellIterator   FirstCell()   const;
  //@}

  bool valid(vertex_handle v) const;
  bool valid(cell_handle   c) const;
  //@}

  /*@{*/
   /*! \name Archetype handling
     
       \see $GrAL ArchetypedGrid concept
   */

  static archetype_type const& Archetype(archetype_handle a) {
    REQUIRE(a == 0, "a = " << a,1);
    return *BeginArchetype();
  }
  static archetype_type   const& ArchetypeOf (Cell const&) { 
    return *BeginArchetype();
  }
  static archetype_type   const& ArchetypeOf (cell_handle) { 
    return *BeginArchetype();
  }
  static archetype_handle  archetype_of(cell_handle) { return 0;}
  static archetype_handle  archetype_of(Cell const&) { return 0;}

  static archetype_iterator BeginArchetype()
    { return archetype_iterator(sd.the_archetype);}
  static archetype_iterator EndArchetype()  { return BeginArchetype() +1;}
  static unsigned NumOfArchetypes() { return 1;}
  static archetype_handle handle(archetype_iterator a)
    { return a - BeginArchetype();}

  /*@}*/
}; 

struct grid_types_TriangleGridAdapter {
  typedef TriangleGridAdapter       grid_type;
  typedef TriangleGridAdapter::Vertex Vertex;
  //typedef TriangleGridAdapter::Edge  Edge;
  //typedef Edge            Facet;
  typedef TriangleGridAdapter::Cell Cell;
  typedef Cell            CellIterator;
  typedef Vertex          VertexIterator;
  //typedef TriangleGridAdapter_FacetIterator EdgeIterator;
  //typedef EdgeIterator    FacetIterator;

  typedef TriangleGridAdapter::VertexOnCellIterator VertexOnCellIterator;

  typedef vertex_handle_int<grid_type>  vertex_handle;
  typedef cell_handle_int  <grid_type>  cell_handle;
  //typedef edge_handle_TriangleGridAdapter            edge_handle;
  //typedef edge_handle                   facet_handle;

  typedef Cell                 Face;
  typedef cell_handle          face_handle;
  typedef CellIterator         FaceIterator;
  typedef VertexOnCellIterator VertexOnFaceIterator;

  typedef grid_dim_tag<2>               dimension_tag;

  typedef polygon1d::polygon                 archetype_type;
  typedef std::vector<archetype_type>        archetype_sequence;
  typedef archetype_sequence::const_iterator archetype_iterator;
  typedef int                                archetype_handle;
  typedef grid_types<archetype_type>         archgt;
};


template<>
struct grid_types<TriangleGridAdapter> : public grid_types_base<grid_types_TriangleGridAdapter>
{
  static vertex_handle handle(Vertex const& V) { return V.handle();}
  static cell_handle   handle(Cell   const& C) { return C.handle();}
  static int  local (VertexOnCellIterator const& v) { return v.local_handle();}
  static cell_handle invalid_cell_handle(TriangleGridAdapter const&) { return -1;}
};

inline
TriangleGridAdapter::CellIterator
TriangleGridAdapter::FirstCell() const { return CellIterator(*this);}


inline
TriangleGridAdapter::VertexOnCellIterator
TriangleGridAdapter::Cell::FirstVertex() const {
  return TriangleGridAdapter::VertexOnCellIterator(*this);
}

inline
TriangleGridAdapter::VertexOnCellIterator
TriangleGridAdapter::Cell::EndVertex() const {
  return TriangleGridAdapter::VertexOnCellIterator(*this, NumOfVertices());
}


inline
TriangleGridAdapter::VertexIterator
TriangleGridAdapter::FirstVertex() const { return VertexIterator(*this);}

inline 
TriangleGridAdapter::vertex_handle 
TriangleGridAdapter::Cell::v(int lv) const {
  return TriangleGridAdapter::vertex_handle(cellslist[3*c+lv]);
}

inline 
TriangleGridAdapter::Vertex
TriangleGridAdapter::Cell::V(int lv) const {
  return TriangleGridAdapter::Vertex(TheGrid(), v(lv));
}

} // namespace GrAL 

#endif /* ifndef  GRAL_MG_TRIANGLE_GRID_ADAPTER_H */
