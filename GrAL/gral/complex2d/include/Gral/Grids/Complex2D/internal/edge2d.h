#ifndef NMWR_GB_EDGE2D_H
#define NMWR_GB_EDGE2D_H



// $LICENSE


#include "Gral/Grids/Complex2D/complex2d.h"

class Edge2D : public complex2d_types {
public:

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
  Edge2D(const Cell& C1, const Cell& C2)
    : _e(C1.find_local_facet(C2),C1) {}

  Edge2D(const EdgeOnCell2D_Iterator& e) : _e(e) {}
  Edge2D(const CellOnCell2D_Iterator& e) : _e(e) {}

  ~Edge2D() {}


  //-------------- iteration -----------------

  inline VertexOnEdge2D_Iterator FirstVertex() const;
  inline VertexOnEdge2D_Iterator EndVertex()   const;
  int NumOfVertices() const { return 2;}

  inline CellOnEdge2D_Iterator FirstCell() const;
  inline CellOnEdge2D_Iterator EndCell()   const;
  int NumOfCells() const { return 2;}


  //--------- edge-vertex operations ---------
  inline Vertex V1() const;
  inline Vertex V2() const;

  //--------- facet-cell operations ----------
  inline Cell   C1() const;
  inline Cell   C2() const;
  

  void FlipVertex(Vertex& v) const { v = ( v == V1() ? V2() : V1());}
  void FlipCell(Cell& c)     const { c = ( c == C1() ? C2() : C1());}

  //-------------- anchors -------------------
  
  Complex const& TheGrid()   const { return _e.TheGrid();}
  Complex const& TheAnchor() const { return _e.TheGrid();}

  handle_type    handle()  const { return TheGrid().handle(*this);}

  //---------------- comparision ------------------------

  friend bool operator< (const self& lhs, const self& rhs);
  friend bool operator==(const self& lhs, const self& rhs);

  friend class Cell2D;
  friend class Complex2D;
};


//---------------------------------------------------------

inline Edge2D::Vertex Edge2D::V1() const 
{ return Vertex(_e.TheCell().vertex(1,_e), TheGrid());}

inline Edge2D::Vertex Edge2D::V2() const 
{ return Vertex(_e.TheCell().vertex(2,_e), TheGrid());}

inline Edge2D::Cell   Edge2D::C1() const 
{ return (_e.TheCell());}
inline Edge2D::Cell   Edge2D::C2() const 
{ return _e.OtherCell();} 

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
  typedef Edge2D::Vertex Vertex;
  Vertex max1(__STD::max(s1.V1(),s1.V2()));
  Vertex min1(__STD::min(s1.V1(),s1.V2()));
  Vertex max2(__STD::max(s2.V1(),s2.V2()));
  Vertex min2(__STD::min(s2.V1(),s2.V2()));
  
  return ( (max1 < max2)  || ( (max1 == max2) && (min1 < min2)));
}



#endif
