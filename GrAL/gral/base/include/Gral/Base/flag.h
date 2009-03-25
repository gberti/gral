#ifndef GRAL_GB_BASE_FLAG_H
#define GRAL_GB_BASE_FLAG_H


/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti, 
    Contact: guntram AT users DOT berlios DOT de

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include "Gral/Base/common-grid-basics.h"
#include "Gral/Base/predicates.h"

namespace GrAL {

/*! \brief Representation of a  flag (tuple of incident elements, one of each dimension)

  \ingroup elements
  \see elements module

  Flags are useful for local %incidence iteration, 
  using the switch operator. An example is found in 
  <tt> \ref calculate-vertex-stars.C </tt>

  Tests are in <tt> \ref test-flag.C </tt>

  \todo Flags are implemented only for 2D and 3D.
 */

template<class GRID, class DIM = typename grid_types<GRID>::dimension_tag> 
class flag {};



// 1D case
template<class GRID>
class flag<GRID, grid_dim_tag<1> > { 
public:
  typedef GRID             grid_type;
  typedef grid_types<GRID> gt;
  typedef typename gt::Vertex Vertex;  
  typedef typename gt::Edge   Edge;
  typedef typename gt::Facet  Facet;
  typedef typename gt::Cell   Cell;
private:
  Vertex v_;
  Edge   e_;
public:
  flag() {}
  flag(Vertex const& v, Edge const& e)
    : v_(v), e_(e) {}
 
  grid_type const& TheGrid() const { return v_.TheGrid();}

  Vertex const& vertex() const { return v_;}
  Edge   const& edge()   const { return e_;}
  Facet  const& facet()  const { return v_;}
  Cell   const& cell()   const { return e_;}

  void switch_vertex() { TheGrid().switch_vertex(v_,e_);}
  void switch_edge()   { TheGrid().switch_cell  (v_,e_);}
  void switch_facet()  { TheGrid().switch_vertex(v_,e_);}
  void switch_cell()   { TheGrid().switch_cell  (v_,e_);}

  Vertex switched_vertex()   const { return TheGrid().switched_vertex(v_,e_);}
  Edge   switched_edge()     const { return TheGrid().switched_cell  (v_,e_);}
  Vertex switched_facet()    const { return TheGrid().switched_vertex(v_,e_);}
  Edge   switched_cell()     const { return TheGrid().switched_cell  (v_,e_);}
};



// 2D case
template<class GRID>
class flag<GRID, grid_dim_tag<2> > { 
public:
  typedef GRID             grid_type;
  typedef grid_types<GRID> gt;
  typedef typename gt::Vertex Vertex;  
  typedef typename gt::Edge   Edge;
  typedef typename gt::Facet  Facet;
  typedef typename gt::Cell   Face;
  typedef typename gt::Cell   Cell;
private:
  Vertex v_;
  Edge   e_;
  Face   f_;
public:
  flag() {}
  flag(Vertex const& v, Edge const& e, Face const& f)
    : v_(v), e_(e), f_(f) {}
 
  flag(Vertex const& v, Cell const& c) : v_(v), f_(c) 
  {
    REQUIRE(incident(v,c), "", 1);
    for(typename gt::FacetOnCellIterator  e(c); !e.IsDone(); ++e) {
      if(incident(v,*e)) {
	e_ = *e;
	goto found;
      }
    }
    // we should always leave the loop via the successful case
    ENSURE( false, "Could not find flag for vertex v=" << v.handle() << " and cell c=" << c.handle(), 1);
  found:
    ;
  }

  grid_type const& TheGrid() const { return v_.TheGrid();}

  Vertex const& vertex() const { return v_;}
  Edge   const& edge()   const { return e_;}
  Facet  const& facet()  const { return e_;}
  Face   const& face()   const { return f_;}
  Cell   const& cell()   const { return f_;}

  void switch_vertex() { TheGrid().switch_vertex(v_,e_);   }
  void switch_edge()   { TheGrid().switch_edge  (v_,e_,f_);}
  void switch_facet()  { TheGrid().switch_edge  (v_,e_,f_);}
  void switch_face()   { TheGrid().switch_cell  (   e_,f_);}
  void switch_cell()   { TheGrid().switch_cell  (   e_,f_);}

  Vertex switched_vertex() const { return TheGrid().switched_vertex(v_,e_   );}
  Edge   switched_edge()     const { return TheGrid().switched_edge  (v_,e_,f_);}
  Facet  switched_facet()    const { return TheGrid().switched_edge  (v_,e_,f_);}
  Face   switched_face()     const { return TheGrid().switched_cell  (   e_,f_);}
  Cell   switched_cell()     const { return TheGrid().switched_cell  (   e_,f_);}
};

// 3D case
template<class GRID>
class flag<GRID, grid_dim_tag<3> > { 
public:
  typedef GRID             grid_type;
  typedef grid_types<GRID> gt;
  typedef typename gt::Vertex Vertex;  
  typedef typename gt::Edge   Edge;
  typedef typename gt::Facet  Face;
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
  flag(Vertex const& v, Cell const& c) : v_(v), c_(c) 
  {
    REQUIRE(incident(v,c), "", 1);
    for(typename gt::FacetOnCellIterator  f(c); !f.IsDone(); ++f) {
      for(typename gt::EdgeOnFacetIterator ef(*f); !ef.IsDone(); ++ef) {
	if(incident(v,*ef)) {
	  e_ = *ef;
	  f_ = *f;
	  goto found;
	}
      }
    }
    // we should always leave the loop via the successful case
    ENSURE( false, "Could not find flag for vertex v=" << v.handle() << " and cell c=" << c.handle(), 1);
  found:
    ;
  }

  Vertex const& vertex() const { return v_;}
  Edge   const& edge()   const { return e_;}
  Face   const& face()   const { return f_;}
  Facet  const& facet()  const { return f_;}
  Cell   const& cell()   const { return c_;}

  grid_type const& TheGrid() const { return v_.TheGrid();}

  void switch_vertex() { TheGrid().switch_vertex(v_,e_);      }
  void switch_edge()   { TheGrid().switch_edge  (v_,e_,f_);   }
  void switch_face()   { TheGrid().switch_facet (   e_,f_,c_);}
  void switch_facet()  { TheGrid().switch_facet (   e_,f_,c_);}
  void switch_cell()   { TheGrid().switch_cell  (      f_,c_);}

  Vertex switched_vertex() const { return TheGrid().switched_vertex(v_,e_);      }
  Edge   switched_edge()   const { return TheGrid().switched_edge  (v_,e_,f_);   }
  Face   switched_face()   const { return TheGrid().switched_facet (   e_,f_,c_);}
  Facet  switched_facet()  const { return TheGrid().switched_facet (   e_,f_,c_);}
  Cell   switched_cell()   const { return TheGrid().switched_cell  (      f_,c_);}

};

} // namespace GrAL 

#endif








