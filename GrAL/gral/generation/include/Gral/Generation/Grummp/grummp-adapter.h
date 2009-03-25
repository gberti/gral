#ifndef  GRAL_MG_GRUMMP_ADAPTER_H
#define  GRAL_MG_GRUMMP_ADAPTER_H

/* ------------------------------------------------------------

    Copyright (C) 2005 - 2009 Roman Putanowicz

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */

#include "Utility/pre-post-conditions.h"
#include "Gral/Base/common-grid-basics.h"
#include "Geometry/coords.h"
#include "Geometry/point-traits.h"
#include "Geometry/algebraic-primitives.h"
//#include "Gral/Iterators/facet-iterator-of-cell-set.h"
//#include "Gral/Iterators/vertex-on-edge-iterator.h"
#include "Gral/Base/element-handle.h"
#include "Gral/Base/polygon.h"
#include <vector>
#include <string>
#include <iostream>

#include "GR_Mesh2D.h"

namespace GrAL {

namespace grummp_adapter {

//namespace grummp {
//using namespace std;
//}

class Vertex;
class Cell;
class VertexOnCellIterator;
class Mesh2DAdapter;

struct grid_types_Mesh2DAdapter {
  typedef Mesh2DAdapter                       grid_type;
  //typedef Mesh2DAdapter::Edge               Edge;
  //typedef Edge                                    Facet;
  typedef GrAL::grummp_adapter::Vertex       Vertex;
  typedef GrAL::grummp_adapter::Cell         Cell;
  typedef Cell                                      CellIterator;
  typedef Vertex                                    VertexIterator;
  //typedef Mesh2DAdapter_FacetIterator       EdgeIterator;
  //typedef EdgeIterator                            FacetIterator;

  typedef vertex_handle_int<grid_type>              vertex_handle;
  typedef cell_handle_int  <grid_type>              cell_handle;
  //typedef edge_handle_Mesh2DAdapter         edge_handle;
  //typedef edge_handle                             facet_handle;
  typedef GrAL::grummp_adapter::VertexOnCellIterator
                                                    VertexOnCellIterator;

  typedef Cell                                      Face;
  typedef cell_handle                               face_handle;
  typedef CellIterator                              FaceIterator;
  typedef VertexOnCellIterator                      VertexOnFaceIterator;

  typedef grid_dim_tag<2>               dimension_tag;

  typedef polygon1d::polygon                 archetype_type;
  typedef std::vector<archetype_type>        archetype_sequence;
  typedef archetype_sequence::const_iterator archetype_iterator;
  typedef int                                archetype_handle;
  typedef grid_types<archetype_type>         archgt;
  typedef ::Mesh2D                     Adaptee;  

  typedef coordN<2> coord_type;
  typedef point_traits<coord_type> pt;
  typedef algebraic_primitives<coord_type> ap;
  typedef pt::component_type       scalar_type;
};

/*! \brief Interface to 'Grummp' mesh generator.

   \ingroup trianglemodule 

   Model of $GrAL VertexGridRange, $GrAL EdgeGridRange, $GrAL CellGridRange,
   $GrAL ArchetypedGrid.
*/
class Mesh2DAdapter : public grid_types_Mesh2DAdapter {
public:
  enum { dim = 2};

private:
  struct SD {
    typedef Mesh2DAdapter::archetype_type archetype_type;

    archetype_type the_archetype[1];
    SD();
  };
  static SD sd;
  Adaptee* _a;
  bool owned;

  void clear(); 

public:
  //@{
  /*! \name Construction 
   */
  //! \brief Empty grid
  inline Mesh2DAdapter();
  inline Mesh2DAdapter(Adaptee *a);
  inline ~Mesh2DAdapter();

  inline void init(Adaptee *a); 
  inline void steal(Adaptee *a);

  inline bool IsOwner() const;
  
  inline unsigned dimension() const;
  
  inline int NumOfVertices() const; 
  inline int NumOfCells() const;
  
  inline bool IsBound() const;
  inline const Adaptee* TheAdaptee() const; 

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

  //FIXME
  static  archetype_type const& ArchetypeOf (Cell const&) { 
    return *BeginArchetype();
  }

//FIXME
  static archetype_type const& ArchetypeOf (cell_handle) { 
    return *BeginArchetype();
  }

  static archetype_handle  archetype_of(cell_handle) { 
    return 0;
  }

  static archetype_handle archetype_of(Cell const&) { 
    return 0;
  }

  static archetype_iterator BeginArchetype() { 
    return archetype_iterator(sd.the_archetype);
  }

  static archetype_iterator EndArchetype() { 
    return BeginArchetype() +1;
  }

  static unsigned NumOfArchetypes() { 
    return 1;
  }

  static archetype_handle handle(archetype_iterator a) {
    return a - BeginArchetype();
  }

  // Vertex Geometry part
  inline unsigned space_dimension() const;
    
  inline grid_type const& TheGrid() const;

  inline coord_type  coord(Vertex const& v) const; 
}; 

inline std::ostream&
operator<<(std::ostream& out, Mesh2DAdapter::coord_type const& p);

//-------------------------------------------------------------------
// Decalartion of the local classes
//-------------------------------------------------------------------

//----------- Vertex / VertexIterator ----------------
class Vertex : public grid_types_Mesh2DAdapter {
public:
  typedef Vertex      value_type;

private: 
  grid_type const* g;
  vertex_handle    v;

public:
  inline Vertex();
  inline explicit Vertex(grid_type const& gg);
  inline Vertex(grid_type const& gg, int vv);
  inline ~Vertex();  

  inline bool IsDone()   const;
  inline Vertex const& operator*() const;
  inline Vertex & operator++(); 

  inline grid_type const& TheGrid() const;
  inline vertex_handle    handle()  const;

  // checking functions
  inline bool bound() const;
  inline bool valid() const; 
  inline void cb()    const;
  inline void cv()    const;

  inline friend bool operator==(Vertex const& lhs, Vertex const& rhs);
  inline friend bool operator!=(Vertex const& lhs, Vertex const& rhs);
};
 
//------------ Cell / CellIterator ----------------

class Cell : public grid_types_Mesh2DAdapter {
  friend  class GrAL::grummp_adapter::VertexOnCellIterator;

  public:
    typedef Cell                value_type;
 
  private: 
    grid_type const* g;
    cell_handle      c;
    ::Cell*    gcell;
  
  public:
    
  inline Cell();
  explicit inline Cell(grid_type const& gg);
  inline Cell(grid_type const& gg, cell_handle cc);
  inline ~Cell();  

  inline bool IsDone() const;
  inline Cell const& operator*() const;
  inline Cell & operator++();

  inline grid_type const& TheGrid() const;
  inline cell_handle   handle() const;

  inline VertexOnCellIterator FirstVertex() const;
  inline VertexOnCellIterator EndVertex  () const;

  inline unsigned NumOfVertices() const;
  inline unsigned NumOfEdges()    const;
  inline unsigned NumOfFacets()   const;

  inline vertex_handle v(int lv) const;
  inline Vertex        V(int lv) const;

  inline archetype_type const& TheArchetype() const; 

  // checking functions
  inline bool bound() const; 
  inline bool valid() const;
  inline void cb()    const;
  inline void cv()    const;

  inline friend bool operator==(Cell const& lhs, Cell const& rhs); 
  inline friend bool operator!=(Cell const& lhs, Cell const& rhs); 
};

//----------------- VertexOnCellIterator ---------------------

class VertexOnCellIterator : public grid_types_Mesh2DAdapter {
private:
  Cell c;
  int  vc;
  Adaptee*   a;
  ::Cell*    gcell;
public:

  inline VertexOnCellIterator();
  inline explicit VertexOnCellIterator(Cell const& cc, 
                                       int vvc = 0);

  inline VertexOnCellIterator& operator++();
  inline Vertex operator*() const;
  inline bool IsDone() const;

  inline grid_type const& TheGrid() const;
  inline Cell const& TheCell() const;

  inline vertex_handle handle() const;
  inline int local_handle()     const;

  // checking functions
  inline bool bound() const;
  inline bool valid() const;
  inline void cb()    const;
  inline void cv()    const;

  inline friend bool operator==(VertexOnCellIterator const& lhs, 
                                VertexOnCellIterator const& rhs); 
  inline friend bool operator!=(VertexOnCellIterator const& lhs, 
                                VertexOnCellIterator const& rhs); 
};

// inline functions definitions
#include "Gral/Generation/Grummp/grummp-adapter-inl.h"

} // namespace grummp_adapter

template<>
struct grid_types<grummp_adapter::Mesh2DAdapter> : public 
             grid_types_base<grummp_adapter::grid_types_Mesh2DAdapter>
{
  static vertex_handle handle(grummp_adapter::Vertex const& V) { 
    return V.handle();
  }
  static cell_handle   handle(grummp_adapter::Cell   const& C) { 
    return C.handle();
  }
  static int  local (grummp_adapter::VertexOnCellIterator const& v) { 
    return v.local_handle();
  }
  static cell_handle invalid_cell_handle(grummp_adapter::Mesh2DAdapter const&) { 
    return -1;
  }
};

} //namespace GrAL

#endif /* ifndef  GRAL_MG_GRUMMP_ADAPTER_H */
