#ifndef NMWR_GB_EDGE2D_H
#define NMWR_GB_EDGE2D_H



// $LICENSE


#include "Gral/Grids/Complex2D/complex2d.h"

class Edge2D : public complex2d_types {
public:

  typedef edge_type_tag               element_type_tag;
  typedef grid_element_category_d<1>  category;
  typedef edge_handle_complex2d   handle_type;
  typedef Complex2D               anchor_type;
  typedef VertexOnEdge2D_Iterator VertexIterator;

private:
  //----- DATA --------
  EdgeOnCell2D_Iterator _e;

  typedef Edge2D self;
public:
  //------------- constructors -------------------------
  Edge2D() : _e() {}
  explicit Edge2D(Complex2D const& cc);
  Edge2D(Complex2D const& cc, edge_handle eh);

  Edge2D(const Cell& C1, const Cell& C2)
    : _e(C1.find_local_facet(C2),C1) {}

  explicit Edge2D(const EdgeOnCell2D_Iterator& e) : _e(e) {}
  explicit Edge2D(const CellOnCell2D_Iterator& e) : _e(e) {}

  ~Edge2D() {}


  //-------------- iteration -----------------

  inline VertexOnEdge2D_Iterator FirstVertex() const;
  inline VertexOnEdge2D_Iterator EndVertex()   const;
  int NumOfVertices() const { return 2;}

  inline CellOnEdge2D_Iterator FirstCell() const;
  inline CellOnEdge2D_Iterator EndCell()   const;
  int NumOfCells() const { return 2;}


  //--------- edge-vertex operations ---------
  inline vertex_handle v1() const { return _e.TheCell().vertex(1,_e);}
  inline vertex_handle v2() const { return _e.TheCell().vertex(2,_e);}


  inline Vertex V1() const { return Vertex(TheGrid(),v1());}
  inline Vertex V2() const { return Vertex(TheGrid(),v2());}



  //--------- facet-cell operations ----------

  inline cell_handle c1() const { return _e.the_cell();}
  inline cell_handle c2() const { return _e.other_cell();}

  inline Cell   C1() const { return (_e.TheCell()  );}
  inline Cell   C2() const { return (_e.OtherCell());}

  //----------- switch operations -----------

  void FlipVertex(Vertex& v) const { v = ( v == V1() ? V2() : V1());}
  void FlipCell(Cell& c)     const { c = ( c == C1() ? C2() : C1());}
  Vertex FlippedVertex(Vertex const& v) const { return ( v == V1() ? V2() : V1());}
  Cell   FlippedCell  (Cell   const& c) const { return ( c == C1() ? C2() : C1());}

  //-------------- anchors -------------------
  
  Complex const& TheGrid()   const { return _e.TheGrid();}
  Complex const& TheAnchor() const { return _e.TheGrid();}

  handle_type    handle()  const { return TheGrid().handle(*this);}

  //---------------- comparision ------------------------

  friend bool operator< (const self& lhs, const self& rhs);
  friend bool operator==(const self& lhs, const self& rhs);
  friend bool operator!=(const self& lhs, const self& rhs)
    { return !(lhs == rhs);}

  friend class Cell2D;
  friend class Complex2D;
};


//---------------------------------------------------------



inline bool operator==(const Edge2D& lhs, const Edge2D& rhs)
{ // vertices anstelle von cells: sonst probleme am rand, speziell an ecken!
  // 2 randkanten derselben zelle wuerden als gleich erkannt.
  return (  (lhs.V1() == rhs.V1() && lhs.V2() == rhs.V2())
	  ||(lhs.V1() == rhs.V2() && lhs.V2() == rhs.V1()));
}

inline bool operator<(const Edge2D& s1, const Edge2D& s2)
{
  // etwas effizienter mit ordered_pair(C(1),C(2))
  // evtl. gleich cnbit gleich so waehlen, dass reihenfolge stimmt?
  typedef Edge2D::vertex_handle vh;
  vh max1(::std::max(s1.v1(),s1.v2()));
  vh min1(::std::min(s1.v1(),s1.v2()));
  vh max2(::std::max(s2.v1(),s2.v2()));
  vh min2(::std::min(s2.v1(),s2.v2()));
  
  return ( (max1 < max2)  || ( (max1 == max2) && (min1 < min2)));
}



#endif
