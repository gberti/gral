#ifndef GRAL_GB_BASE_FLAG_H
#define GRAL_GB_BASE_FLAG_H

// $LICENSE

#include "Gral/Base/common-grid-basics.h"

/*! Representation of a  flag (tuple of incident elements, one of each dimension)

  Tests in test/flag/test-flag.C .
 */

template<class GRID, class DIM = typename grid_types<GRID>::dimension_tag> 
class flag {};

// 2D case
template<class GRID>
class flag<GRID, grid_dim_tag<2> > { 
  typedef GRID             grid_type;
  typedef grid_types<GRID> gt;
  typedef typename gt::Vertex Vertex;  
  typedef typename gt::Edge   Edge;
  typedef typename gt::Cell   Face;
private:
  Vertex v_;
  Edge   e_;
  Face   f_;
public:
  flag() {}
  flag(Vertex const& v, Edge const& e, Face const& f)
    : v_(v), e_(e), f_(f) {}

  grid_type const& TheGrid() const { return v_.TheGrid();}

  Vertex const& vertex() const { return v_;}
  Edge   const& edge()   const { return e_;}
  Edge   const& facet()  const { return e_;}
  Face   const& face()   const { return f_;}
  Face   const& cell()   const { return f_;}

  void switch_vertex() { TheGrid().switch_vertex(v_,e_);   }
  void switch_edge()   { TheGrid().switch_edge  (v_,e_,f_);}
  void switch_facet()  { TheGrid().switch_edge  (v_,e_,f_);}
  void switch_face()   { TheGrid().switch_cell  (   e_,f_);}
  void switch_cell()   { TheGrid().switch_cell  (   e_,f_);}
};

// 3D case
template<class GRID>
class flag<GRID, grid_dim_tag<3> > { 
  typedef GRID             grid_type;
  typedef grid_types<GRID> gt;
  typedef typename gt::Vertex Vertex;  
  typedef typename gt::Edge   Edge;
  typedef typename gt::Facet  Facet;
  typedef typename gt::Cell   Cell;

private:
  Vertex v_;
  Edge   e_;
  Facet  f_;
  Cell   c_;
public:
  flag() {}
  flag(Vertex const& v, Edge const& e, 
       Facet  const& f, Cell const& c)
    : v_(v), e_(e), f_(f), c_(c) {}

  Vertex const& vertex() const { return v_;}
  Edge   const& edge()   const { return e_;}
  Facet  const& face()   const { return f_;}
  Facet  const& facet()  const { return f_;}
  Cell   const& cell()   const { return c_;}

  grid_type const& TheGrid() const { return v_.TheGrid();}

  void switch_vertex() { TheGrid().switch_vertex(v_,e_);      }
  void switch_edge()   { TheGrid().switch_edge  (v_,e_,f_);   }
  void switch_face()   { TheGrid().switch_facet (   e_,f_,c_);}
  void switch_facet()  { TheGrid().switch_facet (   e_,f_,c_);}
  void switch_cell()   { TheGrid().switch_cell  (      f_,c_);}

};


#endif








