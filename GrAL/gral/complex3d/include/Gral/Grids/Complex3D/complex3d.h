#ifndef GRAL_GB_COMPLEX3D_H
#define GRAL_GB_COMPLEX3D_H

// $LICENSE_NEC

#include <vector>

#include "Gral/Base/common-grid-basics.h"
#include "Gral/Base/predicates.h"
#include "Gral/Grids/Complex2D/complex2d.h"


#include "Gral/Iterators/generic-edge-iterators.h"
#include "Gral/Iterators/generic-facet-iterators.h"
#include "Gral/Iterators/vertex-on-edge-iterator.h"

namespace GrAL {

class Complex3D;

class Vertex_Complex3D;
class Edge_Complex3D;
class Facet_Complex3D;
class Cell_Complex3D;
class Cell_Complex3D_base;

class VertexOnCellIterator_Complex3D;


/*! Base type collecting the types related to Complex3D
 */
struct grid_types_Complex3D_base {
  public:
  typedef Complex3D grid_type;

  typedef int vertex_handle;
  typedef int cell_handle;
  typedef int archetype_handle;

  typedef Complex2D   archetype_type;
  typedef grid_types<archetype_type> archgt;

  typedef Vertex_Complex3D Vertex;
  typedef Cell_Complex3D   Cell;

  typedef Vertex           VertexIterator;
  typedef Cell             CellIterator;

  typedef VertexOnCellIterator_Complex3D VertexOnCellIterator;

  typedef Cell_Complex3D_base cell_base_type;

  typedef grid_dim_tag<3> dimension_tag;

  static cell_handle invalid_cell_handle(grid_type const&) { return cell_handle(-1);}
};


struct grid_types_Complex3D 
  : // public grid_types_Complex3D_no_edge,
  //  public generic_edge ::grid_types_edge<grid_types_Complex3D_no_edge>,
  public virtual 
  generic_facet::grid_types_facet
   < generic_edge::grid_types_edge<
         grid_types_Complex3D_base> 
   >
{
  typedef facet_handle          face_handle;
  typedef Facet                 Face;
  typedef FacetIterator         FaceIterator;
  typedef FacetOnCellIterator   FaceOnCellIterator;
  typedef VertexOnFacetIterator VertexOnFaceIterator;
  typedef EdgeOnFacetIterator   EdgeOnFaceIterator;

  typedef vertex_on_edge_iterator<Complex3D, grid_types_Complex3D> VertexOnEdgeIterator;
};

/*! \brief Class for representing general 3D unstructured grids

    \ingroup complex3dmodule
*/
class Complex3D : public grid_types_Complex3D {
  typedef size_t size_type;

 private:
  std::vector<vertex_handle>  cells; // cell-vertex incidences
  std::vector<unsigned>       offset; // pointer into cells, nc+1 entries
  // (last entry  points to end of cells)

  std::vector<unsigned>       cell_archetype;
  std::vector<archetype_type> archetypes;
  size_type                   num_of_vertices;

 public:
  //!
  Complex3D();
  //!
  ~Complex3D();
  //! make this an empty grid
  void clear(); 
  //@{ @name Dimension information
  enum { dim = 3};
  unsigned dimension() const { return dim;}  
  //@}

  //@{ @name Sequence iteration
  inline VertexIterator FirstVertex() const;
  inline VertexIterator EndVertex()   const;

  inline EdgeIterator   FirstEdge()   const;
  inline FacetIterator  FirstFace()   const;
  inline FacetIterator  FirstFacet()  const;

  inline CellIterator   FirstCell()   const;
  inline CellIterator   EndCell()     const;

  size_type NumOfVertices() const { return num_of_vertices;}
  size_type NumOfCells()    const { return cell_archetype.size();}
  //@}

  bool valid_vertex(vertex_handle v) const { return 0 <= v && v < (int)NumOfVertices();}
  bool valid_cell  (cell_handle   c) const { return 0 <= c && c < (int)NumOfCells();}

  inline Vertex switched_vertex(Vertex v, Edge  e) const;
  inline Edge   switched_edge  (Vertex v, Edge  e, Face f) const;
  inline Facet  switched_facet (Edge   e, Facet f, Cell c) const;

 
  void switch_vertex (Vertex& v, Edge   e) const;
  void switch_edge   (Vertex v,  Edge&  e, Face f) const;
  void switch_facet  (Edge   e,  Facet& f, Cell c) const;
  
private:

  friend class Vertex_Complex3D;
  friend class Cell_Complex3D;
  friend class VertexOnCellIterator_Complex3D;

  template<class G_SRC, class VCORR, class CCORR>
  friend
  void ConstructGrid0(Complex3D     & G_dest,
		      G_SRC    const& G_src,
		      VCORR         & v_corr,
		      CCORR         & c_corr);


  // set dependent information
  void calculate_dependent();

  // modifying functions (used in ConstructGrid())
  vertex_handle add_vertex() { return (num_of_vertices++);}
  cell_handle   add_cell(archetype_handle a)
    { 
      cell_handle ret = NumOfCells();
      unsigned nv = Archetype(a).NumOfVertices();
      cells.insert(cells.end(), nv,-1);
      offset.push_back(offset.back() + nv);
      cell_archetype.push_back(a);
      return ret;
    }
    


  struct cell_vertex_incidence_map {
  private:
    ::std::vector<vertex_handle>::iterator pc;
  public:
    cell_vertex_incidence_map(Complex3D & G, cell_handle c)
      : pc(G.cells.begin() + G.offset[c]) {}
    vertex_handle & operator[](int lv) { return *(pc + lv);}
    vertex_handle   operator()(int lv) { return *(pc + lv);}
  };
  friend struct Complex3D::cell_vertex_incidence_map;
  

  // Archetype stuff
  archetype_handle add_archetype() { 
    archetypes.push_back(archetype_type());
    return (archetypes.size() -1);
  }
  archetype_type      & Archetype(archetype_handle a)       {
    REQUIRE(valid_archetype(a), "a = " << a,1); 
    return archetypes[a];
  }

  bool valid_archetype(archetype_handle a) const
    { return ((0 <= a) && (a < (int)archetypes.size())); }
  //     && (archetypes[a].NumOfCells() > 0));}

public:
  //@{ @name Archetype stuff
  inline
  archetype_type const& ArchetypeOf(Cell const&  c) const;
  archetype_type const& ArchetypeOf(cell_handle  c) const
    { return archetypes[cell_archetype[c]];}
  archetype_handle      archetype_of(cell_handle c) const
    { return cell_archetype[c];}
  archetype_handle      archetype_of(Cell const& c) const;

  typedef ::std::vector<archetype_type>::const_iterator archetype_iterator;
  archetype_iterator  BeginArchetype() const { return archetypes.begin();}
  archetype_iterator  EndArchetype()   const { return archetypes.end  ();}
  archetype_handle    handle(archetype_iterator it) const { return it - BeginArchetype();}
  archetype_type const& Archetype(archetype_handle a) const { 
    REQUIRE(valid_archetype(a), "a = " << a,1);
    return archetypes[a];
  }

  //@}

}; // class Complex3D




class elem_base_Complex3D : public virtual grid_types_Complex3D {
 private:
  grid_type const* g;
 public:
  elem_base_Complex3D() : g(0) {}
  elem_base_Complex3D(grid_type const& gg) : g(&gg) {}
protected:
  void init(grid_type const& gg)  { g = &gg;}

public: 
  grid_type const& TheGrid() const { return *g;}

  bool bound() const { return (g != 0);}
};



class Cell_Complex3D_base : public elem_base_Complex3D
{
  typedef Cell_Complex3D      self;
  typedef elem_base_Complex3D base;

 protected:
  cell_handle h;
 public:
  Cell_Complex3D_base() {}
  explicit
  Cell_Complex3D_base(grid_type const& gg) : base(gg), h(0) {}
  Cell_Complex3D_base(grid_type const& gg, cell_handle hh) : base(gg), h(hh) {}
protected:
  void init(grid_type const& gg, cell_handle hh)
    { base::init(gg); h = hh;} 

public:
  cell_handle handle() const { c_(); return h;}

  bool valid() const { return (bound() &&  0 <= h && h < (int)TheGrid().NumOfCells());}
  void c_() const { REQUIRE(valid(), " handle = " << h << '\n',1);}
  void cb_() const { REQUIRE(bound(), "",1);}

  archetype_type const& TheArchetype() const { c_(); return TheGrid().ArchetypeOf(h);}


};


/*!\internal
   \brief Cell type for Complex3D

    Model of $GrAL Cell, $GrAL CellIterator
 */
class Cell_Complex3D
  :  public grid_types_Complex3D::cell_base_type
{
  typedef Cell_Complex3D      self;

public:
  // these typedefs are superflous (already defined in base classes,
  // but g++ forgets them, because doubly defined!
  typedef grid_types_Complex3D gt;
  typedef gt::grid_type grid_type;
  typedef gt::cell_handle cell_handle;
  typedef gt::vertex_handle vertex_handle;
  typedef gt::VertexOnCellIterator VertexOnCellIterator;
 public:
  Cell_Complex3D() {}
  explicit
  Cell_Complex3D(grid_type const& gg, cell_handle hh = 0) { init(gg,hh);}


  self & operator++() { c_(); ++h; return *this;}
  self const& operator*() const { c_(); return *this;}
  bool IsDone() const { cb_(); return (h >= (cell_handle)TheGrid().NumOfCells());}


  friend bool operator==(self const& lhs, self const& rhs) { 
    lhs.cb_(); rhs.cb_(); 
    return (lhs.h == rhs.h);
  }
  friend bool operator!=(self const& lhs, self const& rhs) { 
    return !(lhs == rhs);
  }
  friend bool operator< (self const& lhs, self const& rhs) { 
    lhs.cb_(); rhs.cb_(); 
    return (lhs.h <  rhs.h);
  }

  inline FaceOnCellIterator FirstFace() const { return FirstFacet();}
  inline FaceOnCellIterator EndFace()   const { return EndFacet();}
  inline int NumOfFaces() const { return NumOfFacets();}

  inline VertexOnCellIterator FirstVertex() const;
  inline VertexOnCellIterator EndVertex() const;
  unsigned NumOfVertices() const { 
   c_();  return TheGrid().offset[h+1] - TheGrid().offset[h];
  }
  vertex_handle v(archgt::vertex_handle lv) const 
    { c_(); return TheGrid().cells[TheGrid().offset[h]+lv];}
  inline Vertex V(archgt::Vertex v) const;

  inline edge_handle e(archgt::edge_handle le) const;
};




/*! \internal 
    \brief Vertex type for Complex3D

    Model of $GrAL GridVertex, $GrAL GridVertexIterator
 */
class Vertex_Complex3D : public elem_base_Complex3D {
  typedef Vertex_Complex3D      self;
  typedef elem_base_Complex3D base;
 private:
  vertex_handle h;
 public:
  Vertex_Complex3D() {}
  explicit
  Vertex_Complex3D(grid_type const& gg) : base(gg), h(0) {}
  Vertex_Complex3D(grid_type const& gg, vertex_handle hh) : base(gg), h(hh) {}

  self & operator++() { c_(); ++h; return *this;}
  self const& operator*() const { c_(); return *this;}
  bool IsDone() const { cb_(); return (h >= (vertex_handle)TheGrid().NumOfVertices());}
  vertex_handle handle() const { c_(); return h;}



  friend bool operator==(self const& lhs, self const& rhs)
  { lhs.cb_(); rhs.cb_(); return (lhs.h == rhs.h);}
  friend bool operator!=(self const& lhs, self const& rhs)
  { return !(lhs == rhs);}
  friend bool operator< (self const& lhs, self const& rhs)
  { lhs.cb_(); rhs.cb_(); return (lhs.h <  rhs.h);}

  bool valid() const { return bound() && 0 <= h && h < vertex_handle(TheGrid().NumOfVertices());}
  void c_() const { REQUIRE(valid(), " handle = " << h << '\n',1);}
  void cb_() const { REQUIRE(bound(), "",1);}
};




/*!\internal
    \brief VertexOnCellIterator type for Complex3D

    Model of $GrAL VertexOnCellIterator
 */
class VertexOnCellIterator_Complex3D : public elem_base_Complex3D {
  typedef VertexOnCellIterator_Complex3D  self;
  typedef elem_base_Complex3D             base;
 private:
  cell_handle c;
  int         lv;
 public:
  VertexOnCellIterator_Complex3D() {}
  explicit
  VertexOnCellIterator_Complex3D(Cell const& cc, int llv = 0) 
    : base(cc.TheGrid()), c(cc.handle()),  lv(llv) {}


  self & operator++() { c_(); ++lv; return *this;}
  Vertex operator*() const { 
    c_(); return Vertex(TheGrid(), handle());
  }
  bool IsDone() const { cb_(); return (lv >= (int)Cell_().NumOfVertices());}
  vertex_handle handle() const { c_(); return TheGrid().cells[TheGrid().offset[c]+lv];}
  Cell TheCell()   const { c_(); return Cell_();}
  Cell TheAnchor() const { c_(); return Cell_();}

  archgt::vertex_handle local_handle() const { return archgt::vertex_handle(lv);}

  friend bool operator==(self const& lhs, self const& rhs) { 
    lhs.cb_(); rhs.cb_(); 
    return (lhs.c == rhs.c) && (lhs.lv == rhs.lv);
  }
  friend bool operator!=(self const& lhs, self const& rhs) { 
    return !(lhs == rhs);
  }
  friend bool operator< (self const& lhs, self const& rhs) { 
    lhs.cb_(); rhs.cb_(); 
    return (lhs.c < rhs.c) || ((lhs.c == rhs.c) && (lhs.lv <  rhs.lv));
  }

  bool valid() const { 
    return bound() 
      && (0 <= c  && c  < (int)TheGrid().NumOfCells())
      && (0 <= lv && lv < (int)Cell_().NumOfVertices());
  }
  void c_()  const { REQUIRE(valid(), " c = " << c << " lv = " << lv << '\n',1);}
  void cb_() const { REQUIRE(bound(), "",1);}

private:
  // avoid recursion in check
  Cell Cell_() const { return Cell(TheGrid(),c);}
};


// inline functions

inline
Vertex_Complex3D Complex3D::FirstVertex() const
{ return Vertex(*this);}

inline
Vertex_Complex3D Complex3D::EndVertex() const
{ return Vertex(*this,NumOfVertices());}

inline
Complex3D::EdgeIterator
Complex3D::FirstEdge() const
{ return EdgeIterator(*this);}

inline
Complex3D::FacetIterator
Complex3D::FirstFacet() const
{ return FacetIterator(*this);}

inline
Complex3D::FacetIterator
Complex3D::FirstFace() const
{ return FacetIterator(*this);}

inline
Cell_Complex3D Complex3D::FirstCell() const
{ return Cell(*this);}

inline
Cell_Complex3D Complex3D::EndCell() const
{ return Cell(*this, NumOfCells());}

inline
Complex3D::archetype_type const& 
Complex3D::ArchetypeOf(Complex3D::Cell const&  c) const 
{ return ArchetypeOf(c.handle());}

inline
Complex3D::archetype_handle
Complex3D::archetype_of(Complex3D::Cell const& c) const
{ return archetype_of(c.handle());}





inline  Complex3D::Vertex Complex3D::switched_vertex(Complex3D::Vertex v, 
						     Complex3D::Edge   e) const 
{ 
  VertexOnEdgeIterator ve(e);
  return ( v == *ve ? *(++ve) : *ve);
}
  
inline Complex3D::Edge Complex3D::switched_edge(Complex3D::Vertex v, 
						Complex3D::Edge   e, 
						Complex3D::Face   f) const 
{
  for(EdgeOnFaceIterator ef(f); !ef.IsDone(); ++ef)
    if(*ef != e && incident(v,*ef))
      return *ef;
  ENSURE(false, "could not find switched edge for (v,e,f) = " 
	 << v.handle()  << "," << e.handle() << "," << f.handle() << ")",1);
  return e; // keep compilers happy
}
 

inline  Complex3D::Facet Complex3D::switched_facet(Complex3D::Edge  e, 
						   Complex3D::Facet f, 
						   Complex3D::Cell  c) const 
{
  for(FacetOnCellIterator fc(c); !fc.IsDone(); ++fc)
    if(*fc != f && incident(e, *fc))
      return *fc;
  ENSURE(false, "could not find switched facet for (e,f,c) = " 
	 << e.handle()  << "," << f.handle() << "," << c.handle() << ")",1);
  return f; // keep compilers happy
}

  
  inline   void Complex3D::switch_vertex (Complex3D::Vertex& v, Complex3D::Edge   e) const 
  { v = switched_vertex(v,e);}
  inline void Complex3D::switch_edge   (Complex3D::Vertex v,  Complex3D::Edge&  e, Complex3D::Face f) const 
  { e = switched_edge (v,e,f);}
  inline void Complex3D::switch_facet  (Complex3D::Edge   e,  Complex3D::Facet& f, Complex3D::Cell c) const 
  { f = switched_facet(e,f,c);}






inline
VertexOnCellIterator_Complex3D
Cell_Complex3D::FirstVertex() const
{ c_(); return VertexOnCellIterator(*this);}

inline
VertexOnCellIterator_Complex3D
Cell_Complex3D::EndVertex() const
{ c_(); return VertexOnCellIterator(*this, NumOfVertices());}

inline
Vertex_Complex3D
Cell_Complex3D::V(Cell_Complex3D::archgt::Vertex vv) const
{ return Vertex(TheGrid(), v(vv.handle())); }

inline 
Cell_Complex3D::edge_handle
Cell_Complex3D::e(Cell_Complex3D::archgt::edge_handle le) const { return (EdgeOnCellIterator(*this, le).handle());}

// grid_types

/*! \brief Specialization of the grid_types primary template
    \ingroup complex3dmodule
 */
template<>
struct grid_types<Complex3D> 
  : public grid_types_base<grid_types_Complex3D>
{
  typedef grid_type::archetype_iterator archetype_iterator;

};


template<>
struct element_traits<Vertex_Complex3D>
  : public element_traits_vertex_base<Complex3D>
{ 
  struct  hasher_type : public hasher_type_elem_base {};
  typedef consecutive_integer_tag<0>         consecutive_tag;
};

template<>
struct element_traits<Complex3D::Edge>
  : public element_traits_edge_base<Complex3D>
{ 
  typedef Complex3D::Edge::edge_hasher_type edge_hasher_type;
  struct hasher_type :  public hasher_type_elem_base,
			public edge_hasher_type 
  {
    typedef Complex3D::Edge key_type;
    typedef Complex3D::Edge argument_type;
    // shouldn't this work?? 
    // using Complex3D::Edge::edge_hasher_type::operator();
    size_t operator()(key_type const& k) const { 
      return Complex3D::Edge::edge_hasher_type::operator()(k);
    }
  }; 
};


template<>
struct element_traits<Complex3D::Facet>
  : public element_traits_facet_base<Complex3D>
{ 
  typedef Complex3D::Facet::facet_hasher_type facet_hasher_type; 
  struct hasher_type : public hasher_type_elem_base,
		       public facet_hasher_type
  {
    typedef Complex3D::Facet key_type;
    typedef Complex3D::Facet argument_type;

    //??  using Complex3D::Facet::facet_hasher_type::operator();
    size_t operator()(key_type const& k) const { 
      return Complex3D::Facet::facet_hasher_type::operator()(k);
    }
  }; 
};

template<>
struct element_traits<Cell_Complex3D>
  : public element_traits_cell_base<Complex3D>
{ 
  struct hasher_type : public hasher_type_elem_base {};
  typedef consecutive_integer_tag<0> consecutive_tag;
};

} // namespace GrAL 

#endif


