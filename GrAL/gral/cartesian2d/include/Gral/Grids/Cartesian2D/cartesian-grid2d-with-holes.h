#ifndef NMWR_GB_CARTESIAN_GRID_2D_WITH_HOLES
#define NMWR_GB_CARTESIAN_GRID_2D_WITH_HOLES


//----------------------------------------------------------------
//   (c) Guntram Berti, 1997
//   Chair for Numerical Mathematics & Scientific Computing (NMWR)
//   TU Cottbus - Germany
//   http://math-s.math.tu-cottbus.de/NMWR
//   
//----------------------------------------------------------------


/**
   @author Guntram Berti
   @memo   Mask for regular grid -- cuts out rectangular hole.
 */


#include "cartesian-grid2d.h"

/// Only one hole, at the moment.
/// This  class is primarily intended as mask for RegGrid2D,
/// in order to generate another type of grid from it
/// for example an unstructured grid.

class RegGrid2DWithHoles {
public:
  typedef RegGrid2DWithHoles Grid1;
  typedef RegGrid2D RegGrid;
  typedef  RegGrid::vertex_base vertex_base;
  typedef  RegGrid::Cell   Cell;
  typedef  RegGrid::Vertex Vertex;
  typedef  RegGrid::VertexOnCellIterator VertexOnCellIterator;

  struct rectangle {
    rectangle(const vertex_base& ll1, const vertex_base& ur1) 
      : ll(ll1), ur(ur1) {}
    rectangle(int llx, int lly, int urx, int ury)
      : ll(llx,lly), ur(urx,ury) {}
    vertex_base ll, ur; // lower left and upper right vertex.
  }; 
  

  //---------------- CONSTRUCTORS -------------------------------------

  RegGrid2DWithHoles(int nx, int ny, const rectangle& rect) 
    : _grid(new RegGrid2D(nx,ny)), _the_hole(rect) {}
  RegGrid2DWithHoles(int nx, int ny, 
		     vertex_base const& ll,
		     vertex_base const& ur)
    : _grid(new RegGrid2D(nx,ny)), _the_hole(rectangle(ll,ur)) {}
  RegGrid2DWithHoles(int nx, int ny, int llx, int lly, int urx, int ury)
    : _grid(new RegGrid2D(nx,ny)), 
      _the_hole(rectangle(llx,lly,urx,ury)) {}
  

  //-------------------------------------------------------------------
  //
  //           ITERATORS
  //
  //-------------------------------------------------------------------
 

  class iterator_base1 {
  public:
    typedef iterator_base1 base;
    iterator_base1(const Grid1* g) : _g(g) {}
    const Grid1& TheGrid() const { return (*_g);}
  protected:
    const Grid1* _g;
  };

  //-------------------------------------------------------------------

  class CellIterator1 : public iterator_base1 {
  public: 
    typedef CellIterator1  self;
    CellIterator1(const RegGrid::CellIterator& c, const Grid1* g) 
      : base(g), _c(c)  
    { advance_till_valid();}

    self& operator++() { ++_c; advance_till_valid(); return (*this);}
    self  operator++(int) {self tmp(*this); ++(*this); return tmp;}
    Cell operator*() const { return (*_c);}
    operator Cell()  const { return this->operator*();}
    bool IsDone()    const { return _c.IsDone();}

    int handle() const { return _c.handle();}
  private:
    //-- DATA --//
     RegGrid::CellIterator _c;
    //--      --//

    void advance_till_valid() 
    { 
      while(! _c.IsDone()  && ! TheGrid().IsInside(*_c))
	++_c;
    }
  };

  //-------------------------------------------------------------------

  class VertexIterator1 : public iterator_base1 {
  public: 
    typedef VertexIterator1  self;
    VertexIterator1(const  RegGrid::VertexIterator& v, const Grid1* g) 
      :  base(g), _v(v) 
    { advance_till_valid();}

    self& operator++() { ++_v; advance_till_valid(); return (*this);}
    self  operator++(int) {self tmp(*this); ++(*this); return tmp;}
    Vertex operator*() const { return (*_v);}
    operator Vertex()  const { return this->operator*();}
    bool IsDone()    const { return _v.IsDone();}

    int handle() const { return _v.handle();}
  private:
    //-- DATA --//
     RegGrid::VertexIterator _v;
    //--      --//

    void advance_till_valid() 
    { 
      while(  ! _v.IsDone()  &&  ! TheGrid().IsInside(*_v) ) {
	++_v;
      }
    }
  };

  //-------------------------------------------------------------------
 
  VertexIterator1 FirstVertex() const { return VertexIterator1(BaseGrid().FirstVertex(),this);}
  CellIterator1   FirstCell()   const { return CellIterator1(BaseGrid().FirstCell(),this);}
 
  
  bool IsInside(const Cell& C)   const   { return !IsInside(C, _the_hole);}
  bool IsInside(const Vertex& V) const   { return !IsInside(V, _the_hole);}

  /*
  typedef  RegGrid::VertexIndexType VertexIndexType;
  typedef  RegGrid::CellIndexType CellIndexType;
  VertexIndexType index(const Vertex& V) const { return BaseGrid().index(V);}
  CellIndexType   index(const Cell& C)   const { return BaseGrid().index(C);}
  */

  typedef  RegGrid::vertex_handle vertex_handle;
  typedef  RegGrid::cell_handle   cell_handle;
  vertex_handle  handle(const Vertex& V) const { return BaseGrid().handle(V);}
  cell_handle    handle(const Cell& C)   const { return BaseGrid().handle(C);}

  typedef VertexIterator1 VertexIterator;
  typedef CellIterator1   CellIterator;

  const RegGrid& BaseGrid() const { return (*_grid);}
private:
  //-- DATA --//
  RegGrid* _grid;
  //list<rectangle> _holes;
  rectangle _the_hole;
  //--      --//

  bool IsInside(const Cell& C, const rectangle& RR) const 
  {
    rectangle R(enlarge_on_boundary(RR));
    vertex_base v(BaseGrid().get_cell_llv(C.GlobalNumber()));
    return( (R.ll.x <= v.x) && ( v.x < R.ur.x) &&
	    (R.ll.y <= v.y) && ( v.y < R.ur.y));
  }

  bool IsInside(const Vertex& V, const rectangle& RR) const
    // muss fuer mehrere Loecher geandert werden, da auch       !!!!!
    // die Vertices auf dem Rand von R evtl. nicht im Gitter     !!!
    // liegen.                                                    !
  {
    rectangle R(enlarge_on_boundary(RR));
    vertex_base v(V.x(),V.y());
    return( (R.ll.x < v.x) && ( v.x < R.ur.x) &&
	    (R.ll.y < v.y) && ( v.y < R.ur.y)); 
  }

  /*
  quadrants inside_quadrants(const Vertex& V, const rectangle& R) const 
  {
   if(! (V.x >= R.ll.x) && (V.y >= R.ll.y) && (V.x <= R.ur.x) && (V.y <= R.ur.y))
     return quadrants(false);
   
  }
  */

  rectangle enlarge_on_boundary(const rectangle& R) const
  {
    // "enlarge" R if it touches the boundary, so that the
    // inclusion tests become easier.
    // This could happen on initialization of a hole.
    int llx = ( R.ll.x  != 0 ? R.ll.x : -1);
    int lly = ( R.ll.y  != 0 ? R.ll.y : -1);
    int urx = ( R.ur.x  != BaseGrid().NumXpoints() -1 ? R.ur.x : BaseGrid().NumXpoints());
    int ury = ( R.ur.y  != BaseGrid().NumYpoints() -1 ? R.ur.y : BaseGrid().NumYpoints());

    return rectangle(llx,lly,urx,ury);
  }


};

//-------------------------------------------------------------------
//-------------------------------------------------------------------


struct grid_types<RegGrid2DWithHoles>  {
  typedef grid_types<RegGrid2D> rgt;

  typedef  rgt::Vertex Vertex;
  typedef  rgt::Cell   Cell;

  typedef  RegGrid2DWithHoles::VertexIterator VertexIterator;
  typedef  RegGrid2DWithHoles::CellIterator CellIterator;

  typedef  rgt::VertexOnCellIterator VertexOnCellIterator;

  typedef  RegGrid2DWithHoles::vertex_handle vertex_handle;
  typedef  RegGrid2DWithHoles::cell_handle   cell_handle;
};

#endif
