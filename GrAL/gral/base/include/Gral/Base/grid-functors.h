#ifndef GRAL_BASE_GB_GRIDFUNCTORS_H
#define GRAL_BASE_GB_GRIDFUNCTORS_H

//----------------------------------------------------------------
//   (c) Guntram Berti, 1998
//   Chair for Numerical Mathematics & Scientific Computing (NMWR)
//   TU Cottbus - Germany
//   http://math-s.math.tu-cottbus.de/NMWR
//   
//----------------------------------------------------------------
#include "compiler-config.h"
#include "Utility/pre-post-conditions.h"
#include "Gral/Base/common-grid-basics.h"

//----------------------------------------------------------------
//
//  Here some function objects  are defined, that map between 
//  combinatorial grid types, define predicates on grid elements
//  and so on.
//  They are models of STL unary functions.
//  They are useful for adapting element iterators and algorithms.
//
//  Contents:
//  --------
//  [1a] template<class G> class cell2handle_map;
//  [1b] template<class G> class handle2cell_map;
//  [2a] template<class G> class vertex2handle_map;
//  [2b] template<class G> class handle2vertex_map;
//  [3a] template<class G> class element2handle_map
//  [3b] template<class G> class handle2element_map
//
//  [4]  template<class G> class iscellinside_pred;
//  [5a] template<class G> class cell_nb_degree; 
//  [5b] template<class G> class cell_vtx_degree;
//  [6]  template<class Cell> class cell_is_nb_pred;
//
// Central operations:
// ------------------
// [1a,2a,3a]: <handle>  operator()(const <Element>&) const
// [1b,2b,3b]: <Element> operator()(<handle>)         const
//
// [4]    cell -> bool, F(C) = true <=> C is a "real" cell of G
// [5a,b] cell -> int
//   [5a] F(C) = # neighbor cells of C
//   [5b] F(C) = # vertices of C
//
// Constraints on template parameters:
// ----------------------------------
//  - G is a combinatorial grid.
//    * grid_types<G> is defined and contains
//      + [1]: typedef Cell,   cell_handle
//      + [2]: typedef Vertex, vertex_handle
//    * conversion functions
//      [1a] cell_handle   handle(const Cell&)    const;
//      [1b] Cell          cell  (cell_handle)    const;  
//      [2a] vertex_handle handle(const Vertex&)  const;
//      [2b] Vertex        vertex (vertex_handle) const;  
//----------------------------------------------------------------


//----------------------------------------------------------------
//                  [1a]  cell2handle_map
//----------------------------------------------------------------


template<class G>
struct cell2handle_map {
  typedef G                          grid_type;
  typedef grid_types<G>              gt;
  typedef typename   gt::Cell        Cell;
  typedef Cell                       Element;
  typedef Cell                       element_type;
  typedef typename   gt::cell_handle cell_handle;

  //--- DATA ----
  const G* g;
public:
  typedef Cell                       argument_type;
  typedef cell_handle                result_type;

  cell2handle_map()            : g(0)   {}
  cell2handle_map(const G& gg) : g(&gg) {}

  result_type operator()(const argument_type& c) const {
    REQUIRE((g != 0), "no grid!\n",1);
    return g->handle(c);
  }

  grid_type const& TheGrid() const { 
    REQUIRE((g!=0),"no grid!\n",1);
    return *g;
  }
};

template<class G>
inline cell2handle_map<G> cell2handle(const G& g) 
{ return cell2handle_map<G>(g);}

//----------------------------------------------------------------
//                 [1b]  handle2cell_map
//----------------------------------------------------------------

template<class G>
struct handle2cell_map {
  typedef G                          grid_type;
  typedef grid_types<G>              gt;
  typedef typename   gt::Cell        Cell;
  typedef Cell                       element_type;
  typedef typename   gt::cell_handle cell_handle;

  //--- DATA ----
  const G* g;
public:
  typedef Cell                       result_type;
  typedef cell_handle                argument_type;

  handle2cell_map()            : g(0)   {}
  handle2cell_map(const G& gg) : g(&gg) {}

  result_type operator()(const argument_type& c) const { 
    REQUIRE((g != 0), "no grid!\n",1);
    return g->cell(c);
  }
  grid_type const& TheGrid() const { 
    return *g;
  }
};

template<class G>
inline handle2cell_map<G> handle2cell(const G& g) 
{ return handle2cell_map<G>(g);}




//----------------------------------------------------------------
//                [2a]  vertex2handle_map
//----------------------------------------------------------------

template<class G>
struct vertex2handle_map {
  typedef G                            grid_type;
  typedef grid_types<G>                gt;
  typedef typename   gt::Vertex        Vertex;
  typedef Vertex                       Element;
  typedef Vertex                       element_type;
  typedef typename   gt::vertex_handle vertex_handle;

  //--- DATA ----
  const G* g;
public:
  typedef Vertex                       argument_type;
  typedef vertex_handle                result_type;

  vertex2handle_map()            : g(0)   {}
  vertex2handle_map(const G& gg) : g(&gg) {}

  result_type operator()(const argument_type& c) const { 
    REQUIRE((g != 0), "no grid!\n",1);
    return g->handle(c);
  }
  grid_type const& TheGrid() const {
    REQUIRE((g!=0),"no grid!\n",1);
    return *g;
  }
};

template<class G>
inline vertex2handle_map<G> vertex2handle(const G& g) 
{ return vertex2handle_map<G>(g);}


//----------------------------------------------------------------
//                  [2b]  handle2vertex_map
//----------------------------------------------------------------

template<class G>
struct handle2vertex_map {
  typedef G                            grid_type;
  typedef grid_types<G>                gt;
  typedef typename   gt::Vertex        Vertex;
  typedef typename   gt::vertex_handle vertex_handle;

  //--- DATA ----
  const G* g;
public:
  typedef Vertex                       result_type;
  typedef vertex_handle                argument_type;

  handle2vertex_map()            : g(0)   {}
  handle2vertex_map(const G& gg) : g(&gg) {}

  result_type operator()(const argument_type& c) const {
    REQUIRE((g != 0), "no grid!\n",1);
    return g->vertex(c);
  }
  grid_type const& TheGrid() const {
    REQUIRE((g!=0),"no grid!\n",1);
    return *g;
  }
};

template<class G>
inline handle2vertex_map<G> handle2vertex(const G& g) 
{ return handle2vertex_map<G>(g);}



//----------------------------------------------------------------
//                [3a]  element2handle_map
//----------------------------------------------------------------

template<class E>
struct element2handle_map {
  typedef element_traits<E>            et;
  typedef typename et::grid_type       grid_type;
  typedef grid_types<grid_type>        gt;
  typedef typename et::handle_type     element_handle;
  typedef E                            Element;
  typedef E                            element_type;


  //--- DATA ----
  const grid_type* g;
public:
  typedef Element                       argument_type;
  typedef element_handle                result_type;

  element2handle_map()                    : g(0)   {}
  element2handle_map(const grid_type& gg) : g(&gg) {}

  result_type operator()(const argument_type& e) const {
    REQUIRE((g != 0), "no grid!\n",1);
    return g->handle(e);
  }
  grid_type const& TheGrid() const { 
    REQUIRE((g!=0),"no grid!\n",1);
    return *g;
  }
};

template<class E>
inline element2handle_map<E> element2handle(const E& e) 
{ return element2handle_map<E>(e.TheGrid());}

template<class E, class G>
inline element2handle_map<E> element2handle(const tp<E>&, const G& g) 
{ return element2handle_map<E>(g);}


//----------------------------------------------------------------
//                  [3b]  handle2element_map
//----------------------------------------------------------------

template<class E>
struct handle2element_map {
  typedef element_traits<E>            et;
  typedef typename et::grid_type       grid_type;
  typedef grid_types<grid_type>        gt;
  typedef typename et::handle_type     element_handle;
  typedef E                            Element;
  typedef E                            element_type;
  typedef Element                      result_type;
  typedef result_type                  value_type;
  typedef element_handle               argument_type;

  //--- DATA ----
  const grid_type* g;
public:

  handle2element_map()            : g(0)   {}
  handle2element_map(const grid_type& gg) : g(&gg) {}

  result_type operator()(const argument_type& e) const { 
    REQUIRE((g != 0), "no grid!\n",1);
    return et::handle2element(*g,e);
  }
  grid_type const& TheGrid() const { 
    REQUIRE((g!=0),"no grid!\n",1);
    return *g;
  }
};

template<class E>
inline handle2element_map<E> handle2element(const E& e) 
{ return handle2element_map<E>(e.TheGrid());}

template<class E,class G>
inline handle2element_map<E> handle2element(const tp<E>&, const G& g) 
{ return handle2element_map<E>(g);}



//----------------------------------------------------------------
//                  [4] iscellinside_pred
//----------------------------------------------------------------

template<class G>
class iscellinside_pred {
private:
  const G* g;

public:
  typedef G                 grid_type;
  typedef grid_types<G>     gt;
  typedef typename gt::Cell Cell;
  typedef Cell              element_type;
  typedef Cell              argument_type;
  typedef bool              result_type;
  typedef result_type       value_type;

  iscellinside_pred() : g(0) {}
  iscellinside_pred(const G& gg) : g(&gg) {} 
  bool operator()(const Cell& c) const {
    REQUIRE((g!=0),"no grid!\n",1);    
    return g->IsInside(c);
  } 

  grid_type const& TheGrid() const {
    REQUIRE((g!=0),"no grid!\n",1);
    return *g;
  }
};


template<class G>
inline iscellinside_pred<G>
IsCellInside(const G& g) { return iscellinside_pred<G>(g);}


//----------------------------------------------------------------
//                  [4] iscellinside_pred
//----------------------------------------------------------------

template<class E>
class isonboundary_pred {
  typedef element_traits<E>         et;
  typedef typename et::grid_type    grid_type;
  typedef typename et::element_type element_type;
  typedef E                         argument_type;
  typedef bool                      result_type;
  typedef result_type               value_type;
private:
  const grid_type* g;

public:

  isonboundary_pred(grid_type const& gg) : g(&gg) {} 
  bool operator()(const E& e) const { 
    REQUIRE((g!=0),"no grid!\n",1);    
    return g->IsOnBoundary(e);
  } 

  grid_type const& TheGrid() const { 
    REQUIRE((g!=0),"no grid!\n",1);
    return *g;
  }
};


template<class G>
inline 
isonboundary_pred<typename G::Edge>
IsEdgeOnBoundary(const G& g) 
{ return isonboundary_pred<typename grid_types<G>::Edge>(g);} 

template<class G>
inline
isonboundary_pred<typename G::Facet>
IsFacetOnBoundary(const G& g) 
{ return isonboundary_pred<typename grid_types<G>::Facet>(g);} 


//----------------------------------------------------------------
//                  [5a] cell_nb_degree
//----------------------------------------------------------------

template<class G>
class cell_nb_degree {
public:
  typedef G                 grid_type;
  typedef grid_types<G>     gt;
  typedef typename gt::Cell Cell;
  typedef Cell              element_type;
  typedef Cell              argument_type;
  typedef unsigned          result_type;
  typedef result_type       value_type;

  cell_nb_degree() {}
  unsigned operator()(const Cell& c) const { return c.NumOfNeighbours();}

};


template<class G>
inline cell_nb_degree<G>
CellNbDegree(const G& g) { return cell_nb_degree<G>();}

//----------------------------------------------------------------
//                  [5b] cell_vtx_degree
//----------------------------------------------------------------

template<class G>
class cell_vtx_degree {
public:
  typedef G                 grid_type;
  typedef grid_types<G>     gt;
  typedef typename gt::Cell Cell;
  typedef Cell              element_type;
  typedef Cell              argument_type;
  typedef bool              result_type;
  typedef result_type       value_type;

  cell_vtx_degree() {}
  bool operator()(const Cell& c) const { return c.NumOfVertices();}
};


template<class G>
inline cell_vtx_degree<G>
CellVertexDegree(const G& g) { return cell_vtx_degree<G>();}



//----------------------------------------------------------------
//                  [6] cell_is_nb_pred
//----------------------------------------------------------------

template<class Cell>
class cell_is_nb_pred {
public:
  typedef typename Cell::grid_type        grid_type;
  typedef grid_types<grid_type>           gt;
  typedef typename gt::CellOnCellIterator NbIterator;
  typedef Cell                            element_type;
  typedef Cell                            argument_type;
  typedef bool                            result_type;
  typedef result_type                     value_type;

private:
  Cell C;
public:
  cell_is_nb_pred(const Cell& CC) : C(CC) {}

  bool operator()(const Cell& rs) const
    {
      for(NbIterator N = C.FirstNeighbour(); ! N.IsDone(); ++N)
	if(*N == rs)
	  return true;
      return false;
    }

  grid_type const& TheGrid() const { return C.TheGrid();}
};


template<class Cell>
cell_is_nb_pred<Cell> IsNeighbourCell(const Cell& rs)
{ return cell_is_nb_pred<Cell>(rs); }


#endif
