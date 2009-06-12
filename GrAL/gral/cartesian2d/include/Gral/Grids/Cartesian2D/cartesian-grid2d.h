#ifndef NMWR_GB_CARTESIAN_GRID_2D_H
#define NMWR_GB_CARTESIAN_GRID_2D_H



/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


/*! \file
 */

#include <string>


#include "Utility/pre-post-conditions.h"

#include "Gral/Base/common-grid-basics.h" 
#include "Gral/Base/element-handle.h"
#include "Gral/Iterators/vertex-on-edge-iterator.h"

#include "Gral/Grids/Cartesian2D/index-map.h"
#include "Gral/Base/polygon.h"

namespace GrAL {

namespace cartesian2d {

  class RegGrid2D;
  class grid_types_cart2d;



  //! Alias for consistency with Cartesian3D
  typedef RegGrid2D CartesianGrid2D;

/*! \brief A two-dimensional cartesian grid type

    \ingroup cartesian2dmodule

    RegGrid2D implements the full kernel interface.

    \see Tests in <tt> \ref test-cartesian-iterators.C </tt>

    \todo The constructors are not consistent, allow only
     constructors of the form 
     - \c  RegGrid2D(vertex_size_type nv)
     -  <tt>RegGrid2D(vertex_index_type low, vertex_index_type high) </tt>
 */ 

class RegGrid2D {
public:
  typedef RegGrid2D                    self;
  typedef cartesian_grid_category_d<2> category;
  typedef self                         grid_type;

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

  //@{ 
  /*! @name Constructors  */
  /*! \brief Grid with \c n points in each direction
      \post
         <tt> ll() == (0,0)</tt> and <tt>ur() == (n-1,n-1)</tt>
   */
  RegGrid2D(int n = 2) // default: 1 cell, 4 vertices
    :ll_(0,0), ur_(n-1,n-1)
  { init(); }
  /*! \brief Grid with \c nv_x points in x direction and \c nv_y in y-direction
      \post
         <tt> ll() == (0,0)</tt> and <tt>ur() == (nv_x1,nv_y-1)</tt>
   */
  RegGrid2D(int nv_x, int nv_y) 
    : ll_(0,0), ur_(nv_x-1,nv_y-1)
  { init(); }

  /*! \brief Grid with \c nv[0] points in x direction and \c nv[1] in y-direction
      \post
         <tt> ll() == (0,0)</tt> and <tt>ur() == nv-(1,1)</tt>
   */
  RegGrid2D(const index_type& nv)
    : ll_(0,0), ur_(nv-index_type(1,1))
  { init(); }


  /*! \brief Grid with \c urx-llx+1 points in x direction and \c ury-lly in y-direction
      \post
         <tt> ll() == (llx,lly)</tt> and <tt>ur() == (urx,ury)</tt>
   */
  RegGrid2D(int llx, int lly,int urx, int ury)
    : ll_(llx,lly), ur_(urx,ury)
  { init(); }


  /*! \brief Grid with \c UR[0]-LL[0]+1 points in x direction and \c UR[1]-LL[1] in y-direction
      \post
         <tt> ll() == LL</tt> and <tt>ur() == UR</tt>
   */
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
  //@{  
  /*! @name access to shape information
      \brief Bounds are inclusive vertex bounds
  */
  //! lower vertex bound
  const index_type& ll() const { return ll_;}
  //! upper vertex bound
  const index_type& ur() const { return ur_;}
  //! lower vertex  bound in x-direction
  int llx() const { return ll_.x();}
  //! lower vertex  bound in y-direction
  int lly() const { return ll_.y();}
  //! upper vertex  bound in x-direction
  int urx() const { return ur_.x();}
  //! upper vertex  bound in y-direction
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
  static int get_side  (const std::string& nm);
  static int get_corner(const std::string& nm);
  static std::string side_name  (int side)   { return side_name_  [side-1];} // returns "S", "N" etc.
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

  typedef GrAL::signed_size_t                     size_type;
  typedef vertex_handle_int<RegGrid2D,RegGrid2D>  vertex_handle;
  typedef edge_handle_int  <RegGrid2D,RegGrid2D>  edge_handle;
  typedef cell_handle_int  <RegGrid2D,RegGrid2D>  cell_handle;

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

  typedef vertex_on_edge_iterator<RegGrid2D,grid_types_cart2d> VertexOnEdgeIterator;
  typedef Cell Face;
  typedef Edge Facet;

  //! Combinarorial dimension
  unsigned dimension() const { return 2;}

  /*! \name Checking functions 
      \brief Check validity of handles
  */
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
    typedef  vertex_type_tag        element_type_tag;
    typedef  grid_vertex_category   category;
    typedef  VertexOnVertexIterator VertexIterator;
    typedef  VertexOnVertexIterator NeighbourIterator;

    Vertex() {}
    Vertex(Grid const& g, vertex_handle v) { *this = g.vertex(v);}
    Vertex(ref_ptr<Grid const> g, vertex_handle v) { *this = g->vertex(v);}

    Vertex(const vertex_base& v, const Grid* g) :  elem_base(g),  _v(v) {}
    Vertex(const vertex_base& v, const Grid& g) :  elem_base(&g), _v(v) {}
    Vertex(Grid const& g, int xx, int yy) : elem_base(&g), _v(xx,yy) {}
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
    bool valid() const { return  bound() && TheGrid().IsValid(_v);}
  protected:
    index_type _v;
  };

  //-------------------- EDGE ---------------------

  class Edge : public elem_base {
  public:
    typedef  edge_type_tag        element_type_tag;
    typedef  grid_edge_category   category;

    enum   direction {x_dir,y_dir};

    friend class RegGrid2D;
    typedef RegGrid2D         Grid;
    typedef Edge self;

    Edge()  {}
    Edge(Grid const& g, edge_handle e) { *this = g.edge(e);}
    Edge(ref_ptr<Grid const> g, edge_handle e) { *this = g->edge(e);}

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
	REQUIRE((    ((w1.x() == w2.x()) && (w1.y() == w2.y()+1 || w1.y() == w2.y()-1))
	          || ((w1.y() == w2.y()) && (w1.x() == w2.x()+1 || w1.x() == w2.x()-1))),
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

    bool valid() const { return  bound() && TheGrid().IsValid(low_vertex_index()) &&  TheGrid().IsValid(high_vertex_index()) ;}
 private:
    direction dir;
    vertex_base v1_;
  };

  //----------------- CELL ----------------------

  class Cell : public elem_base {
  public:
    typedef cell_type_tag      element_type_tag;
    typedef grid_cell_category category;

    friend class RegGrid2D;
    typedef Cell self;
    typedef Cell Face;
    typedef Edge Facet;
    typedef grid_types<Grid::archetype_type> archgt;
    typedef index_type local_index_type;

    enum side   { S  = 0, E  = 1, N  = 2, W  = 3, invalid_side   = 4};
    enum corner { SW = 0, SE = 1, NE = 2, NW = 3, invalid_corner = 4};

    Cell() : elem_base(0) {}
    Cell(const Grid& g, cell_handle c) { *this = g.cell(c); }
    Cell(ref_ptr<Grid const> g, cell_handle c) { *this = g->cell(c); }

    Cell(const Grid* g, const vertex_base& b) : elem_base(g),  llv(b)  {}
    Cell(const Grid& g, const vertex_base& b) : elem_base(&g), llv(b)  {}
    Cell(const Grid& g, int x, int y)         : elem_base(&g), llv(x,y) {}

    inline VertexOnCellIterator FirstVertex()    const;
    inline VertexOnCellIterator EndVertex()      const;
    inline EdgeOnCellIterator   FirstEdge()      const;
    inline EdgeOnCellIterator   EndEdge()        const;
    inline EdgeOnCellIterator   FirstFacet()     const;
    inline EdgeOnCellIterator   EndFacet()       const;
    inline CellOnCellIterator   FirstCell()      const;
    inline CellOnCellIterator   EndCell()        const;
    inline CellOnCellIterator   FirstFace()      const;
    inline CellOnCellIterator   EndFace()        const;
    inline CellOnCellIterator   FirstNeighbour() const;
    inline CellOnCellIterator   EndNeighbour()   const;

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
    int NumOfFaces() const { return NumOfCells();}


    VertexOnCellIterator  FirstElement(tp<Vertex>) const { return FirstVertex();}
    VertexOnCellIterator  EndElement  (tp<Vertex>) const { return EndVertex();}
    EdgeOnCellIterator    FirstElement(tp<Edge  >) const { return FirstEdge();}
    EdgeOnCellIterator    EndElement  (tp<Edge  >) const { return EndEdge();}
    CellOnCellIterator    FirstElement(tp<Cell  >) const { return FirstCell();}
    CellOnCellIterator    EndElement  (tp<Cell  >) const { return EndCell();}

    template<class EE> 
    typename incidence_iterator<grid_type, typename EE::element_type_tag, cell_type_tag>::type
    FirstElement() const { return FirstElement(tp<EE>());}
    template<class EE> 
    typename incidence_iterator<grid_type, typename EE::element_type_tag, cell_type_tag>::type
    EndElement() const { return EndElement(tp<EE>());}


    int NumOf(tp<Vertex>) const { return NumOfVertices();}
    int NumOf(tp<Edge  >) const { return NumOfEdges   ();}
    int NumOf(tp<Cell  >) const { return NumOfCells   ();}
    template<class EE> int NumOf() const { return NumOf(tp<EE>());}
   

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
    // FIXME: temporary for BGL!
    // operator int() const { return handle();}
    
    Vertex V(corner i) const { return vertex(i);}
    Vertex V(int i)    const { return vertex(corner(i));}
    Vertex V       (archgt::Vertex const& lV) const { return V(lV.handle());}
    vertex_handle v(archgt::Vertex const& lV) const { return V(lV).handle();}
    vertex_handle v(archgt::vertex_handle lv) const { return V(lv).handle();}
 
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

    inline void FlipEdge(const Vertex& v, Edge& e) const;   
    
    typedef grid_type::archetype_type archetype_type;
    archetype_type const& TheArchetype() const { return TheGrid().ArchetypeOf(*this);}
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
    typedef grid_vertex_iterator_category category;
    typedef Vertex value_type;
    typedef Vertex element_type;

    VertexIterator() : base(0) {} 
    VertexIterator(const Grid* g) : base(g), v(g->MinVertexNum()) {}
    explicit 
    VertexIterator(const Grid& g) : base(g), v(g .MinVertexNum()) {}
    VertexIterator(const Grid& g, vertex_handle vv) : base(&g), v(vv) {}
    //    VertexIterator(vertex_handle vv, const Grid* g) : base(g), v(vv) {}

    self& operator++() { ++v; return (*this); }
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

    static self begin(anchor_type const& a) { return self(a,0);}
    static self end  (anchor_type const& a) { return self(a,a.NumOf<value_type>());}
    static int  size (anchor_type const& a) { return a.NumOf<value_type>();}
  private:
    vertex_handle v;
  };


  //-------------------- EDGE  ITERATOR ----------------------

  class EdgeIterator : public seq_iterator_base {
    typedef EdgeIterator self;
  public:
    typedef grid_edge_iterator_category category;
    typedef Edge value_type;
    typedef Edge element_type;

    EdgeIterator() {}
    explicit
    EdgeIterator(Grid const& g) : base(g), e(g.MinEdgeNum()) {}
    EdgeIterator(Grid const& g, int ee) : base(g), e(ee) {}

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

    static self begin(anchor_type const& a) { return self(a,0);}
    static self end  (anchor_type const& a) { return self(a,a.NumOf<value_type>());}
    static int  size (anchor_type const& a) { return a.NumOf<value_type>();}

  private:
    edge_handle e;
  };


  //-------------------- CELL  ITERATOR ----------------------

  class CellIterator : public seq_iterator_base {
    typedef CellIterator self;
  public:  
    typedef grid_cell_iterator_category category;
    typedef Cell value_type;
    typedef Cell element_type;

    CellIterator() : base(0), c(-1) {}
    explicit
    CellIterator(const Grid& g) : base(&g), c(g.MinCellNum()) {}
    //CellIterator(int cc, const Grid* g) : base(g), c(cc) {} 
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

    static self begin(anchor_type const& a) { return self(a,0);}
    static self end  (anchor_type const& a) { return self(a,a.NumOf<value_type>());}
    static int  size (anchor_type const& a) { return a.NumOf<value_type>();}
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
    typedef grid_incidence_iterator_category_d<0,0> category;
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
    typedef grid_incidence_iterator_category_d<2,0> category;
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
    typedef grid_incidence_iterator_category_d<0,2> category;
    typedef Cell   anchor_type;
    typedef Vertex value_type;
    typedef Vertex element_type;

    VertexOnCellIterator() {}
    explicit
    VertexOnCellIterator(const Cell& cc)
      : base(& cc.TheGrid()), v(Cell::SW), c(cc)  {}
    VertexOnCellIterator(const Cell& cc, int vv) : base(& cc.TheGrid()), v(vv), c(cc) {}

    self& operator++() { cv(); ++v; return (*this);}
    self  operator++(int)    { cv(); self tmp(*this); ++(*this); return tmp;}
    Vertex  operator*() const { cv(); return (c.vertex(Cell::corner(v)));}
    // operator Vertex()   const { return this->operator*();}
    bool    IsDone()    const { cb(); return (v >= c.NumOfVertices());}
    // operator bool() const { return !IsDone();} 
    int LocalNumber()   const { cv(); return v;}
    int local_handle()  const { cv(); return v;}

    vertex_handle   handle() const { cv(); return c.vertex(Cell::corner(v)).handle();}

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

    static self begin(anchor_type const& a) { return self(a,0);}
    static self end  (anchor_type const& a) { return self(a,a.NumOf<value_type>());}
    static int  size (anchor_type const& a) { return a.NumOf<value_type>();}

  private:
    int v;
    Cell c;
  };

  //-------------------- EDGE ON CELL ITERATOR ----------------------

  class EdgeOnCellIterator : public inc_iterator_base {
    typedef EdgeOnCellIterator self;
    friend class CellOnCellIterator;
    friend class RegGrid2D;
    typedef grid_types<RegGrid2D::archetype_type> archgt; 
    typedef archgt::Cell archetype_cell;
  public:
    typedef grid_incidence_iterator_category_d<1,2> category;
    typedef Edge value_type;
    typedef Edge element_type;
    typedef Cell anchor_type;

    EdgeOnCellIterator() {}
    explicit
    EdgeOnCellIterator(Cell const& cc) { *this = cc.FirstEdge();}
    EdgeOnCellIterator(Cell const& cc, int ee) : base(cc.TheGrid()), e(ee), c(cc) {}

    self& operator++() { ++e; return (*this);}
    self  operator++(int)    { self tmp(*this); ++(*this); return tmp;}
    Edge  operator*() const { return (TheCell().edge(Cell::side(e)));}
    // FIXME: could be more efficient.
    edge_handle handle() const { return TheCell().edge(Cell::side(e)).handle();}
    bool    IsDone()    const { return (e >= c.NumOfEdges());}
    operator bool() const { return !IsDone();} 

    int LocalNumber()   const { return e;}
    archgt::edge_handle local_handle()  const { cv(); return e;}

    Cell    OtherCell() const { return TheCell().Nb(Cell::side(e));}
    Vertex V1() const { return c.V(e);}
    Vertex V2() const { return c.V((e+1)%4);}

    self CyclicSucc() const 
    { self tmp(*this); tmp.e = (tmp.e == 3 ? 0 : tmp.e+1); return tmp;}
    self CyclicPred() const 
    { self tmp(*this); tmp.e = (tmp.e == 0 ? 3 : tmp.e-1); return tmp;}

    Cell const&  TheCell()     const { cb(); return c;}
    Cell const&  TheAnchor()   const { cb(); return c;}
    archetype_cell ArchetypeCell() const { return archetype_cell(TheGrid().ArchetypeOf(c), e);}

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

    static self begin(anchor_type const& a) { return self(a,0);}
    static self end  (anchor_type const& a) { return self(a,a.NumOf<value_type>());}
    static int  size (anchor_type const& a) { return a.NumOf<value_type>();}
  private:
    int e;
    Cell c;
  };


  //-------------------- CELL ON CELL ITERATOR ----------------------

  class CellOnCellIterator : public inc_iterator_base {
    friend class RegGrid2D;
    typedef CellOnCellIterator self;

  public:
    typedef grid_incidence_iterator_category_d<2,2> category;
    typedef Cell anchor_type;
    typedef Cell element_type;
    typedef Cell value_type;
    
    CellOnCellIterator() {}
    explicit
    CellOnCellIterator(Cell const& cc) { *this = cc.FirstCell();}
    CellOnCellIterator(Cell const& cc, int n) : base(cc.TheGrid()), nb(n), c(cc)
    {  advance_till_valid(); }
    CellOnCellIterator(const EdgeOnCellIterator& F) : base(F), nb(F.e), c(F.c) 
    {  advance_till_valid(); }
    self& operator++() { 
      ++nb;
      advance_till_valid();
      return (*this);
    }
    self  operator++(int)    { self tmp(*this); ++(*this); return tmp;}
    Cell  operator*() const  { return TheCell().neighbour(Cell::side(nb));}
    cell_handle handle() const { return (operator*()).handle();}

    // operator Cell()   const { return this->operator*();}
    Edge facet()      const { return c.edge(Cell::side(LocalNumber()));}
    Edge TheFacet() const { return facet();}

    bool  IsDone()    const { return (nb >= 4);}
    // operator bool() const { return !IsDone();} 
    int LocalNumber()  const { return nb;}

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

    // static self Begin(Cell c) { return self(c);}
    // static self End  (Cell c) { return self(c.EndCell());}

    static self begin(anchor_type const& a) { return self(a,0);}
    static self end  (anchor_type const& a) { return self(a,a.NumOf<value_type>());}
    static int  size (anchor_type const& a) { return a.NumOf<value_type>();}
  private:
    int nb;
    Cell c;
    void advance_till_valid() {
      while( ! IsDone() && c.IsOnBoundary(nb))
	++(*this);
    }
  };
  typedef CellOnCellIterator CellNeighbourIterator;
  //@}

  //----------------------------------------------------------------------- 
  //---------------  Operations of RegGrid2D  ----------------------------- 
  //----------------------------------------------------------------------- 

  //@{ 
  /*! @name Cartesian size information
     \brief Number of elements in each direction
  */
  //! equal to <tt> urx() - llx() + 1 </tt>
  size_type NumOfXVertices() const { return xpoints;}
  size_type NumOfYVertices() const { return ypoints;}
  size_type NumOfXEdges() const { return xpoints-1;}
  size_type NumOfYEdges() const { return ypoints-1;}
  size_type NumOfXCells() const { return xpoints-1;}
  size_type NumOfYCells() const { return ypoints-1;}

  //! Number of \e all edges in x-direction
  size_type NumOfHorizontalEdges() const { return TheXEdgeMap().range_size();}
  //! Number of \e all edges in y-direction
  size_type NumOfVerticalEdges()   const { return TheYEdgeMap().range_size();} 
  //@}

  size_type Offset(const Vertex&) const {return TheVertexMap().n0();}
  size_type Offset(const Edge&  ) const {return TheXEdgeMap().n0();}
  size_type Offset(const Cell&  ) const {return TheCellMap().n0();}


  
  //@{ 
  /*! @name Sequence iteration 
      \brief STL-style iterator ranges.
  */
  VertexIterator FirstVertex() const { return VertexIterator(*this,MinVertexNum());}
  EdgeIterator   FirstEdge()   const { return EdgeIterator  (*this,MinEdgeNum());}
  EdgeIterator   FirstFacet()  const { return EdgeIterator  (*this,MinEdgeNum());}
  CellIterator   FirstFace()   const { return FirstCell();}
  CellIterator   FirstCell()   const { return CellIterator  (*this,MinCellNum());}
  VertexIterator EndVertex()   const { return VertexIterator(*this,MaxVertexNum()+1);}
  EdgeIterator   EndEdge()     const { return EdgeIterator  (*this,MaxEdgeNum()+1);}
  EdgeIterator   EndFacet()    const { return EdgeIterator  (*this,MaxEdgeNum()+1);}
  CellIterator   EndFace()     const { return EndCell();}
  CellIterator   EndCell()     const { return CellIterator  (*this,MaxCellNum()+1);}

  VertexIterator FirstElement(tp<Vertex>) const { return FirstVertex();}
  VertexIterator EndElement  (tp<Vertex>) const { return EndVertex();}
  EdgeIterator   FirstElement(tp<Edge>)   const { return FirstEdge();}
  EdgeIterator   EndElement  (tp<Edge>)   const { return EndEdge();}
  CellIterator   FirstElement(tp<Cell>)   const { return FirstCell();}
  CellIterator   EndElement  (tp<Cell>)   const { return EndCell();}
  
  template<class E>
  typename sequence_iterator<self, typename E::element_type_tag>::type
  FirstElement() const { return FirstElement(tp<E>());}
  template<class E>
  typename sequence_iterator<self, typename E::element_type_tag>::type
  EndElement()   const { return EndElement(tp<E>());}
 

  size_type NumOfVertices() const { return TheVertexMap().range_size();}
  size_type NumOfEdges()    const { return NumOfHorizontalEdges()+NumOfVerticalEdges();}
  size_type NumOfFacets()   const { return NumOfEdges();}
  size_type NumOfFaces()    const { return NumOfCells();}
  size_type NumOfCells()    const { return TheCellMap().range_size();}
  
  size_type NumOf(tp<Vertex>) const { return NumOfVertices();}
  size_type NumOf(tp<Edge  >) const { return NumOfEdges   ();}
  size_type NumOf(tp<Cell  >) const { return NumOfCells   ();}
  template<class E> size_type NumOf() const { return NumOf(tp<E>());}
  //@}

  //@{ 
  /*! @name Element validity tests 
      \brief Check validity of elements
    */
  inline bool IsValid(const vertex_base& v) const { return TheVertexMap().IsInRange(v);}
  inline bool IsValidCellBase(const vertex_base& v) const { 
    return TheCellMap().IsInRange(v);
  }
  inline bool IsValid(const Cell& C)     const;
  //@}

  //@{ 
  /*! @name Boundary related functions
     \brief See $GrAL Grid-With-Boundary concept
  */
  bool IsOnBoundary(const Vertex& V) const { return IsOnBoundary(V.index());}
  inline bool IsOnBoundary(const vertex_base&) const;
  inline bool IsOnBoundary(const Edge&   E) const;
  inline bool IsOnBoundary(const EdgeOnCellIterator& Nb) const;

  inline bool IsInside(const CellIterator& C) const;
  inline bool IsInside(const Cell& C)    const;

  size_type NumOfBoundaryVertices() const { return (2*(xpoints   + ypoints) -4);}
  size_type NumOfBoundaryEdges()    const { return (2*(xpoints-1 + ypoints-1));}
  size_type NumOfBoundaryFacets()   const { return NumOfBoundaryEdges();}
  size_type NumOfBoundaryFaces()    const { return NumOfBoundaryCells();}
  size_type NumOfBoundaryCells()    const { return (2*(xpoints-1 + ypoints-1) -4);}
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

  /*! @name element to handle mappings 

      \brief A handle is a unique identifier for an Element, but only within a fixed grid.
  */
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
 
  /*! @name intervals for element handles 
     \brief These are inclusive bounds.
  */
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

  bool valid(vertex_handle v) const { return MinVertexNum() <= v && v <= MaxVertexNum();}
  bool valid(edge_handle   e) const { return MinEdgeNum()   <= e && e <= MaxEdgeNum();}
  bool valid(cell_handle   c) const { return MinCellNum()   <= c && c <= MaxCellNum();}
  bool valid_vertex(vertex_handle v) const { return valid(v);}
  bool valid_edge  (edge_handle   e) const { return valid(e);}
  bool valid_cell  (cell_handle   c) const { return valid(c);}

  //@}

  /*! @name switch operator 

      \todo Test these operators
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
     \see $GrAL ArchetypedGrid
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


  struct grid_types_cart2d : public grid_types_detail::grid_types_root {
    typedef grid_types_cart2d self;
    
    typedef cartesian2d::RegGrid2D  Grid;
    typedef cartesian2d::RegGrid2D  grid_type;
    typedef Grid::index_type        index_type;
    typedef Grid::size_type         size_type;
    
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
  typedef  vertex_on_edge_iterator<Grid,self>  VertexOnEdgeIterator;
  typedef  VertexOnEdgeIterator           VertexOnFacetIterator;

  typedef  Grid::VertexOnCellIterator   VertexOnCellIterator;
  typedef  Grid::EdgeOnCellIterator     EdgeOnCellIterator;
  typedef  Grid::EdgeOnCellIterator     FacetOnCellIterator;
  typedef  Grid::CellOnCellIterator     CellOnCellIterator;
  typedef  Grid::CellOnCellIterator     NeighbourCellIterator;
  typedef  Grid::CellOnCellIterator     CellNeighbourIterator;

  // 2D: Cell <-> Face
  typedef cell_handle             face_handle;
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


  template<class E, class A>
  static VertexOnEdgeIterator begin_aux(A const& a, grid_vertex_category, grid_edge_category) { return VertexOnEdgeIterator(a);}
  template<class E, class A>
  static VertexOnEdgeIterator end_aux  (A const& a, grid_vertex_category, grid_edge_category) { return VertexOnEdgeIterator(a.EndVertex());}
  template<class E, class A>
  static int size_aux                  (A const& a, grid_vertex_category, grid_edge_category) { return a.NumOfVertices();}


};

} // namespace cartesian2d




/*! \brief specialization of grid_types primary template for RegGrid2D
 */
template<>
struct grid_types<cartesian2d::RegGrid2D> 
  :  public grid_types_base<cartesian2d::grid_types_cart2d>
{ };

} // namespace GrAL

#include "Gral/Grids/Cartesian2D/element-traits.h"

namespace GrAL {

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
  return EndVertex();
}

inline
RegGrid2D::VertexOnVertexIterator 
RegGrid2D::Vertex::EndVertex() const { return VertexOnVertexIterator(4,*this, &TheGrid());}

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
  return (dir == x_dir 
	  ? Cell(TheGrid(),vertex_base(v1_.x(),  v1_.y()-1))
	  : Cell(TheGrid(),vertex_base(v1_.x()-1,v1_.y()  )));
}    

inline  
 RegGrid2D::Cell 
RegGrid2D::Edge::C2()   const  {  // upper resp. right cell
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
{ return VertexOnCellIterator(*this,SW);}

inline 
RegGrid2D::VertexOnCellIterator 
RegGrid2D::Cell::EndVertex()  const 
{ return VertexOnCellIterator(*this,invalid_corner);}

inline 
RegGrid2D::EdgeOnCellIterator   
RegGrid2D::Cell::FirstEdge()  const 
{ return EdgeOnCellIterator(*this,S);}

inline 
RegGrid2D::EdgeOnCellIterator   
RegGrid2D::Cell::EndEdge()  const 
{ return EdgeOnCellIterator(*this,invalid_side);}

inline 
RegGrid2D::EdgeOnCellIterator   
RegGrid2D::Cell::FirstFacet()  const 
{ return EdgeOnCellIterator(*this,S);}

inline 
RegGrid2D::EdgeOnCellIterator   
RegGrid2D::Cell::EndFacet()  const 
{ return EdgeOnCellIterator(*this,invalid_side);}

inline 
RegGrid2D::CellOnCellIterator   
RegGrid2D::Cell::FirstNeighbour() const 
{ return CellOnCellIterator(*this,S);}

inline 
RegGrid2D::CellOnCellIterator   
RegGrid2D::Cell::EndNeighbour() const 
{ return CellOnCellIterator(*this,invalid_side);}


inline 
RegGrid2D::CellOnCellIterator   
RegGrid2D::Cell::FirstCell() const 
{ return FirstNeighbour();}

inline 
RegGrid2D::CellOnCellIterator   
RegGrid2D::Cell::EndCell() const 
{ return EndNeighbour();}

inline 
RegGrid2D::CellOnCellIterator   
RegGrid2D::Cell::FirstFace() const 
{ return FirstNeighbour();}

inline 
RegGrid2D::CellOnCellIterator   
RegGrid2D::Cell::EndFace() const 
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

  return (e < NumOfHorizontalEdges()
	  ? Edge(Edge::x_dir,TheXEdgeMap().index(e),this)
	  : Edge(Edge::y_dir,TheYEdgeMap().index(e-NumOfHorizontalEdges()),this));
}


inline 
RegGrid2D::edge_handle 
RegGrid2D::edge_num(const RegGrid2D::Edge& E) const
{
  return ( E.dir == Edge::x_dir
	   ? TheXEdgeMap().number(E.v1_)
	   : TheYEdgeMap().number(E.v1_) + NumOfHorizontalEdges());
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
  vertex_base bv = F.TheCell().get_nb_base(Cell::side(F.e));
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


  typedef grid_types<RegGrid2D> gt;

  inline gt::VertexIterator   gral_begin(RegGrid2D const& R, gt::VertexIterator) { return R.FirstVertex();}
  inline gt::VertexIterator   gral_end  (RegGrid2D const& R, gt::VertexIterator) { return R.EndVertex();}
  inline gt::size_type        gral_size (RegGrid2D const& R, gt::VertexIterator) { return R.NumOfVertices();}

  inline gt::EdgeIterator     gral_begin(RegGrid2D const& R, gt::EdgeIterator) { return R.FirstEdge();}
  inline gt::EdgeIterator     gral_end  (RegGrid2D const& R, gt::EdgeIterator) { return R.EndEdge();}
  inline gt::size_type        gral_size (RegGrid2D const& R, gt::EdgeIterator) { return R.NumOfEdges();}

  inline gt::CellIterator     gral_begin(RegGrid2D const& R, gt::CellIterator) { return R.FirstCell();}
  inline gt::CellIterator     gral_end  (RegGrid2D const& R, gt::CellIterator) { return R.EndCell();}
  inline gt::size_type        gral_size (RegGrid2D const& R, gt::CellIterator) { return R.NumOfCells();}


  inline gt::VertexOnCellIterator   gral_begin(gt::Cell   a, gt::VertexOnCellIterator)   { return a.FirstVertex();}
  inline gt::VertexOnCellIterator   gral_end  (gt::Cell   a, gt::VertexOnCellIterator)   { return a.EndVertex();}
  inline gt::size_type              gral_size (gt::Cell   a, gt::VertexOnCellIterator)   { return a.NumOfVertices();}
  inline gt::VertexOnEdgeIterator   gral_begin(gt::Edge   a, gt::VertexOnEdgeIterator)   { return a.FirstVertex();}
  inline gt::VertexOnEdgeIterator   gral_end  (gt::Edge   a, gt::VertexOnEdgeIterator)   { return a.EndVertex();}
  inline gt::size_type              gral_size (gt::Edge   a, gt::VertexOnEdgeIterator)   { return a.NumOfVertices();}
  inline gt::VertexOnVertexIterator gral_begin(gt::Vertex a, gt::VertexOnVertexIterator) { return a.FirstVertex();}
  inline gt::VertexOnVertexIterator gral_end  (gt::Vertex a, gt::VertexOnVertexIterator) { return a.EndVertex();}
  inline gt::size_type              gral_size (gt::Vertex a, gt::VertexOnVertexIterator) { return a.NumOfVertices();}

  inline gt::EdgeOnCellIterator     gral_begin(gt::Cell   a, gt::EdgeOnCellIterator) { return a.FirstEdge();}
  inline gt::EdgeOnCellIterator     gral_end  (gt::Cell   a, gt::EdgeOnCellIterator) { return a.EndEdge();}
  inline gt::size_type              gral_size (gt::Cell   a, gt::EdgeOnCellIterator) { return a.NumOfEdges();}
  /*
  inline gt::EdgeOnVertexIterator   gral_begin(gt::Vertex a, gt::Edge) { return a.FirstEdge();}
  inline gt::EdgeOnVertexIterator   gral_end  (gt::Vertex a, gt::Edge) { return a.EndEdge();}
  inline gt::size_type              gral_size (gt::Vertex a, gt::Edge) { return a.NumOfEdges();}
  */

  inline gt::CellOnVertexIterator   gral_begin(gt::Vertex a, gt::CellOnVertexIterator) { return a.FirstCell();}
  inline gt::CellOnVertexIterator   gral_end  (gt::Vertex a, gt::CellOnVertexIterator) { return a.EndCell();}
  inline gt::size_type              gral_size (gt::Vertex a, gt::CellOnVertexIterator) { return a.NumOfCells();}
  /*
  inline gt::CellOnEdgeIterator     gral_begin(gt::Edge   a, gt::Cell) { return a.FirstCell();}
  inline gt::CellOnEdgeIterator     gral_end  (gt::Edge   a, gt::Cell) { return a.EndCell();}
  inline gt::size_type              gral_size (gt::Edge   a, gt::Cell) { return a.NumOfCells();}
  */
  inline gt::CellOnCellIterator     gral_begin(gt::Cell   a, gt::CellOnCellIterator) { return a.FirstCell();}
  inline gt::CellOnCellIterator     gral_end  (gt::Cell   a, gt::CellOnCellIterator) { return a.EndCell();}
  inline gt::size_type              gral_size (gt::Cell   a, gt::CellOnCellIterator) { return a.NumOfCells();}




} // namespace cartesian2d 

} // namespace GrAL 


#endif
