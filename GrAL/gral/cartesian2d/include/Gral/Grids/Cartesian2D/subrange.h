#ifndef NMWR_GB_REG2D_SUBRANGE_H
#define NMWR_GB_REG2D_SUBRANGE_H


// $LICENSE

#include "Gral/Grids/Cartesian2D/cartesian-grid2d.h"

/*! \brief Rectangular subrange of RegGrid2D

   This class allows it to describe a rectangular subrange
   of a regular 2D-grid. It defines altered versions of
   the sequence iterators bounded by the range, but it
   does not behave like a (sub)grid, the underlying grid
   is still considered to be the original one.
   Especially there is no means to iterate over the boundary
   of the subrange.
*/

namespace cartesian2d {

class SubrangeReg2D {
public:
  typedef SubrangeReg2D Range;
  typedef RegGrid2D Grid;
  typedef Grid      grid_type;
  typedef  Grid::indexmap_type indexmap_type;
  typedef  Grid::index_type index_type;

  typedef  Grid::vertex_handle vertex_handle;
  typedef  Grid::Vertex Vertex;
  typedef  Grid::cell_handle cell_handle;
  typedef  Grid::Cell Cell;
  typedef  Grid::edge_handle edge_handle;
  typedef  Grid::Edge Edge;

  SubrangeReg2D() : g(0) {}
  // what should be the behavior if  llv ! <= urv ? 
  SubrangeReg2D(const Grid& gg, const index_type& llv, const index_type& urv)
    : g(&gg), 
      vertex_index_map(llv,urv), 
      cell_index_map  (llv,index_type(urv.x()-1,urv.y()-1)),
      xedge_index_map (llv,index_type(urv.x()-1,urv.y()  )), 
      yedge_index_map (llv,index_type(urv.x(),  urv.y()-1))
    {}
  SubrangeReg2D(const Grid& gg,int llx, int lly, int urx, int ury) 
    : g(&gg), 
      vertex_index_map(index_type(llx,lly),index_type(urx,ury)),
      cell_index_map  (index_type(llx,lly),index_type(urx-1,ury-1)), 
      xedge_index_map (index_type(llx,lly),index_type(urx-1,ury  )), 
      yedge_index_map (index_type(llx,lly),index_type(urx,  ury-1))

    {}
  // hack: Allow non-closed subranges
  SubrangeReg2D(const Grid& gg, 
		const index_type& llv_v, const index_type& urv_v,
		const index_type& llv_c, const index_type& urv_c)
    : g(&gg),
      vertex_index_map(llv_v, urv_v),
      cell_index_map(llv_c, urv_c),
      // this is a hack ... does not lead to good edge partitioning
      xedge_index_map (llv_v,index_type(urv_v.x()-1,urv_v.y()  )), 
      yedge_index_map (llv_v,index_type(urv_v.x(),  urv_v.y()-1))
  {}

  static unsigned dimension() { return 2;}

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

    VertexIterator_1() : base(0,0), v(-1) {}
    VertexIterator_1(const Range& r) : base(r.TheGrid(), r) , v(r.MinVertexNum()) {}
    VertexIterator_1(const Grid* g, const Range* r) : base(g,r), v(r->MinVertexNum()) {}
    VertexIterator_1(const Grid& g, const Range& r) : base(g,r), v(r.MinVertexNum()) {}
    VertexIterator_1(vertex_handle vv, const Grid* g, const Range* r) 
      : base(g,r), v(vv) {}
    VertexIterator_1(vertex_handle vv, const Grid& g, const Range& r) 
      : base(g,r), v(vv) {}

    self& operator++()     { ++v; return (*this); }
    self  operator++(int)  { self tmp(*this); ++(*this); return tmp;}
    Vertex   operator*() const { return TheRange().vertex(v);} 
    vertex_handle handle() const { return v;}
    
    bool     IsDone()    const { return  (v > TheRange().MaxVertexNum());}
    friend bool operator==(const self& ls, const self& rs) { return (ls.v == rs.v);}
    friend bool operator!=(const self& ls, const self& rs) { return !(ls == rs);}
    friend bool operator< (const self& ls, const self& rs) { return (ls.v <  rs.v);}

    //    vertex_handle GlobalNumber() const {return v;}
  private:
    const indexmap_type& TheMap() const { return TheRange().TheVertexMap();}
    vertex_handle v;
  };
  typedef VertexIterator_1 VertexIterator;


  class CellIterator_1 : public iterator_base_1 {
  public:
    typedef CellIterator_1 self;

    CellIterator_1() : base(0,0), c(-1) {}
    CellIterator_1(const Range& r) : base(r.TheGrid(), r), c(r.MinCellNum()) {}
    CellIterator_1(const Grid* g, const Range* r) : base(g,r), c(r->MinCellNum()) {}
    CellIterator_1(const Grid& g, const Range& r) : base(g,r), c(r.MinCellNum()) {}
    CellIterator_1(cell_handle cc, const Grid* g, const Range* r) 
      : base(g,r), c(cc) {}
    CellIterator_1(cell_handle cc, const Grid& g, const Range& r) 
      : base(g,r), c(cc) {}

    self& operator++() {
      ++c;
      return (*this);
    }
    self  operator++(int)    { self tmp(*this); ++(*this); return tmp;}
    self& operator +=(const index_type& ij) {
      c+= TheMap().offset(ij);
      return *this;
    }
    Cell   operator*() const { return TheRange().cell(c);} 
    cell_handle handle() const { return c;}    

    bool     IsDone()    const { return  (c > TheRange().MaxCellNum());}
    friend bool operator==(const self& ls, const self& rs) { return (ls.c == rs.c);}
    friend bool operator!=(const self& ls, const self& rs) { return !(ls == rs);}
    friend bool operator< (const self& ls, const self& rs) { return (ls.c <  rs.c);}
    //    cell_handle GlobalNumber() const {
  private:
    const indexmap_type& TheMap() const { return TheRange().TheCellMap();}
    cell_handle c;
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
    friend bool operator!=(const self& ls, const self& rs) { return !(ls == rs);}
    friend bool operator< (const self& ls, const self& rs) { return (ls.e <  rs.e);}
  private:
    //    const indexmap_type& TheMap() const { return TheRange().TheEdgeMap();}
    edge_handle e;
  };
  typedef EdgeIterator_1 EdgeIterator;



  const Grid& TheGrid() const { return *g;}

  const index_type& ll() const {return TheVertexMap().ll();}
  const index_type& ur() const {return TheVertexMap().ur();}

  int  llx() const { return TheVertexMap().llx();}
  int  lly() const { return TheVertexMap().lly();}
  int  urx() const { return TheVertexMap().urx();}
  int  ury() const { return TheVertexMap().ury();}
  index_type side_vertex1(int s) const {
    return index_type(llx()+(NumOfXVertices()-1)* Grid::side_vertex_1_[s-1].x(),
		      lly()+(NumOfYVertices()-1)* Grid::side_vertex_1_[s-1].y());
  }
  index_type side_vertex2(int s) const {
    return index_type(llx()+(NumOfXVertices()-1)* Grid::side_vertex_2_[s-1].x(),
		      lly()+(NumOfYVertices()-1)* Grid::side_vertex_2_[s-1].y());
  }
  
  const indexmap_type& TheVertexMap() const {return vertex_index_map;}
  VertexIterator FirstVertex() const { return VertexIterator(TheGrid(),*this);}
  VertexIterator EndVertex()   const { return VertexIterator(MaxVertexNum() +1,TheGrid(),*this);}
  vertex_handle handle(Vertex const& V) const { return TheGrid().handle(V);}
  Vertex vertex(vertex_handle v) const { return TheGrid().vertex(TheVertexMap().index(v));}
  vertex_handle MinVertexNum()   const { return  TheVertexMap().n0();}
  vertex_handle MaxVertexNum()   const { return  TheVertexMap().nmax();}
  unsigned NumOfVertices() const { return TheVertexMap().range_size();}
  int NumOfXVertices() const { return (urx()-llx()+1);}
  int NumOfYVertices() const { return (ury()-lly()+1);}

  const indexmap_type& TheCellMap() const {return cell_index_map;}
  CellIterator FirstCell() const { return CellIterator(TheGrid(),*this);}
  CellIterator EndCell()   const { return CellIterator(MaxCellNum()+1,TheGrid(),*this);}
  cell_handle handle(Cell const& C) const { return TheGrid().handle(C);}
  Cell cell(cell_handle v) const { return TheGrid().cell(TheCellMap().index(v));}
  cell_handle MinCellNum()   const { return  TheCellMap().n0();}
  cell_handle MaxCellNum()   const { return  TheCellMap().nmax();}
  unsigned NumOfCells() const { return TheCellMap().range_size();}
  int NumOfXCells() const { return (urx()-llx());}
  int NumOfYCells() const { return (ury()-lly());}


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
  // aliases for facets
  unsigned NumOfFacets() const { return NumOfEdges();}
  EdgeIterator FirstFacet() const {return FirstEdge();}
  EdgeIterator EndFacet()   const {return EndEdge();} // past-the-end!

  /*! \name Archetype handling
   */
  /*@{*/ 
  typedef grid_type::archetype_iterator archetype_iterator;
  typedef grid_type::archetype_type     archetype_type;
  typedef grid_type::archetype_handle   archetype_handle;

  static archetype_iterator BeginArchetype() { return grid_type::BeginArchetype();}
  static archetype_iterator EndArchetype()   { return grid_type::EndArchetype();}
  static archetype_type const& Archetype(archetype_handle = 0) { return *BeginArchetype();} 
  static archetype_type const& ArchetypeOf (Cell const&)  
    { return *BeginArchetype();}
  static archetype_type   const& ArchetypeOf (cell_handle) 
    { return *BeginArchetype();}
  static archetype_handle        archetype_of(cell_handle) 
    { return 0;}
  static archetype_handle        archetype_of(Cell const&) 
    { return 0;}
  static unsigned NumOfArchetypes() { return 1;}
  /*@}*/

private:
  const Grid* g;
  indexmap_type vertex_index_map;
  indexmap_type cell_index_map;
  indexmap_type xedge_index_map;
  indexmap_type yedge_index_map;
};

} // namespace cartesian2d


/*! \brief specialization of grid_types for SubrangeReg2D

 */
struct grid_types<cartesian2d::SubrangeReg2D> {
  typedef  cartesian2d::SubrangeReg2D range_type;
  typedef  range_type::VertexIterator VertexIterator;
  typedef  range_type::EdgeIterator   EdgeIterator;
  typedef  range_type::EdgeIterator   FacetIterator;
  typedef  range_type::CellIterator   CellIterator;

  typedef grid_types<cartesian2d::RegGrid2D> gt;
  typedef  gt::Vertex Vertex;
  typedef  gt::Edge   Edge;
  typedef  gt::Facet  Facet;
  typedef  gt::Cell   Cell;

  typedef  gt::vertex_handle vertex_handle; 
  typedef  gt::edge_handle   edge_handle;
  typedef  gt::cell_handle   cell_handle;

  typedef  gt::VertexOnCellIterator VertexOnCellIterator;
  typedef  gt::EdgeOnCellIterator   EdgeOnCellIterator;
  typedef  gt::CellOnCellIterator   CellOnCellIterator;

  typedef  gt::archetype_type     archetype_type;
  typedef  gt::archetype_handle   archetype_handle;
  typedef  gt::archetype_iterator archetype_iterator;
  typedef  archetype_iterator     ArchetypeIterator; 
  typedef  grid_types<archetype_type>    archgt;

};


#endif
