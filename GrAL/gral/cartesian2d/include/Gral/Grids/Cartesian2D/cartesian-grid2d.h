#ifndef NMWR_GB_CARTESIAN_GRID_2D_H
#define NMWR_GB_CARTESIAN_GRID_2D_H


// $LICENSE


#include <string>


#include "Utility/pre-post-conditions.h"

#include "Gral/Base/common-grid-basics.h" 
#include "Gral/Base/element-handle.h"
#include "Gral/Iterators/vertex-on-edge-iterator.h"

#include "Gral/Grids/Cartesian2D/index-map.h"
#include "Gral/Base/polygon.h"


namespace cartesian2d {

class RegGrid2D;
typedef RegGrid2D CartesianGrid2D;

/*! \brief A two-dimensional cartesian grid type

    RegGrid2D implements the full kernel interface.

    \todo The constructors are not consistent, allow only
     constructors of the form 
     - \c  RegGrid2D(vertex_size_type nv)
     -  <tt>RegGrid2D(vertex_index_type low, vertex_index_type high) </tt>
 */ 

class RegGrid2D {
public:
  //@{ 
  // typedef int dummy;
  //@} dummy group (work around)

  typedef xmjr_indexmap2D indexmap_type;
  // iteration is done in y-direction.
  // replacing this with ymjr_... would yield a regular grid
  // in which iteration is done in x-direction.
  typedef  indexmap_type::index_type index_type; // 2D-integer index
  typedef index_type                 vertex_index_type;
  typedef index_type                 cell_index_type;

private:
  // DATA 
  index_type ll_,ur_;   // integer coordinates of lower left and upper right vertices
                        // ll does not have to be (0,0)
  int xpoints, ypoints; // number of vertices in x and y direction
  // this is redundant information: 
  //   xpoints == ur_.x() -ll_.x() + 1
  //   ypoints == ur_.y() -ll_.y() + 1

  // maps between 1D and 2D integer indices
  // see init_maps for information about their relationship
  indexmap_type vertex_index_map;
  indexmap_type cell_index_map;
  indexmap_type xedge_index_map;
  indexmap_type yedge_index_map;

public:
  enum { dim = 2};

  /*! @name Constructors  */
  //@{ 
  RegGrid2D(int pts = 2) // default: 1 cell, 4 vertices
    :ll_(0,0), ur_(pts-1,pts-1)
  { init(); }
 
  RegGrid2D(int nv_x, int nv_y) 
    : ll_(0,0), ur_(nv_x-1,nv_y-1)
  { init(); }

  RegGrid2D(const index_type& nv)
    : ll_(0,0), ur_(nv-index_type(1,1))
  { init(); }

  RegGrid2D(int llx, int lly,int urx, int ury)
    : ll_(llx,lly), ur_(urx,ury)
  { init(); }

  RegGrid2D(const index_type& LL, const index_type& UR)
    :  ll_(LL), ur_(UR)
  { init(); }


 
  //@}

private:
  void init()
    { 
      xpoints = ur_.x() - ll_.x() +1;
      ypoints = ur_.y() - ll_.y() +1;
      vertex_index_map = indexmap_type(ll_,ur_);
      cell_index_map   = indexmap_type(ll_,index_type(ur_.x()-1,ur_.y()-1)); 
      xedge_index_map  = indexmap_type(ll_,index_type(ur_.x()-1,ur_.y()  )); 
      yedge_index_map  = indexmap_type(ll_,index_type(ur_.x(),  ur_.y()-1)); 
    }

public:
  /*! @name access to shape information */
  //@{  
  const index_type& ll() const { return ll_;}
  const index_type& ur() const { return ur_;}
  int llx() const { return ll_.x();}
  int lly() const { return ll_.y();}
  int urx() const { return ur_.x();}
  int ury() const { return ur_.y();}

  index_type vertex_size() const { return ur_ - ll_ + index_type(1);}
  index_type cell_size()   const { return ur_ - ll_;}
  //@}


private:
  
  const indexmap_type& TheVertexMap() const {return vertex_index_map;}
  const indexmap_type& TheXEdgeMap()  const {return xedge_index_map;}
  const indexmap_type& TheYEdgeMap()  const {return yedge_index_map;}
  const indexmap_type& TheCellMap()   const {return cell_index_map;}


  // static DATA:  2D-increments for local neighbour offsets
  static index_type side_offset_  [4];
  static index_type corner_offset_[4];  
  static index_type direction_    [4];  
  static index_type side_vertex_1_[4];
  static index_type side_vertex_2_[4];

  // names for I/O
  static std::string  side_name_  [4];
  static std::string  corner_name_[4];

public:
  typedef polygon1d::polygon      archetype_type;
  typedef archetype_type const*   archetype_iterator;
  typedef unsigned                archetype_handle;
private:
  // static data.
  // TODO: move all of the above into this class
  class SD {
  public:
    SD();

    typedef RegGrid2D grid_type;
    grid_type::archetype_type the_archetype[1];
  };
  static SD sd;
public:

  // map strings to side-/corner-enum and vice versa
  // this recognises different spellings, e.g. "S", "s", "South", "south".
  static int get_side(const std::string& nm);
  static int get_corner(const std::string& nm);
  static std::string side_name(int side) { return side_name_[side-1];} // returns "S", "N" etc.
  static std::string corner_name(int corner) { return corner_name_[corner-1];} // returns "SW", "NW" etc.

  static int invalid_side();
  static int invalid_corner();

  static bool valid_v(int c) { return (0 <= c && c < 4);}
  static void cv_v   (int c) { REQUIRE(valid_v(c), "  c=" << c, 1);} 
  // c in {SW,SE,NE,NW}
  static const index_type& corner_offset(int c) { cv_v(c); return corner_offset_[c];}
  static int opposite_corner(int c)  { cv_v(c); return ((c+2) % 4);}

  // s in {S,E,N,W}
  static bool valid_e(int c) { return (0 <= c && c < 4);}
  static void cv_e   (int c) { REQUIRE(valid_e(c), "  c=" << c, 1);} 
 
  static int   opposite_side(int s)   { cv_e(s); return ((s+2) % 4);}
  static const index_type& side_offset(int s)   { cv_e(s); return side_offset_[s];}
  static const index_type& direction(int s)     { cv_e(s); return direction_[s];}
  static const index_type& outer_normal(int s)  { cv_e(s); return side_offset_[s];}

  index_type side_vertex1(int s) const { return index_type(llx()+(xpoints-1)*side_vertex_1_[s].x(),
							   lly()+(ypoints-1)*side_vertex_1_[s].y());}
  index_type side_vertex2(int s) const { return index_type(llx()+(xpoints-1)*side_vertex_2_[s].x(),
							   lly()+(ypoints-1)*side_vertex_2_[s].y());}
public:
  typedef RegGrid2D Grid;
  
  typedef vertex_handle_int<RegGrid2D> vertex_handle;
  typedef edge_handle_int<RegGrid2D>   edge_handle;
  typedef cell_handle_int<RegGrid2D>   cell_handle;

  cell_handle invalid_cell()      const {return TheCellMap().n0() -1;}
  cell_handle outer_cell_handle() const {return TheCellMap().n0() -1;}
 
  class Vertex;
  class Edge;
  class Cell;

  class VertexIterator;
  class EdgeIterator;
  class CellIterator;
  class VertexOnVertexIterator;
  class CellOnVertexIterator;

  class VertexOnCellIterator;
  class EdgeOnCellIterator;
  class CellOnCellIterator;

  typedef vertex_on_edge_iterator<RegGrid2D> VertexOnEdgeIterator;

  unsigned dimension() const { return 2;}

  /*! \name Checking functions */
 /*@{*/ 
  void cv(vertex_handle e) const { REQUIRE(valid_handle(e), "",1);}
  bool valid_handle(vertex_handle e) const { return (0 <= e && e < (int)NumOfVertices()); }
  void cv(edge_handle e) const { REQUIRE(valid_handle(e), "",1);}
  bool valid_handle(edge_handle e) const { return (0 <= e && e < (int)NumOfEdges()); }
  void cv(cell_handle e) const { REQUIRE(valid_handle(e), "",1);}
  bool valid_handle(cell_handle e) const { return (0 <= e && e < (int)NumOfCells()); }
  /*@}*/


  //-----------------------------------------------------------
  //
  // now the definitions of local classes:
  // Elements: Vertex, Edge, Cell,
  // Sequence Iterators: VertexIterator, EdgeIterator, CellIterator,
  // Adjacency Iterators: VertexOnCellIterator, EdgeOnCellIterator, 
  //                      CellOnCellIterator,
  //                      VertexOnVertexIterator
  //
  //-----------------------------------------------------------


  //-----------------------------------------------------------
  //
  // ELEMENTS (VERTEX, EDGE, CELL)
  //
  //-----------------------------------------------------------
 
  /*! @name Element classes */
  //@{ 
  class elem_base { // common base for vertex, edge, cell
  public:
    typedef RegGrid2D grid_type;
    typedef RegGrid2D anchor_type;
    typedef grid_type::index_type index_type;

    elem_base() : _g((const Grid*)0) {}
    elem_base(const Grid* g) :_g(g) {}
     
    grid_type const& TheGrid() const {
      REQUIRE((_g != 0),"No Grid!\n",1);
      return (*_g);
    }
    grid_type const& TheAnchor() const { return TheGrid();}

    bool bound() const { return _g != 0;} 
  protected: 
    const Grid* _g;
  };

  typedef index_type vertex_base;

  //------------------ VERTEX ---------------------

  class Vertex : public elem_base {
    typedef Vertex self;
    friend class Edge;
  public:
    typedef  VertexOnVertexIterator VertexIterator;
    typedef  VertexOnVertexIterator NeighbourIterator;

    Vertex() {}
    Vertex(Grid const& g, vertex_handle v) { *this = g.vertex(v);}

    Vertex(const vertex_base& v, const Grid* g) :  elem_base(g),  _v(v) {}
    Vertex(const vertex_base& v, const Grid& g) :  elem_base(&g), _v(v) {}
    int x() const { return _v.x();}
    int y() const { return _v.y();}
 
    
    inline VertexOnVertexIterator FirstVertex() const;
    inline VertexOnVertexIterator EndVertex() const;
    inline VertexOnVertexIterator FirstNeighbour() const;
    inline VertexOnVertexIterator EndNeighbour() const;
    inline CellOnVertexIterator   FirstCell() const;
    inline CellOnVertexIterator   EndCell() const;

    int NumOfVertices() const { 
      return 4 - ( (x() > _g->ll().x() ? 0 : 1) +
		   (x() < _g->ur().x() ? 0 : 1) +
		   (y() > _g->ll().y() ? 0 : 1) +
		   (y() < _g->ur().y() ? 0 : 1));
    }
    int NumOfCells() const {
      return 4 - ( (x()   > _g->llx() && y() > _g->lly() ? 0 : 1) +
		   (x()   < _g->urx() && y() > _g->lly() ? 0 : 1) +
		   (x()   > _g->llx() && y()   < _g->ury() ? 0 : 1) +
 		   (x()   < _g->urx() && y()   < _g->ury() ? 0 : 1));
    }
    int next_neighbour(int nb) const {
      if( !TheGrid().IsOnBoundary(*this)) return ++nb;
      do { nb++; } while( (nb <= 3) && (! TheGrid().IsValid(get_nb_base(nb))));
      return nb;
    }

    int next_cell(int c) const {
      if( !TheGrid().IsOnBoundary(*this)) return ++c;
      do { c++; } while( (c <= 3) && (! TheGrid().IsValidCellBase(get_cell_base(c))));
      return c;
    }

    vertex_base get_nb_base(int nb) const { // 0 <= nb <=  3, no check if Neighbour exists!
      return vertex_base(x() + TheGrid().side_offset((int)nb).x(),
			 y() + TheGrid().side_offset((int)nb).y());
    }

    vertex_base get_cell_base(int c) const { // 0 <= nb <= 3, no check if Neighbour exists!
      return vertex_base(x() + TheGrid().corner_offset((int)c).x() -1,
			 y() + TheGrid().corner_offset((int)c).y() -1);
    }
   
    Vertex vertex(int nb) const { return Vertex(get_nb_base(nb),TheGrid());}
    inline Cell   cell(int c) const;

    friend bool operator==(const self& lhs, const self& rhs) 
    {return (lhs.GlobalNumber() == rhs.GlobalNumber());}
    friend bool operator!=(const self& lhs, const self& rhs) 
      { return ! (lhs == rhs);}
    friend bool operator<(const self& lhs, const self& rhs) 
    {return (lhs.GlobalNumber() < rhs.GlobalNumber());}

    vertex_handle GlobalNumber() const {return TheGrid().vertex_num(_v);}
    vertex_handle handle      () const {return TheGrid().vertex_num(_v);}

    const index_type&  index() const { return _v;}
    //    const vertex_base& base() const {return _v;}
    bool valid() const { return TheGrid().IsValid(_v);}
  protected:
    index_type _v;
  };

  //-------------------- EDGE ---------------------

  class Edge : public elem_base {
  public:
    enum   direction {x_dir,y_dir};

    friend class RegGrid2D;
    typedef RegGrid2D         Grid;
    typedef Edge self;

    Edge()  {}
    Edge(Grid const& g, edge_handle e) { *this = g.edge(e);}

    Edge(direction d, const vertex_base& v, const Grid* g)
      : elem_base(g), dir(d), v1_(v)  {}
    Edge(direction d, const vertex_base& v, const Grid& g)
      : elem_base(&g), dir(d), v1_(v)  {}
    Edge(const Vertex& w1, const Vertex& w2) : elem_base(&(w1.TheGrid()))
      { init(w1._v,w2._v);}
    Edge(const vertex_base& w1, const vertex_base& w2, const Grid& g) : elem_base(&g)
      { init(w1,w2);}

    void init(const vertex_base& w1, const vertex_base& w2)
      {
	REQUIRE((    (w1.x() == w2.x()) && (w1.y() == w2.y()+1 || w1.y() == w2.y()-1)
		  || (w1.y() == w2.y()) && (w1.x() == w2.x()+1 || w1.x() == w2.x()-1)),
		"Edge(w1,w2): (w1,w2) = (" << w1 << ','  << w2 << ')' << "is no edge!\n",1);
	if(w1.y() == w2.y()) {
	  dir = x_dir;
	  v1_ = ( w1.x() < w2.x() ? w1 : w2);
	}
	else {
	  dir = y_dir;
	  v1_ = ( w1.y() < w2.y() ? w1 : w2);
	}
      }

    Edge(const CellIterator&, const CellOnCellIterator& nb); // somewhat obsolete.
    Edge(const Cell&,         const CellOnCellIterator& nb);
    explicit Edge(const CellOnCellIterator& nb);

    unsigned NumOfVertices() const { return 2;}
    Vertex V1() const { return Vertex(v1_,_g);}
    Vertex V2() const { 
      return Vertex((dir==x_dir 
		     ? vertex_base(v1_.x()+1,v1_.y())
		     : vertex_base(v1_.x(),  v1_.y()+1)),TheGrid());
    }
    vertex_handle v1() const { return V1().handle();}
    vertex_handle v2() const { return V2().handle();}

    Vertex V(int i) const {
      REQUIRE( (i == 0 || i == 1), "Edge::V(i): i must be 0 or 1! (i = " << i << ")\n",1); 
      return ( i == 0 ? V1() : V2()); }  

    void FlipVertex(Vertex& V) const {
      REQUIRE( (V == V1() || V == V2()), "FlipVertex(): Vertex not on Edge!\n",1);
      V = ( V == V1() ? V2() : V1());
    }
    Vertex FlippedVertex(const Vertex& V) const {
      REQUIRE( (V == V1() || V == V2()), "FlipVertex(): Vertex not on Edge!\n",1);
      return ( V == V1() ? V2() : V1());
    }
    VertexOnEdgeIterator FirstVertex() const; //{ return VertexOnEdgeIterator(*this);}
    VertexOnEdgeIterator EndVertex()   const; //{ return VertexOnEdgeIterator(*this,2);}

    index_type index() const { return v1_;}
    index_type low_vertex_index() const { return index();}
    index_type high_vertex_index() const { return index() + (dir == x_dir ? index_type(1,0) : index_type(0,1));}

    inline void FlipCell(Cell& C) const;
    inline Cell FlippedCell(Cell& C) const;
    inline Cell C1()   const;  // lower resp. left cell
    inline Cell C2()   const;  // upper resp. right cell


    friend bool operator==(const self& lhs, const self& rhs) 
    {return (lhs.GlobalNumber() == rhs.GlobalNumber());}
    friend bool operator!=(const self& lhs, const self& rhs) 
      { return !(lhs == rhs);}
    friend bool operator<(const self& lhs, const self& rhs) 
    {return (lhs.GlobalNumber() < rhs.GlobalNumber());}

    edge_handle GlobalNumber() const { return TheGrid().edge_num(*this); }
    edge_handle handle      () const { return TheGrid().edge_num(*this); }

  private:
    direction dir;
    vertex_base v1_;
  };

  //----------------- CELL ----------------------

  class Cell : public elem_base {
  public:
    friend class RegGrid2D;
    typedef Cell self;
    typedef grid_types<Grid::archetype_type> archgt;
    typedef index_type local_index_type;

    enum side   { S  = 0, E  = 1, N  = 2, W  = 3, invalid_side   = 4};
    enum corner { SW = 0, SE = 1, NE = 2, NW = 3, invalid_corner = 4};

    Cell() : elem_base(0) {}
    Cell(const Grid& g, cell_handle c) { *this = g.cell(c); }

    Cell(const Grid* g, const vertex_base& b) : elem_base(g),  llv(b)  {}
    Cell(const Grid& g, const vertex_base& b) : elem_base(&g), llv(b)  {}


    VertexOnCellIterator FirstVertex()    const;
    VertexOnCellIterator EndVertex()      const;
    EdgeOnCellIterator   FirstEdge()      const;
    EdgeOnCellIterator   EndEdge()        const;
    EdgeOnCellIterator   FirstFacet()     const;
    EdgeOnCellIterator   EndFacet()       const;
    CellOnCellIterator   FirstCell()      const;
    CellOnCellIterator   EndCell()   const;
    CellOnCellIterator   FirstNeighbour() const;
    CellOnCellIterator   EndNeighbour()   const;

    int NumOfVertices()   const {return 4;}
    int NumOfNeighbours() const {return 4- NumOfBoundaryFacets();}
    int NumOfEdges()      const {return 4;}
    int NumOfFacets()     const {return 4;}
    int NumOfBoundaryFacets() const
      {
	int n = 0; 
	n += ((llv.x()   == TheGrid().llx()) ? 1 : 0);
	n += ((llv.y()   == TheGrid().lly()) ? 1 : 0);
	n += ((llv.x()+1 == TheGrid().urx()) ? 1 : 0);
	n += ((llv.y()+1 == TheGrid().ury()) ? 1 : 0);
	return n;
      }
    int NumOfCells() const { return 4 - NumOfBoundaryFacets();}
 
    friend bool operator==(const self& lhs, const self& rhs) 
      {return (lhs.llv == rhs.llv);} // GlobalNumber() == rhs.GlobalNumber());}
    friend bool operator!=(const self& lhs, const self& rhs) 
      { return !(lhs == rhs);}
    friend bool operator<(const self& lhs, const self& rhs) 
    {return (lhs.GlobalNumber() < rhs.GlobalNumber());}

    side   opposite(side s  ) const { return side  ((s+2)%4);}
    corner opposite(corner c) const { return corner((c+2)%4);}
    bool IsValid(side s)   const { return ((S <= s)  && (s <= W));}
    bool IsValid(corner c) const { return ((SW <= c) && (c <= NW));}

    cell_handle GlobalNumber()    const { return TheGrid().cell_num(llv);}
    cell_handle handle      ()    const { return TheGrid().cell_num(llv);}
    Vertex V(corner i) const { return vertex(i);}
    Vertex V(int i)    const { return vertex(corner(i));}
    Vertex V       (archgt::Vertex const& lV) const { return V(lV.handle());}
    vertex_handle v(archgt::Vertex const& lV) const { return V(lV).handle();}

    /// information for these calculations could go into
    /// some static tables of RegGrid2D
    Vertex vertex(corner v) const
    {
      REQUIRE(((0<=v) && (v<4)),"Cartesian2D::Cell::vertex(v) : v = " << (int)v , 1);
      return Vertex(vertex_base(llv.x() + TheGrid().corner_offset((int)v).x(),
				llv.y() + TheGrid().corner_offset((int)v).y()),
				TheGrid());
    }

    vertex_base ll() const { return llv;}
    index_type  index() const { return llv;}
    
    Edge edge(side e) const 
      {
	REQUIRE(((S<=e) && (e<=W)),"Cartesian2D::Cell::edge(e) : e = " << (int)e , 1);
	return ( e == S ? Edge(Edge::x_dir,llv,TheGrid())
		 : e == E ? Edge(Edge::y_dir,vertex_base(llv.x()+1,llv.y()),TheGrid())
		 : e == N ? Edge(Edge::x_dir,vertex_base(llv.x(),  llv.y()+1),TheGrid())
		 :          Edge(Edge::y_dir,llv,TheGrid()));
      } 
    
    Cell neighbour(side nb) const {
	REQUIRE(((S<=nb) && (nb<=W)),"Cartesian2D::Cell::neighbour(nb) : nb = " << (int) nb , 1);
	vertex_base bv = get_nb_base(nb);
	REQUIRE(TheGrid().IsValidCellBase(bv),
		"Cell::Neighbour(nb) : nb = " << (int)nb << " not in grid!\n"
		<< " llv = " << llv << " bv = " << bv << '\n',1);
	return Cell(TheGrid(),bv); 
    }
    Cell Nb(side nb) const { return neighbour(nb);}

    bool IsOnBoundary(side nb) const { return TheGrid().IsOnBoundary(edge(nb));}
    bool IsOnBoundary(int nb) const  { return IsOnBoundary(side(nb));}

    void FlipEdge(const Vertex& v, Edge& e) const;   
  protected:
    //    cell_base _b;
    vertex_base get_nb_base(side nb) const { // no check
      return vertex_base(llv.x() + TheGrid().side_offset((int)nb).x(),
			 llv.y() + TheGrid().side_offset((int)nb).y());
    }

    /// DATA 
    vertex_base llv; /// lower left vertex
  };
  //@}
  

  //-----------------------------------------------------------
  //
  //           ITERATORS
  //
  //-----------------------------------------------------------
 

  //-----------------------------------------------------------
  //                                      
  //        SEQUENCE ITERATORS             
  //                                       
  //-----------------------------------------------------------
 
  /*! @name sequence iterators */
  //@{ 
  class seq_iterator_base {
  public:
    typedef RegGrid2D grid_type;
    typedef RegGrid2D anchor_type;

    typedef seq_iterator_base base;
    seq_iterator_base()                   : _g(0) {}
    seq_iterator_base(const grid_type* g) : _g(g) {}
    seq_iterator_base(const grid_type& g) : _g(&g) {}

    grid_type const& TheGrid()   const { cb(); return (*_g); }
    grid_type const& TheAnchor() const { return TheGrid();}

    bool bound() const { return _g != 0;}
    void cb()    const { REQUIRE(bound(), "", 1);} 
  protected:
    const grid_type* _g;
  };

  //------------------- VERTEX ITERATOR ----------------------
   
  class VertexIterator : public seq_iterator_base {
    typedef VertexIterator self;
  public:
    VertexIterator() : base(0) {} 
    VertexIterator(const Grid* g) : base(g), v(g->MinVertexNum()) {}
    explicit 
    VertexIterator(const Grid& g) : base(g), v(g .MinVertexNum()) {}
    VertexIterator(const Grid& g, vertex_handle vv) : base(&g), v(vv) {}
    VertexIterator(vertex_handle vv, const Grid* g) : base(g), v(vv) {}
    self& operator++() {
      ++v;
      return (*this);
    }
    self  operator++(int)    { cv(); self tmp(*this); ++(*this); return tmp;}
    self& operator +=(const index_type& ij) {
      cv(); v+= TheGrid().TheVertexMap().offset(ij);
      return *this;
    }

    Vertex   operator*() const { cv(); return TheGrid().vertex(v);} 
    // random access
    /* Vertex operator()(int i, int j) const { 
      // vertex_handle + int is not defined!
      return TheGrid().vertex(v + TheGrid().TheVertexMap().offset(i,j));
    }
    */
    bool     IsDone()    const { cb();  return  (v > TheGrid().MaxVertexNum());}
    // operator bool() const { return !IsDone();} 
    vertex_handle GlobalNumber() const { cv(); return v;}
    vertex_handle handle      () const { cv(); return v;}

    friend bool operator==(self const& lhs, self const& rhs)
    {
      lhs.cb(); rhs.cb();
      REQUIRE((&(lhs.TheGrid()) == &(rhs.TheGrid())), "Comparing vertices with different grids!\n",1);
      return (lhs.v == rhs.v);
    }
    friend bool operator!=(const self& lhs, const self& rhs) 
      { return !(lhs == rhs);}

    bool valid() const { return bound() && TheGrid().valid_handle(v);}
    void cv()    const { REQUIRE(valid(), "handle= " << v, 1);}
  private:
    vertex_handle v;
  };


  //-------------------- EDGE  ITERATOR ----------------------

  class EdgeIterator : public seq_iterator_base {
    typedef EdgeIterator self;
  public:
    EdgeIterator() {}
    explicit
    EdgeIterator(Grid const& g) : base(g), e(g.MinEdgeNum()) {}
    EdgeIterator(int ee,const Grid* g) 
      : base(g), e(ee)  {}

    self& operator++() { cv(); ++e; return(*this);}
    Edge operator*() const { cv(); return TheGrid().get_edge(e);}
    bool IsDone()    const { cb(); return ( e >= TheGrid().NumOfEdges());}

    //  operator Edge()  const { return (this->operator*());}
    //  operator bool() const { return !IsDone();} 
    //  edge_handle GlobalNumber() const { return e;}
    edge_handle handle      () const { return e;}

    friend bool operator==(self const& lhs, self const& rhs)
    {
      lhs.cb(); rhs.cb(); 
      REQUIRE((&(lhs.TheGrid()) == &(rhs.TheGrid())), "Comparing edges with different grids!\n",1);
      return (lhs.e == rhs.e);
    }
    friend bool operator!=(const self& lhs, const self& rhs) 
      { return !(lhs == rhs);}

    bool valid() const { return bound() && TheGrid().valid_handle(e);}
    void cv()    const { REQUIRE(valid(), "handle= " << e, 1);}
  private:
    edge_handle e;
  };


  //-------------------- CELL  ITERATOR ----------------------

  class CellIterator : public seq_iterator_base {
    typedef CellIterator self;
  public:  
    CellIterator() : base(0), c(-1) {}
    explicit
    CellIterator(const Grid& g) : base(&g), c(g.MinCellNum()) {}
    CellIterator(int cc, const Grid* g) : base(g), c(cc) {} 
    CellIterator(const Grid& g, int cc) : base(&g), c(cc) {} 
 
    self& operator++() { cv(); ++c; return (*this);}
    Cell  operator*() const { cv(); return TheGrid().cell(c);}
    // operator Cell()   const { return TheGrid().cell(c);}
    bool  IsDone()    const { cb(); return (c > TheGrid().MaxCellNum());}
    // operator bool() const { return !IsDone();} 
    // cell_handle GlobalNumber() const { return c;}
    cell_handle handle      () const { cv(); return c;}

    friend bool operator==(self const& lhs, self const& rhs)
    {
      lhs.cb(); rhs.cb();
      REQUIRE((&(lhs.TheGrid()) == &(rhs.TheGrid())), "Comparing cells with different grids!\n",1);
      return (lhs.c == rhs.c);
    }
    friend bool operator!=(const self& lhs, const self& rhs) 
      { return !(lhs == rhs);}

    bool valid() const { return bound() && TheGrid().valid_handle(c);}
    void cv()    const { REQUIRE(valid(), "handle= " << c, 1);}
  private:
    cell_handle c;
  };
  //@}


  ///////////////////////////////////////////
  //                                       //
  //       INCIDENCE ITERATORS             //
  //                                       //
  ///////////////////////////////////////////
  
  /*! @name incidence iterators */
  //@{ 
  class inc_iterator_base {
  public:
    typedef RegGrid2D grid_type;
  
    typedef inc_iterator_base base;
    inc_iterator_base() : _g(0) {}
    inc_iterator_base(const grid_type* g) : _g(g) {}
    inc_iterator_base(const grid_type& g) : _g(&g) {}

    grid_type const& TheGrid() const { cb(); return *_g; }

    bool bound() const { return _g != 0;}
    void cb()    const { REQUIRE(bound(), "", 1);} 
  protected:
    const grid_type* _g;
  };


  //-------------------- VERTEX ON VERTEX  ITERATOR ----------------------

  class VertexOnVertexIterator : public inc_iterator_base {
    typedef VertexOnVertexIterator self;
  public:
    typedef Vertex anchor_type;
    typedef Vertex value_type;
    typedef Vertex element_type;

    VertexOnVertexIterator() {}
    explicit
    VertexOnVertexIterator(Vertex const& vv) { *this = vv.FirstVertex(); }
    VertexOnVertexIterator(int v, const Vertex& vv, const Grid* g) 
      : base(g), _v(v), _vv(vv)  {}
    self& operator++() { cv(); _v = _vv.next_neighbour(_v); return (*this);}
    self  operator++(int)    { cv(); self tmp(*this); ++(*this); return tmp;}
    Vertex  operator*() const { cv(); return (_vv.vertex(_v));}
    // operator Vertex()   const { return this->operator*();}
    bool    IsDone()    const { cb(); return (_v >= 4);}
    // operator bool() const { return !IsDone();} 

    Vertex const& TheVertex()  const { cb(); return _vv;}
    Vertex const& TheAnchor()  const { cb(); return _vv;}

    int LocalNumber()   const { cv(); return _v;}

    friend bool operator==(self const& lhs, self const& rhs)
    {
      lhs.cb(); rhs.cb();
      REQUIRE((lhs.TheVertex() == rhs.TheVertex()), 
              "Comparing VertexOnVertexIterator with different vertex anchors!\n",1);
      return (lhs._v == rhs._v);
    }
    friend bool operator!=(const self& lhs, const self& rhs) { return !(lhs == rhs);}

    bool valid() const { return bound() && ! IsDone();}
    void cv()    const { REQUIRE(valid(), "vv=" << _vv.index() << " v=" << _v, 1);}
  private:
    int _v; // [0 , _c.NumOfVertices()[
    Vertex _vv;
  };

  //-------------------- CELL ON VERTEX  ITERATOR ----------------------

  class CellOnVertexIterator : public inc_iterator_base {
    typedef CellOnVertexIterator self;
  public:
    typedef Vertex anchor_type;
    typedef Cell   value_type;
    typedef Cell   element_type;

    CellOnVertexIterator() {}
    explicit
    CellOnVertexIterator(Vertex const& vv) { *this = vv.FirstCell();}
    CellOnVertexIterator(int c, const Vertex& vv, const Grid* g) 
      :  base(g), _c(c), _vv(vv) {}
    self& operator++() {cv(); _c = _vv.next_cell(_c); return (*this);}
    self  operator++(int)    { cv(); self tmp(*this); ++(*this); return tmp;}
    Cell operator*() const { cv(); return (_vv.cell(_c));}
    bool    IsDone()    const { cb(); return (_c >= 4);}
    // operator bool() const { return !IsDone();} 
    int LocalNumber()   const { cv(); return _c;}

    Vertex const& TheVertex()  const { cb(); return _vv;}
    Vertex const& TheAnchor()  const { cb(); return _vv;}

    friend bool operator==(self const& lhs, self const& rhs)
    {
      lhs.cb(); rhs.cb();
      REQUIRE((lhs.TheVertex() == rhs.TheVertex()), 
              "Comparing CellOnVertexIterator with different vertex anchors!\n",1);
      return (lhs._c == rhs._c);
    }
    friend bool operator!=(const self& lhs, const self& rhs)  { return !(lhs == rhs);}

    bool valid() const { return bound() && ! IsDone();}
    void cv()    const { REQUIRE(valid(), "vv=" << _vv.index() << " c=" << _c, 1);}
  private:
    int _c; // [0 , 4[
    Vertex _vv;
  };


  //-------------------- VERTEX ON CELL ITERATOR ----------------------

  class VertexOnCellIterator : public inc_iterator_base {
    typedef VertexOnCellIterator self;
  public:
    typedef Cell   anchor_type;
    typedef Vertex value_type;
    typedef Vertex element_type;

    VertexOnCellIterator() {}
    explicit
    VertexOnCellIterator(const Cell& cc)
      : base(& cc.TheGrid()), v(Cell::SW), c(cc)  {}
    VertexOnCellIterator( Cell::corner vv, const Cell& cc, const Grid* g) 
      : base(g), v(vv), c(cc)  {}
    self& operator++() { cv(); ++((int&)v); return (*this);}
    self  operator++(int)    { cv(); self tmp(*this); ++(*this); return tmp;}
    Vertex  operator*() const { cv(); return (c.vertex(v));}
    // operator Vertex()   const { return this->operator*();}
    bool    IsDone()    const { cb(); return (v >= c.NumOfVertices());}
    // operator bool() const { return !IsDone();} 
    int LocalNumber()   const { cv(); return v;}

    vertex_handle   handle() const { cv(); return c.vertex(v).handle();}

    self CyclicSucc() const { cv(); self tmp(*this); tmp.v = Cell::corner(tmp.v == 3 ? 0 : tmp.v+1); return tmp;}
    self CyclicPred() const { cv(); self tmp(*this); tmp.v = Cell::corner(tmp.v == 0 ? 3 : tmp.v-1); return tmp;}

    Cell const& TheCell()     const { cb(); return c;}
    Cell const& TheAnchor()   const { cb(); return c;}

    friend
    bool operator==(self const& lhs, self const& rhs)
    { 
      lhs.cb(); rhs.cb();
      REQUIRE( (lhs.c == rhs.c), "Comparing VertexOnCellIterators with different anchor cells!\n",1);
      return ((int) lhs.v == (int) rhs.v);
    }
    friend bool operator!=(const self& lhs, const self& rhs) { return !(lhs == rhs);}

    bool valid() const { return bound() && ! IsDone();}
    void cv()    const { REQUIRE(valid(), "c=" << c.index() << " v=" << v, 1);}
  private:
    Cell::corner v; 
    Cell c;
  };

  //-------------------- EDGE ON CELL ITERATOR ----------------------

  class EdgeOnCellIterator : public inc_iterator_base {
    typedef EdgeOnCellIterator self;
    friend class CellOnCellIterator;
    friend class RegGrid2D;

  public:
    typedef Edge value_type;
    typedef Edge element_type;
    typedef Cell anchor_type;

    EdgeOnCellIterator() {}
    explicit
    EdgeOnCellIterator(Cell const& cc) { *this = cc.FirstEdge();}
    EdgeOnCellIterator( Cell::side ee, const Cell& cc, const Grid* g)
      :  base(g), e(ee), c(cc) {}
    self& operator++() { ++((int&)e); return (*this);}
    self  operator++(int)    { self tmp(*this); ++(*this); return tmp;}
    Edge  operator*() const { return (TheCell().edge(e));}
    // FIXME: could be more efficient.
    edge_handle handle() const { return TheCell().edge(e).handle();}
    bool    IsDone()    const { return (e >= c.NumOfEdges());}
    operator bool() const { return !IsDone();} 

    int  LocalNumber()  const { return e;}

    Cell    OtherCell() const { return TheCell().Nb(e);}
    Vertex V1() const { return c.V(e);}
    Vertex V2() const { return c.V((e+1)%4);}

    self CyclicSucc() const 
    { self tmp(*this); tmp.e = Cell::side(tmp.e == 3 ? 0 : tmp.e+1); return tmp;}
    self CyclicPred() const 
    { self tmp(*this); tmp.e = Cell::side(tmp.e == 0 ? 3 : tmp.e-1); return tmp;}

    Cell const&  TheCell()     const { cb(); return c;}
    Cell const&  TheAnchor()   const { cb(); return c;}

    friend
    bool operator==(self const& lhs, self const& rhs)
    {     
      lhs.cb(); rhs.cb();
      REQUIRE( (lhs.c == rhs.c), "Comparing EdgeOnCellIterators with different anchor cells!\n",1);
      return ((int) lhs.e == (int) rhs.e);
    }
    friend bool operator!=(const self& lhs, const self& rhs)  { return !(lhs == rhs);}
   
    bool valid() const { return bound() && ! IsDone();}
    void cv()    const { REQUIRE(valid(), "c=" << c.index() << " e=" << e, 1);}

  private:
    Cell::side e;  
    Cell c;
  };


  //-------------------- CELL ON CELL ITERATOR ----------------------

  class CellOnCellIterator : public inc_iterator_base {
    friend class RegGrid2D;
    typedef CellOnCellIterator self;

  public:
    typedef Cell anchor_type;
    typedef Cell element_type;
    typedef Cell value_type;
    
    CellOnCellIterator() {}
    explicit
    CellOnCellIterator(Cell const& cc) { *this = cc.FirstCell();}
    CellOnCellIterator(const EdgeOnCellIterator& F) : base(F), nb(F.e), c(F.c) 
      {
	while( ! IsDone() && c.IsOnBoundary(nb))
	  ++(*this);
      }
    CellOnCellIterator( Cell::side nnb, const Cell& cc, const Grid* g) 
      : base(g), nb(nnb), c(cc) 
      {
	while( ! IsDone() && c.IsOnBoundary(nb))
	  ++(*this);
      }
    self& operator++() { 
      do {
	++((int&)nb);
      } while( ! IsDone() && c.IsOnBoundary(nb));
      return (*this);
    }
    self  operator++(int)    { self tmp(*this); ++(*this); return tmp;}
    Cell  operator*() const  { return TheCell().neighbour(nb);}
    cell_handle handle() const { return (operator*()).handle();}

    // operator Cell()   const { return this->operator*();}
    Edge facet()      const { return c.edge(LocalNumber());}
    bool  IsDone()    const { return (nb >= 4);}
    // operator bool() const { return !IsDone();} 
    Cell::side LocalNumber()  const { return nb;}

    Cell const&  TheCell()   const { return c;}
    Cell const&  TheAnchor() const { return c;}

    friend bool operator==(self const& lhs, self const& rhs)  { 
      lhs.cb(); rhs.cb();
      REQUIRE( (lhs.c == rhs.c), "Comparing CellOnCellIterators with different anchor cells!\n",1);
      return ((int) lhs.nb == (int) rhs.nb);
    }
    friend bool operator!=(const self& lhs, const self& rhs)    { return !(lhs == rhs);}
    
    bool valid() const { return bound() && ! IsDone();}
    void cv()    const { REQUIRE(valid(), "c=" << c.index() << " nb=" << nb, 1);}
  private:
    Cell::side nb;  // [0 , 4[
    Cell c;
  };
  typedef CellOnCellIterator CellNeighbourIterator;
  //@}

  //----------------------------------------------------------------------- 
  //---------------  Operations of RegGrid2D  ----------------------------- 
  //----------------------------------------------------------------------- 

  /*! @name Size operations */
  //@{ 

  int NumOfVertices() const { return TheVertexMap().range_size();}
  int NumOfEdges()    const { return (NumOfXEdges() + NumOfYEdges());}
  int NumOfFacets()   const { return NumOfEdges();}
  int NumOfFaces()    const { return NumOfCells();}
  int NumOfCells()    const { return TheCellMap().range_size();}

  int NumOfXEdges() const { return TheXEdgeMap().range_size();}
  int NumOfYEdges() const { return TheYEdgeMap().range_size();}
  int NumOfHorizontalEdges() const { return NumOfXEdges();}
  int NumOfVerticalEdges()   const { return NumOfYEdges();}


  int NumOfBoundaryVertices() const { return (2*(xpoints   + ypoints) -4);}
  int NumOfBoundaryEdges()    const { return (2*(xpoints-1 + ypoints-1));}
  int NumOfBoundaryFacets()   const { return NumOfBoundaryEdges();}
  int NumOfBoundaryFaces()    const { return NumOfBoundaryCells();}
  int NumOfBoundaryCells()    const { return (2*(xpoints-1 + ypoints-1) -4);}
  //@}

  int Offset(const Vertex&) const {return TheVertexMap().n0();}
  int Offset(const Edge&  ) const {return TheXEdgeMap().n0();}
  int Offset(const Cell&  ) const {return TheCellMap().n0();}


  int NumXpoints() const { return xpoints;}
  int NumYpoints() const { return ypoints;}
  
  /*! @name Sequence iteration */
  //@{ 
  VertexIterator FirstVertex() const { return VertexIterator(MinVertexNum(),this);}
  EdgeIterator   FirstEdge()   const { return EdgeIterator(MinEdgeNum(),this);}
  EdgeIterator   FirstFacet()  const { return EdgeIterator(MinEdgeNum(),this);}
  CellIterator   FirstFace()   const { return FirstCell();}
  CellIterator   FirstCell()   const { return CellIterator(MinCellNum(),this);}
  VertexIterator EndVertex()   const { return VertexIterator(MaxVertexNum()+1,this);}
  EdgeIterator   EndEdge()     const { return EdgeIterator(MaxEdgeNum()+1,this);}
  EdgeIterator   EndFacet()    const { return EdgeIterator(MaxEdgeNum()+1,this);}
  CellIterator   EndFace()     const { return EndCell();}
  CellIterator   EndCell()     const { return CellIterator(MaxCellNum()+1,this);}
  //@}

  /*! @name Element validity tests */
  //@{ 
  inline bool IsValid(const vertex_base& v) const { return TheVertexMap().IsInRange(v);}
  inline bool IsValidCellBase(const vertex_base& v) const { 
    return TheCellMap().IsInRange(v);
  }
  bool IsValid(const Cell& C)     const;
  //@}

  /*! @name Boundary tests */
  //@{ 
  bool IsOnBoundary(const Vertex& V) const { return IsOnBoundary(V.index());}
  inline bool IsOnBoundary(const vertex_base&) const;
  inline bool IsOnBoundary(const Edge&   E) const;
  inline bool IsOnBoundary(const EdgeOnCellIterator& Nb) const;

  bool IsInside(const CellIterator& C) const;
  bool IsInside(const Cell& C)    const;
  //@}


  friend class SubrangeReg2D;
  friend class Vertex;
  friend class Edge;
  friend class Cell;

  friend class VertexIterator;
  friend class EdgeIterator;
  friend class CellIterator;
  friend class VertexOnVertexIterator;
  friend class VertexOnCellIterator;
  friend class EdgeOnCellIterator;
  friend class CellOnCellIterator;

  /*! @name element to handle mappings */
  //@{ 
  vertex_handle handle(const Vertex& V) const { return vertex_num(V.x(),V.y());} 
  vertex_handle handle(const VertexIterator& V) const { return V.GlobalNumber();}
  vertex_handle handle(const VertexOnCellIterator& V) const { return handle(*V);}
  edge_handle   handle(const Edge& E)   const { return edge_num(E);}
  cell_handle   handle(const Cell& C)   const { return cell_num(C);}

  // vertex_handle  <-> Vertex
  vertex_handle get_vertex_handle(index_type I) const { return vertex_num(I);}
  index_type    get_index        (vertex_handle h) const { return get_vertex(h);}

  vertex_handle vertex_num(int x, int y)     const { return TheVertexMap().number(x,y);}
  vertex_handle vertex_num(const vertex_base& V) const { return vertex_num(V.x(),V.y());} 
  vertex_handle vertex_num(const Vertex& V) const { return vertex_num(V.x(),V.y());}
  vertex_base get_vertex(vertex_handle v) const { return TheVertexMap().index(v);}
  Vertex vertex(vertex_handle v) const { return Vertex(get_vertex(v),this);}
  Vertex vertex(const vertex_base& v) const { return Vertex(v,this);}
  Vertex vertex(int x, int y) const { return vertex(vertex_base(x,y));}

  // edge_handle <-> Edge
  inline edge_handle edge_num(const Edge& E) const;
  inline Edge get_edge(edge_handle e)   const;
  Edge   edge(edge_handle e)   const { return get_edge(e);}
  Edge   xedge(vertex_base leftv)   const { return Edge(Edge::x_dir,leftv,  *this);}
  Edge   yedge(vertex_base bottomv) const { return Edge(Edge::y_dir,bottomv,*this);}

  // cell_handle <-> Cell
  // (x,y) == lower left corner == int-coordinates of box center
  cell_handle get_cell_handle(index_type  I) const { return cell_num(I);}
  index_type  get_index      (cell_handle h) const { return get_cell_llv(h);}

  cell_handle cell_num(int llx, int lly) const { return TheCellMap().number(llx,lly);}
  cell_handle cell_num(const vertex_base& llv) const {return cell_num(llv.x(),llv.y());}
  cell_handle cell_num(const Cell& C) const { return cell_num(C.ll());}
  vertex_base   get_cell_llv(cell_handle c)   const { return TheCellMap().index(c);}
  Cell   cell(cell_handle c)    const { return Cell(this,get_cell_llv(c));}
  Cell   cell(vertex_base llv)  const { return Cell(this,llv);}
  Cell   cell(int llx, int lly) const { return Cell(this,vertex_base(llx,lly));}
  //@}
 
  /*! @name intervals for element handles */
  //@{ 
  vertex_handle MinVertexNum() const { return  TheVertexMap().n0();}
  vertex_handle MaxVertexNum() const { return  TheVertexMap().nmax();}
  vertex_handle MinNum(const Vertex&) const { return MinVertexNum();}
  vertex_handle MaxNum(const Vertex&) const { return MaxVertexNum();}

  edge_handle MinEdgeNum() const { return TheXEdgeMap().n0();}
  edge_handle MaxEdgeNum() const { return MinEdgeNum() + NumOfEdges() -1;}
  edge_handle MinNum(const Edge&) const { return MinEdgeNum();}
  edge_handle MaxNum(const Edge&) const { return MaxEdgeNum();}

  cell_handle MinCellNum() const { return TheCellMap().n0();}
  cell_handle MaxCellNum() const { return TheCellMap().nmax();}
  cell_handle MinNum(const Cell&) const { return MinCellNum();}
  cell_handle MaxNum(const Cell&) const { return MaxCellNum();}
  //@}

  /*! @name switch operator 
   */
  //@{
  inline void switch_vertex(Vertex& v, Edge const& e) const 
    { e.FlipVertex(v);}
  inline void switch_edge(Vertex const& v, Edge & e, Cell const& c) const
    { c.FlipEdge(v,e);}  
  inline void switch_facet(Vertex const& v, Edge & e, Cell const& c) const
    { switch_edge(v,e,c);}
  inline void switch_cell(Edge const& e, Cell & c) const
    { e.FlipCell(c);}
  inline Vertex switched_vertex(Vertex const& v, Edge const& e) const
    { Vertex sv(v); switch_vertex(sv,e); return sv;}
  inline Edge switched_edge(Vertex const& v, Edge const& e, Cell const& c) const
    { Edge se(e); switch_edge(v,se,c); return se;}
  inline Edge switched_facet(Vertex const& v, Edge const& e, Cell const& c) const
    { Edge se(e); switch_edge(v,se,c); return se;}
  inline Cell switched_cell(Edge const& e, Cell const& c) const
    { Cell sc(c); switch_cell(e,sc); return sc;}
  //@}

  /*! \name Archetype handling
   */
  /*@{*/ 
  static archetype_iterator BeginArchetype() { return sd.the_archetype; }
  static archetype_iterator EndArchetype()   { return sd.the_archetype+1; }
  static archetype_handle   handle(archetype_iterator it)  { return it - BeginArchetype();}
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

};



class SubrangeReg2D;


struct grid_types_cart2d {

  typedef cartesian2d::RegGrid2D  Grid;
  typedef cartesian2d::RegGrid2D  grid_type;
  typedef Grid::index_type        index_type;
  typedef Grid::vertex_index_type vertex_index_type;
  typedef Grid::cell_index_type   cell_index_type;

  typedef  Grid::Vertex Vertex;
  typedef  Grid::Edge   Edge;
  typedef  Grid::Edge   Facet;
  typedef  Grid::Cell   Cell;
  typedef  Grid::Cell   Face;


  typedef  Grid::vertex_handle vertex_handle;
  typedef  Grid::edge_handle   edge_handle;
  typedef  Grid::edge_handle   facet_handle;
  typedef  Grid::cell_handle   cell_handle;

  typedef  Grid::VertexIterator  VertexIterator;
  typedef  Grid::EdgeIterator    EdgeIterator;
  typedef  Grid::EdgeIterator    FacetIterator;
  typedef  Grid::CellIterator    CellIterator;

  typedef  Grid::VertexOnVertexIterator   VertexOnVertexIterator;
  typedef  Grid::VertexOnVertexIterator   VertexNeighbourIterator;
  typedef  Grid::CellOnVertexIterator     CellOnVertexIterator;
  typedef  vertex_on_edge_iterator<Grid>  VertexOnEdgeIterator;
  typedef  VertexOnEdgeIterator           VertexOnFacetIterator;

  typedef  Grid::VertexOnCellIterator   VertexOnCellIterator;
  typedef  Grid::EdgeOnCellIterator     EdgeOnCellIterator;
  typedef  Grid::EdgeOnCellIterator     FacetOnCellIterator;
  typedef  Grid::CellOnCellIterator     CellOnCellIterator;
  typedef  Grid::CellOnCellIterator     NeighbourCellIterator;
  typedef  Grid::CellOnCellIterator     CellNeighbourIterator;

  // 2D: Cell <-> Face
  typedef cell_handle             face_handle;
  typedef Cell                    Face;
  typedef CellIterator            FaceIterator;
  typedef VertexOnCellIterator    VertexOnFaceIterator;
  typedef CellOnVertexIterator    FaceOnVertexIterator;
  typedef EdgeOnCellIterator      EdgeOnFaceIterator;
  //  typedef CellOnEdgeIterator      FaceOnEdgeIterator;


  typedef grid_dim_tag<2>   dimension_tag;
  static int dimension(const Cell& ) { return 2;}
  static int dimension(const Facet&) { return 1;} 
  static int dimension(const Vertex&) { return 0;}

  struct hash_facet {
    int operator()(const Facet& F) const { return F.GlobalNumber();}
  };

  static cell_handle invalid_cell_handle(grid_type const& G) { return G.invalid_cell();}
  static cell_handle outer_cell_handle  (grid_type const& G) { return G.outer_cell_handle();}
  static bool is_cell_valid (grid_type const& G, cell_handle c) { return (c !=G.invalid_cell());}
  static bool is_cell_inside(grid_type const& G, cell_handle c) { return (c !=G.invalid_cell());}
  static bool is_cell_valid (grid_type const& G, Cell const& c) { return (G.IsValid(c));}
  static bool is_cell_inside(grid_type const& G, Cell const& c) { return (G.IsInside(c));}

  typedef grid_type::archetype_type     archetype_type;
  typedef grid_type::archetype_handle   archetype_handle;
  typedef grid_type::archetype_iterator archetype_iterator;
  typedef grid_types<archetype_type>    archgt;

  typedef cartesian2d::SubrangeReg2D    cartesian_subrange_type;

};

} // namespace cartesian2d




/*! \brief specialization of grid_types template for RegGrid2D
 */
struct grid_types<cartesian2d::RegGrid2D> : public grid_types_base<cartesian2d::grid_types_cart2d> 
{ };



#include "Gral/Grids/Cartesian2D/element-traits.h"

namespace cartesian2d {

////////////////////////
//
//  methods of RegGrid2D::Vertex
//
////////////////////////

inline  
RegGrid2D::VertexOnVertexIterator 
RegGrid2D::Vertex::FirstVertex() const {
  if( y() > TheGrid().lly()) // not on lower boundary 
    return VertexOnVertexIterator(0,*this,&TheGrid()); // south nb exists
  else if( x() < TheGrid().urx()) //  not on lower right corner
    return VertexOnVertexIterator(1,*this,&TheGrid()); // east nb exists
  else if(y() < TheGrid().ury()) 
    return VertexOnVertexIterator(2,*this,&TheGrid()); // north nb exists
  else if(x() > TheGrid().llx())
    return VertexOnVertexIterator(3,*this,&TheGrid()); // west nb exists
  return VertexOnVertexIterator(4,*this,&TheGrid()); // no nb exists
}

inline
RegGrid2D::VertexOnVertexIterator 
RegGrid2D::Vertex::EndVertex() const { return VertexOnVertexIterator(5,*this, &TheGrid());}

inline 
RegGrid2D::VertexOnVertexIterator 
RegGrid2D::Vertex::FirstNeighbour() const 
{ return FirstVertex();}
inline 
RegGrid2D::VertexOnVertexIterator 
RegGrid2D::Vertex::EndNeighbour() const 
{ return EndVertex();}

inline 
RegGrid2D::CellOnVertexIterator 
RegGrid2D::Vertex::FirstCell() const 
{
  int c = -1;
  c = next_cell(c);
  return CellOnVertexIterator(c,*this,&TheGrid());
}

inline 
RegGrid2D::CellOnVertexIterator 
RegGrid2D::Vertex::EndCell() const { return CellOnVertexIterator(4,*this,&TheGrid());}


inline 
RegGrid2D::Cell 
RegGrid2D::Vertex::cell(int c) const 
{ return Cell(TheGrid(),get_cell_base(c));}



////////////////////////
//
// methods of  RegGrid2D::Edge
//
////////////////////////

inline 
RegGrid2D::Edge::Edge(const RegGrid2D::CellIterator&, 
		      const RegGrid2D::CellOnCellIterator& nb)
{ *this = nb.facet(); }

inline 
RegGrid2D::Edge::Edge(const RegGrid2D::CellOnCellIterator& nb)
{ *this = nb.facet(); }

inline 
RegGrid2D::Edge::Edge(const RegGrid2D::Cell&, const RegGrid2D::CellOnCellIterator& nb)
{ *this = nb.facet(); }


inline
RegGrid2D::VertexOnEdgeIterator RegGrid2D::Edge::FirstVertex() const { return VertexOnEdgeIterator(*this);}
    
inline
RegGrid2D::VertexOnEdgeIterator RegGrid2D::Edge::EndVertex()   const { return VertexOnEdgeIterator(*this,2);}

inline 
 RegGrid2D::Cell 
RegGrid2D::Edge::C1()   const  { // lower resp. left cell
  /*
  REQUIRE((dir == x_dir 
	   ?TheGrid().TheCellMap().IsInRange(index_type(v1.x(),  v1.y()-1))
	   :TheGrid().TheCellMap().IsInRange(index_type(v1.x()-1,v1.y()  ))),
	  "Edge::C1() : Edge on Boundary",1);
	  */
  return (dir == x_dir 
	  ? Cell(TheGrid(),vertex_base(v1_.x(),  v1_.y()-1))
	  : Cell(TheGrid(),vertex_base(v1_.x()-1,v1_.y()  )));
}    

inline  
 RegGrid2D::Cell 
RegGrid2D::Edge::C2()   const  {  // upper resp. right cell
  // REQUIRE(TheGrid().TheCellMap().IsInRange(v1),
  //	  "Edge::C1() : Edge on Boundary",1);
  return Cell(TheGrid(),v1_); 
}

inline  
void 
RegGrid2D::Edge::FlipCell(RegGrid2D::Cell& C)   const  
{ 
  REQUIRE( (C == C1() || C == C2()), "FlipCell(): Cell not on Edge!\n",1);
  C = ( C == C1() ? C2() : C1());
}


////////////////////////
//
//  methods of RegGrid2D::Cell
//
////////////////////////

inline 
RegGrid2D::VertexOnCellIterator 
RegGrid2D::Cell::FirstVertex()  const 
{ return VertexOnCellIterator(SW,*this,_g);}

inline 
RegGrid2D::VertexOnCellIterator 
RegGrid2D::Cell::EndVertex()  const 
{ return VertexOnCellIterator(invalid_corner,*this,_g);}

inline 
RegGrid2D::EdgeOnCellIterator   
RegGrid2D::Cell::FirstEdge()  const 
{ return EdgeOnCellIterator(S,*this,_g);}

inline 
RegGrid2D::EdgeOnCellIterator   
RegGrid2D::Cell::EndEdge()  const 
{ return EdgeOnCellIterator(invalid_side,*this,_g);}

inline 
RegGrid2D::EdgeOnCellIterator   
RegGrid2D::Cell::FirstFacet()  const 
{ return EdgeOnCellIterator(S,*this,_g);}

inline 
RegGrid2D::EdgeOnCellIterator   
RegGrid2D::Cell::EndFacet()  const 
{ return EdgeOnCellIterator(invalid_side,*this,_g);}

inline 
RegGrid2D::CellOnCellIterator   
RegGrid2D::Cell::FirstNeighbour() const 
{ return CellOnCellIterator(S,*this,_g);}

inline 
RegGrid2D::CellOnCellIterator   
RegGrid2D::Cell::EndNeighbour() const 
{ return CellOnCellIterator(invalid_side,*this,_g);}


inline 
RegGrid2D::CellOnCellIterator   
RegGrid2D::Cell::FirstCell() const 
{ return FirstNeighbour();}

inline 
RegGrid2D::CellOnCellIterator   
RegGrid2D::Cell::EndCell() const 
{ return EndNeighbour();}

inline
void RegGrid2D::Cell::FlipEdge(const Vertex& v, Edge& e) const {
  REQUIRE(((v == e.V1()) || (v == e.V2())),
	  "FlipEdge(v,e): v not in {e.V1,e.V2} !\n",1);
  VertexOnCellIterator w =FirstVertex();
  while (( ! (*w == v)) && ( ! w.IsDone())) 
    ++w;
  REQUIRE( (*w == v), "FlipEdge(v,e): v not on cell!\n",1);
  Vertex v2 = e.FlippedVertex(v);
  e =  (*(w.CyclicSucc()) == v2 ? Edge(v,*(w.CyclicPred())) : Edge(v,*(w.CyclicSucc())));
}


////////////////////////
//
// methods of RegGrid2D
//
////////////////////////
  
inline 
RegGrid2D::Edge
RegGrid2D::get_edge(edge_handle e) const
{
  // offset <= edge < NumOfEdges() + offset ???
  REQUIRE( ((0 <= e) && (e < NumOfEdges())),
	   "RegGrid2D::get_edge(e) : e = " << e
	   << " not in [0," << NumOfEdges()-1 << "]!", 1);

  return (e < NumOfXEdges()
	  ? Edge(Edge::x_dir,TheXEdgeMap().index(e),this)
	  : Edge(Edge::y_dir,TheYEdgeMap().index(e-NumOfXEdges()),this));
}


inline 
RegGrid2D::edge_handle 
RegGrid2D::edge_num(const RegGrid2D::Edge& E) const
{
  return ( E.dir == Edge::x_dir
	   ? TheXEdgeMap().number(E.v1_)
	   : TheYEdgeMap().number(E.v1_) + NumOfXEdges());
}



inline bool RegGrid2D::IsOnBoundary(const RegGrid2D::vertex_base& V) const
{
 return (   (V.x() == llx()) || (V.x() == urx())
	 || (V.y() == lly()) || (V.y() == ury()));
}

inline  bool RegGrid2D::IsOnBoundary(const RegGrid2D::Edge&   E) const
{
  return (  (E.dir == Edge::x_dir && (E.v1_.y() == lly() || E.v1_.y() == ury()))
	  ||(E.dir == Edge::y_dir && (E.v1_.x() == llx() || E.v1_.x() == urx())));
}

inline  bool RegGrid2D::IsOnBoundary(const RegGrid2D::EdgeOnCellIterator& F) const
{ 
  vertex_base bv = F.TheCell().get_nb_base(F.e);
  return ( ! TheCellMap().IsInRange(bv.x(),bv.y()));
}


inline bool RegGrid2D::IsInside(const RegGrid2D::Cell& C) const
{
  vertex_base bv = C.ll();
  return ( TheCellMap().IsInRange(bv.x(),bv.y()));
}

inline bool RegGrid2D::IsValid(const RegGrid2D::Cell& C) const
{
  return ((C._g == this) && IsInside(C));
}




} // namespace cartesian2d 



#endif
