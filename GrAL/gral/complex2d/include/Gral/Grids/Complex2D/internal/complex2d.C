
// $LICENSE

// inline functions for  class Complex2D,
// to be included in complex2d.h

#include "algorithm"
 
inline Complex2D::vertex_handle Complex2D::handle(const Complex2D::Vertex& V)  const { 
  REQUIRE(( &(V.TheGrid()) == this),
	  "handle(Vertex): V.TheGrid() == " 
	  <<  &(V.TheGrid()) << " != this = " << this << '\n',1);
  return (V._pos);
}

inline Complex2D::vertex_handle Complex2D::handle(const Complex2D::VertexIterator& v) const
{ return v._pos;}
inline Complex2D::vertex_handle Complex2D::handle(const Complex2D::VertexOnCellIterator& v) const
 { return v.handle();}


inline  Complex2D::cell_handle   Complex2D::handle(const Complex2D::Cell& C) const { 
  REQUIRE(( &(C.TheGrid()) == this),
	  "handle(Cell): C.TheGrid() == " 
	  <<  &(C.TheGrid()) << " != this = " << this << '\n',1);
  return (C._pos);
}

inline Complex2D::cell_handle   Complex2D::handle(const Complex2D::CellIterator& c) const
 { return c._pos;}
inline Complex2D::cell_handle   Complex2D::handle(const Complex2D::CellNeighbourIterator& c) const 
{ return (_cells[c._f.C.handle()]._neighbours[c._f.lf]); }

inline Edge2D Complex2D::edge(const Complex2D::edge_handle& e) const 
{ return Edge(EdgeOnCellIterator(e.le,Cell(*this,e.c)));}

inline Edge2D Complex2D::facet(const Complex2D::edge_handle& e) const 
{ return Edge(EdgeOnCellIterator(e.le,Cell(*this,e.c)));}

inline Complex2D::edge_handle Complex2D::handle(const Edge2D& E) const
{
  // We must ensure E1 == E2  <=> handle(E1) == handle(E2) 
  cell_handle c1 = E._e.C.handle();
  cell_handle c2 = _cells[c1]._neighbours[E._e.lf];
  if (c1 > c2)
    return edge_handle(c1,E._e.lf);
  else // c2 cannot be outer_pos()
    return edge_handle(c2, find(_cells[c2]._neighbours.begin(),
				_cells[c2]._neighbours.end(), c1)
		           - _cells[c2]._neighbours.begin());
}

inline  bool Complex2D::IsOnBoundary(const EdgeOnCell2D_Iterator& e) const 
{ return (_cells[e.C.handle()]._neighbours[e.lf] == outer_cell_handle());}
inline  bool Complex2D::IsOnBoundary(const Edge2D& e) const 
{ return (IsOnBoundary(e._e));}



//---------------------- iteration functionality ------------------------

inline Complex2D::VertexIterator  Complex2D::FirstVertex() const
{ return VertexIterator(0,*this);}

inline Complex2D::VertexIterator  Complex2D::EndVertex() const
{ return VertexIterator(NumOfVertices(),*this);}


inline Complex2D::EdgeIterator  Complex2D::FirstEdge() const
{ 
  CellIterator FC(FirstCell());
  if( ! FC.IsDone())
    return EdgeIterator(FC,(*(FC)).FirstEdge());
  else
    return EdgeIterator(FC,EdgeOnCellIterator());
}
inline Complex2D::EdgeIterator  Complex2D::EndEdge() const
{ return  EdgeIterator(EndCell(), EdgeOnCellIterator()); }

inline Complex2D::EdgeIterator  Complex2D::FirstFacet() const
{ return FirstEdge();}
inline Complex2D::EdgeIterator  Complex2D::EndFacet() const
{ return EndEdge();}

inline void Complex2D::switch_vertex(Complex2D::Vertex    & v, 
				     Complex2D::Edge const& e) const
{ e.FlipVertex(v);}

inline void Complex2D::switch_edge(Complex2D::Vertex const& v, 
				   Complex2D::Edge        & e, 
				   Complex2D::Cell   const& c) const
{ c.FlipEdge(v,e);}

 
inline void Complex2D::switch_cell(Complex2D::Edge const& e, 
				   Complex2D::Cell      & c) const
{ e.FlipCell(c);}



inline Complex2D::BoundaryFacetIterator Complex2D::FirstBoundaryFacet() const
{ return BoundaryFacetIterator(_boundary.begin(),this);}

inline Complex2D::BoundaryFacetIterator Complex2D::FirstBoundaryEdge()  const
{ return BoundaryFacetIterator(_boundary.begin(),this);}

inline Complex2D::BoundaryVertexIterator Complex2D::FirstBoundaryVertex() const 
{ 
  // note: this works only if there is at most one boundary component!

  if (! FirstBoundaryFacet().IsDone()) {
    return BoundaryVertexIterator(* (FirstBoundaryFacet()));
  }
  else // no boundary (e.g. if this is a sphere)
    return BoundaryVertexIterator();
}

inline Complex2D::CellIterator  Complex2D::FirstCell() const
{ return CellIterator(0,*this);}
inline Complex2D::CellIterator  Complex2D::EndCell() const
{ return CellIterator(NumOfCells(),*this);}


//---------------------- element creation -------------------------------

inline Complex2D::cell_handle Complex2D::_new_cell(int num_of_v)
{
  _cells.push_back(cell2d_connectivity());
  _cells.back().resize(num_of_v);
  return (_cells.size() -1);
}

inline Complex2D::vertex_handle Complex2D::_new_vertex(const Complex2D::CoordType& coo)
{
  _vertices.push_back(vertex_base(coo));
  _vertices.back().reserve(4); // heuristical, could be a member variable.
  return (_vertices.size() -1);
}


//---------------------- adjacency update functions ---------------------

inline void Complex2D::set_neighbour(const Complex2D::EdgeOnCellIterator& n1_it, 
				     const Complex2D::Cell& n2)
{ _cells[n1_it.C.handle()]._neighbours[n1_it.lf] = handle(n2);}

inline void Complex2D::add_cell_on_vertex(const Complex2D::Vertex& V, const Complex2D::Cell& C)
{ add_cell_on_vertex(V,handle(C)); }

inline void Complex2D::add_cell_on_vertex(const Complex2D::Vertex& V, const Complex2D::cell_handle& c)
{ _vertices[handle(V)]._cells.push_back(c);}


//---------------------- geometric functionality ------------------------

inline const Complex2D::CoordType& Complex2D::Coord(const Complex2D::Vertex&   v) const 
{ return (_vertices[v._pos]._coord); }

inline  Complex2D::CoordType& Complex2D::Coord(const Complex2D::Vertex&   v)  
{ return (_vertices[v._pos]._coord);}

