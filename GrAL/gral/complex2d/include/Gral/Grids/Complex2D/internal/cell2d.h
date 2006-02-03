#ifndef NMWR_GB_CELL2D_H
#define NMWR_GB_CELL2D_H


// $LICENSE

#include "Gral/Grids/Complex2D/complex2d.h"


class Cell2D : public complex2d_types {
  //----- DATA -----
public:
  cell_handle _pos; // public for grid_function access
private:
  ComplexPtr _cc;   // reference to grid

public: 
  typedef cell_type_tag               element_type_tag;
  typedef grid_element_category_d<2>  category;

  typedef CellNeighbourIterator    NeighbourIterator;
  typedef EdgeOnCell2D_Iterator    EdgeOnCellIterator;

  typedef VertexOnCellIterator     VertexIterator;
  typedef EdgeOnCellIterator       EdgeIterator;
  typedef FacetOnCellIterator      FacetIterator;
  typedef Cell2D self;

  //------------ construction ------------------------  
  Cell2D() : _pos(-1), _cc(0) {}
  explicit Cell2D(Complex2D const&  cc);
  Cell2D(Complex2D const&  cc, cell_handle pos)
    : _pos(pos), _cc(&cc) {}

  ~Cell2D() {}

  //-------------- iteration  -----------------

  //--- sequential access ------

  inline VertexOnCellIterator     FirstVertex() const;
  inline VertexOnCellIterator     EndVertex() const;
  inline EdgeOnCellIterator       FirstEdge() const;
  inline EdgeOnCellIterator       EndEdge() const;
  inline FacetOnCellIterator      FirstFacet() const;
  inline FacetOnCellIterator      EndFacet() const;
  inline NeighbourIterator        FirstNeighbour() const;
  inline NeighbourIterator        EndNeighbour() const;
  inline NeighbourIterator        FirstCell() const;
  inline NeighbourIterator        EndCell() const;

  //----- random access -------
  inline Vertex V(int i) const;
  inline Edge   E(int i) const;
  inline Edge   F(int i) const;
  inline vertex_handle v(int i) const;

  int NumOfVertices()   const {
    REQUIRE((is_valid()),"Cell2D: action with invalid cell!",1);
    return (base()._vertices.size());
  }
  int NumOfEdges()      const {return NumOfVertices();}
  int NumOfFacets()     const {return NumOfVertices();}
  int NumOfCells()      const {return NumOfNeighbours();} 
  int NumOfNeighbours() const; 

  inline void FlipEdge(const Vertex& v,  Edge& e) const;
  Edge FlippedEdge(Vertex const& v, Edge const& e) const;  

  Complex const& TheGrid() const {return (*_cc);}
  cell_handle    handle()  const { return _pos;}

  archetype_type const& TheArchetype() const;

  //---------------- comparision --------------------

  friend bool operator<(const self& s1, const self& s2)
  { return (s1._pos < s2._pos);}
  friend bool operator==(const self& s1, const self& s2)
  { return (s1._pos == s2._pos);}
  friend bool operator!=(const self& lhs, const self& rhs)
    { return !(lhs == rhs);}

  inline bool is_valid() const;
  inline bool valid() const { return is_valid();}
private:
  friend class VertexOnCell2D_Iterator;
  friend class EdgeOnCell2D_Iterator;
  friend class CellOnCell2D_Iterator;
  friend class Vertex2D;
  friend class Edge2D;
  friend class Complex2D;

  inline vertex_handle vertex(int lv) const;
  inline cell_handle   cell  (int lc) const;

  cell2d_connectivity const& base() const;
public:
  inline int find_local_facet(const Cell& Nb) const;
  inline vertex_handle vertex(unsigned side, const EdgeOnCellIterator& nb) const; // side in {1,2}

  friend ::std::ostream& operator <<(::std::ostream& out, self const& rs)
    { return (out << rs._pos << ' ' << rs._cc); }

};


#endif
