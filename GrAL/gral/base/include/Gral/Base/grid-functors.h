#ifndef GRAL_BASE_GB_GRIDFUNCTORS_H
#define GRAL_BASE_GB_GRIDFUNCTORS_H


// $LICENSE

#include "Config/compiler-config.h"
#include "Utility/pre-post-conditions.h"
#include "Gral/Base/common-grid-basics.h"

//----------------------------------------------------------------
/*! \defgroup element2handle  Element <-> Handle conversion function objects
    \ingroup  functors

  Here some function objects  are defined that map between 
  combinatorial grid types and their handle types
  They are models of STL Adaptable Unary Function.

  \b Contents:
  -  template<class G> class cell2handle_map;
  -  template<class G> class handle2cell_map;
  -  template<class G> class vertex2handle_map;
  -  template<class G> class handle2vertex_map;
  -  template<class G> class element2handle_map;
  -  template<class G> class handle2element_map;


 <b> Central operations: <b>
 ------------------
  - xxx2handle: <tt> <handle>  operator()(const <Element>&) const </tt>
  - handle2xxx: <tt> <Element> operator()(<handle>)         const </tt>


 <b> Template parameters: </b>
  - G is a combinatorial grid.
    - grid_types<G> is defined and contains
      - [1]: typedef Cell,   cell_handle
      - [2]: typedef Vertex, vertex_handle
    - conversion functions
      [1a] cell_handle   handle(const Cell&)    const;
      [1b] Cell          cell  (cell_handle)    const;  
      [2a] vertex_handle handle(const Vertex&)  const;
      [2b] Vertex        vertex (vertex_handle) const;  




*/
//----------------------------------------------------------------


/*!  \defgroup predicates Grid predicates
     \ingroup functors

  \b Contents:
  - template<class G> class isonboundary_pred; <BR>
    edge->bool, facet->bool, F(e) = true <=> e is on the boundary of G.
  - template<class G> class iscellinside_pred; <BR>
    cell -> bool, F(C) = true <=> C is a "real" cell of G
  - template<class Cell> class cell_is_nb_pred;
    C x C -> bool, F(C,N) = true <=> C and N are adjacent. 
   
 */





/*!  \brief Map cells to their handles
     \ingroup element2handle

     Model of Adaptable Unary Function  Cell -> handle
*/

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
    return c.handle();
  }

  grid_type const& TheGrid() const { 
    REQUIRE((g!=0),"no grid!\n",1);
    return *g;
  }
};

/*! \brief Creator function for cell2handle_map
  \ingroup element2handle
  \relates cell2handle_map
 */
template<class G>
inline cell2handle_map<G> cell2handle(const G& g) 
{ return cell2handle_map<G>(g);}





/*!  \brief Map handles to corresponding cells
     \ingroup element2handle

     Model of Adaptable Unary Function handle -> Cell.
*/
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

/*! \brief Creator function for handle2cell_map
  \ingroup element2handle
  \relates handle2cell_map
*/
template<class G>
inline handle2cell_map<G> handle2cell(const G& g) 
{ return handle2cell_map<G>(g);}




//----------------------------------------------------------------
//                [2a]  vertex2handle_map
//----------------------------------------------------------------


/*!  \brief Map vertices to their handles
     \ingroup element2handle

     Model of Adaptable Unary Function  vertex -> handle
*/
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
    return c.handle();
  }
  grid_type const& TheGrid() const {
    REQUIRE((g!=0),"no grid!\n",1);
    return *g;
  }
};

/*! \brief Creator function for vertex2handle_map
  \ingroup element2handle
  \relates vertex2handle_map
 */
template<class G>
inline vertex2handle_map<G> vertex2handle(const G& g) 
{ return vertex2handle_map<G>(g);}


//----------------------------------------------------------------
//                  [2b]  handle2vertex_map
//----------------------------------------------------------------


/*!  \brief Map handles to corresponding vertices
     \ingroup element2handle

     Model of Adaptable Unary Function  handle -> Vertex
*/
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

/*! \brief Creator function for handle2vertex_map
  \ingroup element2handle
  \relates handle2vertex_map
 */
template<class G>
inline handle2vertex_map<G> handle2vertex(const G& g) 
{ return handle2vertex_map<G>(g);}



//----------------------------------------------------------------
//                [3a]  element2handle_map
//----------------------------------------------------------------

/*!  \brief Map elements to their handles
     \ingroup element2handle

     Model of Adaptable Unary Function  Element -> handle <BR>
     This is more general than vertex2handle_map or cell2handle2_map.
*/
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
    return e.handle();
  }
  grid_type const& TheGrid() const { 
    REQUIRE((g!=0),"no grid!\n",1);
    return *g;
  }
};

/*! \brief Creator function for element2handle_map
  \ingroup element2handle
  \relates element2handle_map
 */
template<class E>
inline element2handle_map<E> element2handle(const E& e) 
{ return element2handle_map<E>(e.TheGrid());}

/*! \brief Creator function for element2handle_map
  \ingroup element2handle
  \relates element2handle_map
*/
template<class E, class G>
inline element2handle_map<E> element2handle(const tp<E>&, const G& g) 
{ return element2handle_map<E>(g);}


//----------------------------------------------------------------
//                  [3b]  handle2element_map
//----------------------------------------------------------------

/*!  \brief Map handles to corresponding elements
     \ingroup element2handle

     Model of Adaptable Unary Function  handle -> Element <BR>
     This is more general than handle2vertex_map or handle2cell_map.

*/
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

/*! \brief Creator function for handle2element_map
  \ingroup element2handle
  \relates handle2element_map
 */
template<class E>
inline handle2element_map<E> handle2element(const E& e) 
{ return handle2element_map<E>(e.TheGrid());}

/*! \brief Creator function for handle2element_map
  \ingroup element2handle
  \relates handle2element_map
 */
template<class E,class G>
inline handle2element_map<E> handle2element(const tp<E>&, const G& g) 
{ return handle2element_map<E>(g);}



//----------------------------------------------------------------
//                  [4] iscellinside_pred
//----------------------------------------------------------------

/*! \brief Predicate returning true iff a Cell is not an `outer' cell
    \ingroup predicates
    
    An outer cell is an artificial cell denoting the outside of a grid.
    For each valid cell c of a grid, F(c) is true.

    <b> Template parameters: </b>
    - G: $GrAL Grid-With-Boundary
 */
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

/*! \brief Creator function for iscellinside_pred
  \ingroup predicates
 */
template<class G>
inline iscellinside_pred<G>
IsCellInside(const G& g) { return iscellinside_pred<G>(g);}


/*! \brief Predicate returning true iff a Edge/Facet is on the boundary
    
    <b> Template parameters: </b>
    - G: $GrAL Grid-With-Boundary
    \ingroup predicates
 */
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

/*! \brief creator function for isonboundary_pred
    \ingroup predicates
    \relates isonboundary_pred
 */
template<class G>
inline 
isonboundary_pred<typename G::Edge>
IsEdgeOnBoundary(const G& g) 
{ return isonboundary_pred<typename grid_types<G>::Edge>(g);} 

/*! \brief creator function for isonboundary_pred
    \ingroup predicates
    \relates isonboundary_pred
 */
template<class G>
inline
isonboundary_pred<typename G::Facet>
IsFacetOnBoundary(const G& g) 
{ return isonboundary_pred<typename grid_types<G>::Facet>(g);} 



/*! \defgroup  degreefunctors
    \ingroup functors
  
    \b Contents:   
   - template<class G> class cell_nb_degree; <BR>
     F(C) = Number of neighbors of C
   - template<class G> class cell_vtx_degree; <BR>
     F(C) = Number of vertices of C
 */


/*! \brief Functor giving number of neighbors of a Cell
   \ingroup degreefunctors
   
   
 */
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

/*! \brief creator function for cell_nb_degree
   \relates cell_nb_degree
 */
template<class G>
inline cell_nb_degree<G>
CellNbDegree(const G& g) { return cell_nb_degree<G>();}

/*! \brief Functor giving number of vertices of a Cell
   \ingroup degreefunctors
   
   
 */

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


/*! \brief creator function for cell_vtx_degree
   \relates cell_vtx_degree
 */
template<class G>
inline cell_vtx_degree<G>
CellVertexDegree(const G& g) { return cell_vtx_degree<G>();}



//----------------------------------------------------------------
//                  [6] cell_is_nb_pred
//----------------------------------------------------------------

/*! \brief Predicate returning true iff two cells are neighbors
    \ingroup predicates
    
    <b> Template parameters: </b>
    - G: $GrAL GridRange
    - G::Cell : CellGridRange 

 */
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

/*! \brief Creator function for cell_is_nb_pred
    \ingroup predicates
 */
template<class Cell>
cell_is_nb_pred<Cell> IsNeighbourCell(const Cell& rs)
{ return cell_is_nb_pred<Cell>(rs); }


#endif
