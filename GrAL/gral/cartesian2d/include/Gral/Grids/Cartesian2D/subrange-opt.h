#ifndef NMWR_GB_REG2D_SUBRANGE_H
#define NMWR_GB_REG2D_SUBRANGE_H

////////////////////////////////////////////////////////////////
//
//   This class allows it to describe a rectangular subrange
//   of a regular 2D-grid. It defines altered versions of
//   the sequence iterators bounded by the range, but it
//   does not behave like a (sub)grid, the underlying grid
//   is still considered to be the original one.
//   Especially there is no means to iterate over the boundary
//   of the subrange.
//
//   (c) Guntram Berti
//   Chair for Numerical Mathematics & Scientific Computing (NMWR)
//   TU Cottbus - Germany
//   http://math-s.math.tu-cottbus.de/NMWR
//   
//   Created: Oct. 08, 1997
//
//   Changes:
//    Oct. 16, 1997: Added support for cell-iteration
//    Oct. 23, 1997: Added support for edge-iteration
//
////////////////////////////////////////////////////////////////

#include "Grids/Reg2D/cartesian-grid2d.h"

class SubrangeReg2D {
public:
  typedef SubrangeReg2D Range;
  typedef RegGrid2D Grid;
  typedef typename Grid::indexmap_type indexmap_type;
  typedef typename Grid::index_type index_type;

  typedef typename Grid::vertex_handle vertex_handle;
  typedef typename Grid::Vertex Vertex;
  typedef typename Grid::cell_handle cell_handle;
  typedef typename Grid::Cell Cell;
  typedef typename Grid::edge_handle edge_handle;
  typedef typename Grid::Edge Edge;

  SubrangeReg2D() : g(0) {}
  // what should be the behavior if not llv <= urv ? 
  SubrangeReg2D(const Grid& gg, const index_type& llv, const index_type& urv)
    : g(&gg), 
      vertex_index_map(llv,urv), 
      cell_index_map  (llv,index_type(urv.x-1,urv.y-1)),
      xedge_index_map (llv,index_type(urv.x-1,urv.y  )), 
      yedge_index_map (llv,index_type(urv.x,  urv.y-1))
    {}
  SubrangeReg2D(const Grid& gg,int llx, int lly, int urx, int ury) 
    : g(&gg), 
      vertex_index_map(index_type(llx,lly),index_type(urx,ury)),
      cell_index_map  (index_type(llx,lly),index_type(urx-1,ury-1)), 
      xedge_index_map (index_type(llx,lly),index_type(urx-1,ury  )), 
      yedge_index_map (index_type(llx,lly),index_type(urx,  ury-1))

    {}

  //  SubrangeReg2D(const Grid& gg, const rect& b) 
  // : g(gg), vertex_index_map(b.ll,b.ur) {}

  class iterator_base_1 {
  public:
    typedef iterator_base_1 base;
    iterator_base_1(const Grid* gg, const Range* rr) : g(gg), r(rr) {}
    iterator_base_1(const Grid& gg, const Range& rr) : g(&gg), r(&rr) {}
    const Grid&  TheGrid()  const { return (*g);}
    const Range& TheRange() const { return (*r);}
  protected:
    const Grid*  g;
    const Range* r;
  };

  class VertexIterator_1 : public iterator_base_1 {
  public:
    typedef VertexIterator_1 self;

    VertexIterator_1(const Grid* g, const Range* r) : base(g,r), v(r->MinVertexNum()) {}
    VertexIterator_1(const Grid& g, const Range& r) : base(g,r), v(r.MinVertexNum()) {}
    VertexIterator_1(vertex_handle vv, const Grid* g, const Range* r) 
      : base(g,r), v(vv) {}
    VertexIterator_1(vertex_handle vv, const Grid& g, const Range& r) 
      : base(g,r), v(vv) {}

    self& operator++() {
      ++v;
      return (*this);
    }
    self  operator++(int)    { self tmp(*this); ++(*this); return tmp;}
    self& operator +=(const index_type& ij) {
      v+= TheMap().offset(ij);
      return *this;
    }
    Vertex   operator*() const { return TheRange().vertex(v);} 
    // random access
    //    Vertex operator()(int i, int j) const { 
    //  return TheGrid().vertex(v + TheMap().offset(i,j));
    //}
    
    bool     IsDone()    const { return  (v > TheRange().MaxVertexNum());}
    friend bool operator==(const self& ls, const self& rs) { return (ls.v == rs.v);}
    friend bool operator< (const self& ls, const self& rs) { return (ls.v <  rs.v);}
  private:
    const indexmap_type& TheMap() const { return TheRange().TheVertexMap();}
    vertex_handle v;
  };
  typedef VertexIterator_1 VertexIterator;


  class CellIterator_1 : public iterator_base_1 {
  public:
    typedef CellIterator_1 self;

    CellIterator_1() : base(0,0), i(0), j(0) {}
    CellIterator_1(const Grid* g, const Range* r) 
      : base(g,r), i(r->llx()) {}
    CellIterator_1(const Grid& g, const Range& r) : base(g,r), c(r.MinCellNum()) {}
    CellIterator_1(cell_handle cc, const Grid* g, const Range* r) 
      : base(g,r), c(cc) {}
    CellIterator_1(cell_handle cc, const Grid& g, const Range& r) 
      : base(g,r), c(cc) {}

    self& operator++() {
      //++c;
      if(i< urx) ++i;
      else { i = llx; ++j;}
      return (*this);
    }
    // self  operator++(int)    { self tmp(*this); ++(*this); return tmp;}
    //self& operator +=(const index_type& ij) {
    //  c+= TheMap().offset(ij);
    //  return *this;
    // }
    //    Cell   operator*() const { return TheRange().cell(c);} 
    Cell   operator*() const { return TheGrid().cell(i,j);}
    cell_handle GlobalNumber() const { 
      return TheGrid().TheCellIndexMap.number(i,j);
    } 
    // random access
    // Cell operator()(int i, int j) const { 
    //  return TheGrid().cell(c + TheMap().offset(i,j));
    //}
    
    bool     IsDone()    const { return  (j > ury);}
    friend bool operator==(const self& ls, const self& rs) 
      { return ((ls.i == rs.i) && (ls.j == rs.j);}
    friend bool operator< (const self& ls, const self& rs) 
      { return ((ls.j < rs.j) ||( (ls.j == rs.j) && (ls.i < rs.i)));}
  private:
    const indexmap_type& TheMap() const { return TheRange().TheCellMap();}
    //    cell_handle c;
    int i,j;
    int llx,lly;
    int nx,ny;
  };
  typedef CellIterator_1 CellIterator;

  class EdgeIterator_1 : public iterator_base_1 {
  public:
    typedef EdgeIterator_1 self;

    EdgeIterator_1() : base(0,0), e(-1) {}
    EdgeIterator_1(const Grid* g, const Range* r) : base(g,r), e(r->MinEdgeNum()) {}
    EdgeIterator_1(const Grid& g, const Range& r) : base(g,r), e(r.MinEdgeNum()) {}
    EdgeIterator_1(edge_handle ee, const Grid* g, const Range* r) 
      : base(g,r), e(ee) {}
    EdgeIterator_1(edge_handle ee, const Grid& g, const Range& r) 
      : base(g,r), e(ee) {}

    self& operator++() {
      ++e;
      return (*this);
    }
    self  operator++(int)    { self tmp(*this); ++(*this); return tmp;}
    Edge   operator*() const { return TheRange().edge(e);} 

    //    self& operator +=(const index_type& ij) {
    //  e+= TheMap().offset(ij);
    //  return *this;
    // }
    // random access
    // Edge operator()(int i, int j) const { 
    //  return TheGrid().edge(e + TheMap().offset(i,j));
    //}
    
    bool     IsDone()    const { return  (e > TheRange().MaxEdgeNum());}
    friend bool operator==(const self& ls, const self& rs) { return (ls.e == rs.e);}
    friend bool operator< (const self& ls, const self& rs) { return (ls.e <  rs.e);}
  private:
    //    const indexmap_type& TheMap() const { return TheRange().TheEdgeMap();}
    edge_handle e;
  };
  typedef EdgeIterator_1 EdgeIterator;


  const Grid& TheGrid() const { return *g;}

  const indexmap_type& TheVertexMap() const {return vertex_index_map;}
  VertexIterator FirstVertex() const { return VertexIterator(TheGrid(),*this);}
  VertexIterator EndVertex()   const { return VertexIterator(MaxVertexNum() +1,TheGrid(),*this);}
  Vertex vertex(vertex_handle v) const { return TheGrid().vertex(TheVertexMap().index(v));}
  vertex_handle MinVertexNum()   const { return  TheVertexMap().n0();}
  vertex_handle MaxVertexNum()   const { return  TheVertexMap().nmax();}
  unsigned NumOfVertices() const { return TheVertexMap().range_size();}


  const indexmap_type& TheCellMap() const {return cell_index_map;}
  CellIterator FirstCell() const { return CellIterator(TheGrid(),*this);}
  CellIterator EndCell()   const { return CellIterator(MaxCellNum()+1,TheGrid(),*this);}
  Cell cell(cell_handle v) const { return TheGrid().cell(TheCellMap().index(v));}
  cell_handle MinCellNum()   const { return  TheCellMap().n0();}
  cell_handle MaxCellNum()   const { return  TheCellMap().nmax();}
  unsigned NumOfCells() const { return TheCellMap().range_size();}

  const indexmap_type& TheXEdgeMap() const {return xedge_index_map;}
  const indexmap_type& TheYEdgeMap() const {return yedge_index_map;}
  EdgeIterator FirstEdge() const { return EdgeIterator(TheGrid(),*this);}
  EdgeIterator EndEdge()   const { return EdgeIterator(MaxEdgeNum()+1,TheGrid(),*this);}
  Edge edge(edge_handle e) const { 
    return (e <= TheXEdgeMap().nmax() 
	    ?  TheGrid().xedge(TheXEdgeMap().index(e))
	    :  TheGrid().yedge(TheYEdgeMap().index(e - (TheXEdgeMap().nmax() + 1)
						     + TheYEdgeMap().n0()))); 
  }

  edge_handle MinEdgeNum()   const { return  TheXEdgeMap().n0();}
  edge_handle MaxEdgeNum()   const { return  MinEdgeNum() + NumOfEdges() -1;}
  unsigned NumOfEdges() const { return(  TheXEdgeMap().range_size() 
				       + TheYEdgeMap().range_size());}

  const index_type& ll() const {return TheVertexMap().ll();}
  const index_type& ur() const {return TheVertexMap().ur();}
private:
  const Grid* g;
  indexmap_type vertex_index_map;
  indexmap_type cell_index_map;
  indexmap_type xedge_index_map;
  indexmap_type yedge_index_map;
};

#endif
