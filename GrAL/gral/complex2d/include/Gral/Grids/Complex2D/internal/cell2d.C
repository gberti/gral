#ifndef NMWR_GB_CELL2D_C
#define NMWR_GB_CELL2D_C

// $LICENSE


#include "algorithm"
#include "functional"

inline Cell2D::Cell2D(Complex2D const&  cc)
{ *this = *(cc.FirstCell());}

inline bool Cell2D::is_valid() const { return (_pos != _cc->outer_cell_handle());}

inline
cell2d_connectivity const& 
Cell2D::base() const { return (_cc->_cells[_pos]);}


//----------------------  iteration -----------------------------

inline VertexOnCell2D_Iterator  Cell2D::FirstVertex() const
{ return    VertexOnCellIterator(0,*this);}
inline VertexOnCell2D_Iterator  Cell2D::EndVertex() const
{ return    VertexOnCellIterator(NumOfVertices(),*this);}

inline EdgeOnCell2D_Iterator  Cell2D::FirstEdge() const
{ return EdgeOnCellIterator(0,*this);}
inline EdgeOnCell2D_Iterator  Cell2D::EndEdge() const
{ return EdgeOnCellIterator(NumOfEdges(),*this);}

inline   EdgeOnCell2D_Iterator  Cell2D::FirstFacet() const { return FirstEdge();}
inline   EdgeOnCell2D_Iterator  Cell2D::EndFacet()   const { return EndEdge();}


inline CellOnCell2D_Iterator Cell2D::FirstNeighbour() const
{ return CellNeighbourIterator(0,*this);}

inline CellOnCell2D_Iterator Cell2D::EndNeighbour() const
{ 
  // don't use NumOfNeighbors() here, it may be smaller than
  // NumOfFacets().
  return CellNeighbourIterator(NumOfFacets(),*this);
}

inline CellOnCell2D_Iterator Cell2D::FirstCell()  const { return FirstNeighbour();}
inline CellOnCell2D_Iterator Cell2D::EndCell()    const { return EndNeighbour();}


inline int Cell2D::NumOfNeighbours() const
{ 
  return count_if(base()._neighbours.begin(), base()._neighbours.end(), 
		  bind2nd(std::not_equal_to<int>(),-1)); 
}

inline Vertex2D Cell2D::V(int i) const
{
  REQUIRE((is_valid()),"Cell2D: action with invalid cell!",1);
  REQUIRE((0 <= i && i < NumOfVertices()),\
	  "Cell2D::Vertex(int i): i = " << i\
	  << ", must be in [0," << NumOfVertices()-1 << "] !", 1);

  return( _cc->vertex(base()._vertices[i]));
}

inline Edge2D Cell2D::E(int i) const
{
  REQUIRE((is_valid()),"Cell2D: action with invalid cell!",1);
  REQUIRE((1 <= i && i <= NumOfEdges()),\
	  "Cell2D::Edge(int i): i = " << i\
	  << ", must be in [1," << NumOfEdges() << "] !", 1);

  return( Edge(EdgeOnCellIterator(i-1,*this)));
}

inline Edge2D Cell2D::F(int i) const { return E(i);}



inline Cell2D::vertex_handle
Cell2D::vertex(int lv)  const    
{ 
  REQUIRE((is_valid()),"Cell2D: action with invalid cell!",1);
  REQUIRE( ((0 <= lv) && (lv < NumOfVertices())), 
	    "local vertex lv = " << lv  
	   << " out of range [0, " << NumOfVertices() << "]\n",1);
  return (base()._vertices[lv]);
} 

inline Cell2D::cell_handle
Cell2D::cell(int lc)  const    
{ 
  REQUIRE((is_valid()),"Cell2D: action with invalid cell!",1);
  return (base()._neighbours[lc]);
} 


inline Cell2D::vertex_handle Cell2D::vertex(unsigned side, 
					    const EdgeOnCell2D_Iterator& nb) const
{
  REQUIRE((is_valid()),"Cell2D: action with invalid cell!",1);
  REQUIRE( ((side == 1 ) || (side == 2)),\
           "Cell2D::_vertex : side was " << side << ", must be in {1,2}! ", 1);

  int v = ((nb.lf + side - 1) % NumOfVertices());
  return (base()._vertices[v]);
}

inline int Cell2D::find_local_facet(const Cell2D& Nb) const
{
  REQUIRE((is_valid()),"Cell2D: action with invalid cell!",1);
  unsigned cnt = 0;
  cell_handle hNb = _cc->handle(Nb);
  while(cnt != base()._neighbours.size() && base()._neighbours[cnt] != hNb) {
    ++cnt;
  }
  ENSURE( ((int)cnt < NumOfFacets()),
	  "Cell2D " << _pos << " _find_facet: neighbour " << Nb._pos << " not found! ",1);
  
  return(cnt);
}


inline  Edge2D 
Cell2D::FlippedEdge(Vertex2D const& v, Edge2D const& e) const
{ Edge res = e; FlipEdge(v,res); return res;}


inline void Cell2D::FlipEdge(const Vertex2D& v, Edge2D& e) const
{
  REQUIRE((is_valid()),"Cell2D: action with invalid cell!",1);
  REQUIRE(((v == e.V1()) || (v == e.V2())),
	  "FlipEdge(v,e): v not in {e.V1,e.V2} !\n",1);
  EdgeOnCell2D_Iterator f = FirstEdge();
  while( (! (*f == e))  && ( ! f.IsDone())) {
    ++f;
  }
  
  REQUIRE((*f == e), 
	  "FlipEdge(): nb-it does not match! f found = " 
	  << "(" << f << ")"
	  << ", e._e = (" << e._e  <<")\n",1);

  if(vertex(1,f) == v.handle()) 
    f = f.CyclicPred();
  else // *(++w) == v
    f = f.CyclicSucc();
 
  e._e = f;
}


#endif
