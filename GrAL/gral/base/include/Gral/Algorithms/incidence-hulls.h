#ifndef NMWR_GB_GRID_ALGORITHM_INCIDENCE_HULL_H
#define NMWR_GB_GRID_ALGORITHM_INCIDENCE_HULL_H


// $LICENSE


/*! \defgroup incidence_hull Incidence Hull Calculation
    \ingroup combinatoric_grid_algorithms

   
   \brief  Incidence hulls are subsets of a grid defined by a germ set
    and a stencil (incidence sequence).

    See the paper "A calculus for stencils on arbitrary grids"
    or $DISS.
    The class \c incidence_hull is the top-level entry point.

   \todo Better demos (now only \ref test-incidence-hulls.C)
  
   \see Test in \ref test-incidence-hulls.C 
 */

#ifdef NMWR_INCLUDE_TEMPLATE_DEFS
#include "Gral/Algorithms/internal/incidence-hulls.C"
// #else
#endif

#include "Gral/Base/partial-multi-grid-functions.h"
#include "Gral/Base/multi-grid-functions.h"
#include "Gral/Algorithms/incidence-stencil.h"
#include "Gral/Subranges/layered-subrange.h"
#include "Gral/Subranges/enumerated-subrange.h"
#include "Container/function-adapter.h"
#include "Utility/ref-ptr.h"

#include <iostream>
#include <vector>

namespace GrAL {

// after execution, vertex_seq contains all vertices
// not marked as visited (visited[v] != 0) that are
// incident to cells in range(seed), 
// and visited(v) == level for these vertices.

template<class GT, class CellIt, class VtxSeq, class EltMarker>
void mark_vertices_on_cells(CellIt      seed,       // in
			    VtxSeq    & vertex_seq, // out
			    EltMarker & visited,    // inout
			    int         level);     // in

template<class CellIt, class VtxSeq, class EltMarker>
void mark_vertices_on_cells(CellIt      seed,       // in
			    VtxSeq    & vertex_seq, // out
			    EltMarker & visited,    // inout
			    int         level)     // in
{ 
  typedef grid_types<typename CellIt::grid_type> gt;
  mark_vertices_on_cells<gt>(seed, vertex_seq, visited, level);
}


// after execution, cell_seq contains all cells
// not marked as visited (visited[c] != 0) that are
// adjacent to cells in range(seed), 
// and visited(c) == level for these cells.
// If cells in range(seed) are not marked as visited
// before execution, they may be contained in cell_seq
template<class GT, class CellIt, class CellSeq, class EltMarker, class CellPred>
void mark_cells_on_cells(CellIt     seed,       // in
			 CellSeq&   cell_seq,   // out
			 EltMarker& visited,    // inout
			 int        level,      // in
			 CellPred   inside);

template<class CellIt, class CellSeq, class EltMarker, class CellPred>
void mark_cells_on_cells(CellIt     seed,       // in
			 CellSeq&   cell_seq,   // out
			 EltMarker& visited,    // inout
			 int        level,      // in
			 CellPred   inside)
{
  typedef grid_types<typename CellIt::grid_type> gt;
  mark_cells_on_cells<gt>(seed, cell_seq, visited, level, inside);
}




template<class GT, class VertexIt, class CellSeq, class EltMarker, class CellPred>
void mark_cells_on_vertices(VertexIt   seed,       // in
			    CellSeq&   cell_seq,   // out
			    EltMarker& visited,    // inout
			    int        level,      // in
			    CellPred   inside);

template<class VertexIt, class CellSeq, class EltMarker, class CellPred>
void mark_cells_on_vertices(VertexIt   seed,       // in
			    CellSeq&   cell_seq,   // out
			    EltMarker& visited,    // inout
			    int        level,      // in
			    CellPred   inside)
{
  typedef grid_types<typename VertexIt::grid_type> gt;
  mark_cells_on_vertices<gt>(seed, cell_seq, visited, level, inside);
}

/*! \brief Calculate incidence hull on cell germ set
    \ingroup incidence_hull
   
 */
template<class GT,
	 class CellIt, 
         class VSeq, 
         class CSeq, 
         class EltMarker, 
         class AdjSeq, 
         class CellPred> 
void mark_on_cells(CellIt      seed,         //!< in : cell seed set
		   VSeq&       vertex_seq,   //!< out: visited vertices
		   CSeq&       cell_seq,     //!< out: visited cells
		   EltMarker&  visited,      //!< inout: already visited elements
		   AdjSeq&     adj_queue,    //!< inout: seq of adjacencies to handle
		   int&        level,        //!< inout: current level of adj.
		   CellPred    inside,       //!< in: restrict cell range to cells \c c with <tt>inside(c)==true</tt>
		   bool &      end);         //!< inout: true if seed range is empty


/*! \brief Calculate incidence hull on cell germ set
    \ingroup incidence_hull
 */
template<class CellIt, 
         class VSeq, 
         class CSeq, 
         class EltMarker, 
         class AdjSeq, 
         class CellPred>
void mark_on_cells(CellIt      seed,         //!< in : cell seed set
		   VSeq&       vertex_seq,   //!< out: visited vertices
		   CSeq&       cell_seq,     //!< out: visited cells
		   EltMarker&  visited,      //!< inout: already visited elements
		   AdjSeq&     adj_queue,    //!< inout: seq of adjacencies to handle
		   int&        level,        //!< inout: current level of adj.
		   CellPred    inside,       //!< in: restrict cell range to cells \c c with <tt>inside(c)==true</tt>
		   bool&       end)          //!< inout: true if seed range is empty
{
 typedef grid_types<typename CellIt::grid_type> gt;
 mark_on_cells<gt>(seed, vertex_seq, cell_seq, visited, adj_queue, level, inside, end);
}


/*! \brief Calculate incidence hull on vertex germ set
    \ingroup incidence_hull
   
 */
template<class GT,
	 class VertexIt, 
         class VSeq, 
         class CSeq, 
         class EltMarker, 
         class AdjSeq, 
         class CellPred>
void mark_on_vertices(VertexIt    seed,         //!< in : vertex seed set
		      VSeq&       vertex_seq,   //!< out: visited vertices
		      CSeq&       cell_seq,     //!< out: visited cells
		      EltMarker&  visited,      //!< inout: already visited elements
		      AdjSeq&     adj_queue,    //!< inout: seq of adjacencies to handle
		      int&        level,        //!< inout: current level of adj.
		      CellPred    inside,       //!< in: restrict cell range to cells \c c with <tt>inside(c)==true</tt>
		      bool&       end);         //!< inout: true if seed range is empty

/*!  \brief Calculate incidence hull on vertex germ set
    \ingroup incidence_hull   
 */
template<class VertexIt, 
         class VSeq, 
         class CSeq, 
         class EltMarker, 
         class AdjSeq, 
         class CellPred>
void mark_on_vertices(VertexIt    seed,         //!< in : vertex seed set
		      VSeq&       vertex_seq,   //!< out: visited vertices
		      CSeq&       cell_seq,     //!< out: visited cells
		      EltMarker&  visited,      //!< inout: already visited elements
		      AdjSeq&     adj_queue,    //!< inout: seq of adjacencies to handle
		      int&        level,        //!< inout: current level of adj.
		      CellPred    inside,       //!< in: restrict cell range to cells \c c with <tt>inside(c)==true</tt>
		      bool&       end)          //!< inout: true if seed range is empty
{
 typedef grid_types<typename VertexIt::grid_type> gt;
 mark_on_vertices<gt>(seed, vertex_seq, cell_seq, visited, adj_queue, level, inside, end);
}

/*! \brief Default config class for incidence_hull

    \ingroup incidence_hull
    
    This class can be used as parameter to instantiate incidence_hull.
*/
template<class GRID, class GT = grid_types<GRID> >
struct incidence_hull_cfg {
  typedef GRID              grid_type;
  typedef GT                gt;
  typedef incidence_stencil stencil_type;
};



/*! \brief Calculate incidence hull

    \ingroup incidence_hull

    \templateparam 
    - CFG:
      - <code> typedef grid_type </code>
      - <code> typedef gt        </code>
      - <code> typedef stencil_type </code>
     

    \see \ref test-incidence-hulls.C
*/
template<class CFG>
class incidence_hull
{
public:
  typedef typename CFG::stencil_type stencil_type;
  typedef typename CFG::gt           gt;
  typedef typename gt::grid_type grid_type;
  typedef typename gt::Vertex    Vertex;
  typedef typename gt::Cell      Cell;

  typedef layered_vertex_range<grid_type>          vertex_range;
  typedef layered_cell_range<grid_type>            cell_range;
  typedef typename vertex_range::range_type_ref    vertex_layer_type;
  typedef typename cell_range::range_type_ref      cell_layer_type;

  typedef typename vertex_range::VertexIterator LayerVertexIterator;
  typedef typename cell_range  ::CellIterator   LayerCellIterator;

  enum  periodic_flag { periodic, non_periodic};
private: 
  vertex_range v_layers;
  cell_range   c_layers;
  partial_multi_grid_function<grid_type, int> visited;
  std::vector<int> celllevel2layer;
  std::vector<int> vertexlevel2layer;
  stencil_type the_stencil;
  int          far_distance_;
public:
  //! \brief Construct empty hull
  incidence_hull() : far_distance_(-1) {}

  /*! \brief Construct from seed range.

    Equivalent to 
    \code
     indicence_hull H();
     H.init(seed,stencil,p_flag);
    \endcode
  */
  template<class RANGE>
  incidence_hull(RANGE const& seed, stencil_type const& stencil, periodic_flag p_flag = non_periodic);

  /*! \brief  Construct from seed range.

    Equivalent to 
    \code
     indicence_hull H();
     H.init(seed,stencil,p_flag, pred);
    \endcode
  */
  template<class RANGE, class PRED>
  incidence_hull(RANGE const& seed, stencil_type const& stencil, periodic_flag p_flag, PRED pred);

  //! \brief initialize with seed, stencil (optionally periodic)
  template<class RANGE>
  void init   (RANGE const& seed, stencil_type const& stencil, periodic_flag p_flag = non_periodic);

  //! \brief initialize with seed, stencil (optionally periodic), considering only elements matching pred.
  template<class RANGE, class PRED>
  void init   (RANGE const& seed, stencil_type const& stencil, periodic_flag p_flag, PRED pred);

  //! \brief make this hull empty (stencil is preserved)
  void clear();
  //! \brief true iff hull is empty
  bool empty() const { return NumOfVertexLayers() == 0 && NumOfCellLayers() == 0;}

  template<class RANGE, class PRED>
  void compute(RANGE const& seed, stencil_type const& stencil, periodic_flag p_flag, PRED pred);

  //! \brief recompute hull after adding seed to seed range
  template<class RANGE>
  void add_seed_range     (RANGE const& seed);
  /*! \brief recompute hull after adding seed to seed range

      Works incrementally. Layers are not correct afterwards, but distance map interface is.
  */
  template<class RANGE>
  void add_seed_range_incr(RANGE const& seed);

  /*! \brief recompute hull after removing seed from seed range
   */
  template<class RANGE>
  void remove_seed_range(RANGE const& seed);


  grid_type    const& TheGrid()  const { return v_layers.TheGrid();}
  /*! \brief Vertex range of hull
    \note This is not necessarily equal to the closure of \c cells()
   */
  vertex_range const& vertices() const { return v_layers;}
  /*! \brief Cell range of hull
   */
  cell_range   const& cells   () const { return c_layers;}


  LayerVertexIterator FirstVertex() const { return v_layers.FirstVertex();}
  LayerVertexIterator EndVertex()   const { return v_layers.EndVertex();}
  LayerCellIterator   FirstCell()   const { return c_layers.FirstCell();}
  LayerCellIterator   EndCell()     const { return c_layers.EndCell();}


  /*! \name Element levels
      \brief Level the element is in.
      This is the level number in the \e total set of layers for \e all elements,
      which are typically interleaved. For instance, a stencil VCVCVC... 
      will generate level numbers 0,2,4,... for the vertices and 1,3,5,... for the cells.
      \note This is \e not the layer number to use eg. in \c vertices()
  */
  //@{
  int level(Vertex const& v) const { return visited(v);}
  int level(Cell   const& c) const { return visited(c);}
  //@}


  template<class CFG2>
  class contains_pred_type {
  private:
    ref_ptr<incidence_hull<CFG2> const> h;
  public:
    contains_pred_type(        incidence_hull const& hh) : h(hh) {}
    contains_pred_type(ref_ptr<incidence_hull const> hh) : h(hh) {}
      
    template<class E>
    bool operator()(E const& e) const { return h->contains(e);}
  };

  /*! \brief Predicate returning true iff an element is inside the hull
      <tt> H.contains_pred()(e)</tt> is equivalent to <tt> H.contains(e) </tt>. 
  */
  contains_pred_type<CFG> contains_pred() const { return contains_pred_type<CFG>(*this);}

  /*! \brief  True iff e is inside hull.
  */
  template<class E>
  bool contains(E const& e) const { return visited.defined(e);}

  /*! \name Distance function interface
      \brief Hull interpreted as discrete distance function.
      
      If the element is not inside the hull, \c far_distance() is returned.

  */
  //@{
  int operator()(Vertex const& v) const { return (visited.defined(v) ? layer(v)-1 : far_distance_);}
  int operator()(Cell   const& c) const { return (visited.defined(c) ? layer(c)-1 : far_distance_);}
  int  far_distance() const { return far_distance_;}
  //! \brief set value used for elements outside the hull
  void set_far_distance(int f) { far_distance_ = f;}
  //@}

  /*! \name Element layers.
      \brief  Consecutive layers of elements
  */   


  //@{
  /*! \brief Consecutive layer of vertex v

      \post \c v \f$\in\f$ <tt>vertices(layer(v))</tt>
  */
  int layer(Vertex const& v) const { return vertexlevel2layer[level(v)];}
  /*! \brief Consecutive layer of cell c 

      \post \c c \f$\in\f$ <tt>cells(layer(c))</tt>
  */
  int layer(Cell   const& c) const { return celllevel2layer  [level(c)];}

  int NumOfVertexLayers() const { return v_layers.NumOfLayers();}
  int NumOfCellLayers  () const { return c_layers.NumOfLayers();}
  /*! \brief i-th vertex layer (<tt> 1 <= i <= NumOfVertexLayers() </tt>)
   */
  temporary<vertex_layer_type> vertices(int i) const { return temporary<vertex_layer_type>(v_layers.Layer(i));}
  /*! \brief i-th cell layer (<tt> 1 <= i <= NumOfCellLayers()</tt>)
   */
  temporary<cell_layer_type  > cells   (int i) const { return temporary<cell_layer_type  >(c_layers.Layer(i));}
  //@}

}; // class incidence_hull


template<class CFG>
template<class RANGE>
incidence_hull<CFG>::incidence_hull
(RANGE const& seed,
 typename incidence_hull<CFG>::stencil_type const& stencil,
 typename incidence_hull<CFG>::periodic_flag p_flag)
  : v_layers(seed.TheGrid()), 
    c_layers(seed.TheGrid()),
    visited (seed.TheGrid(), 0),
    far_distance_(-1)
{
  compute(seed, stencil, p_flag, constant<typename gt::Cell, bool>(true));
}

template<class CFG>
template<class RANGE, class PRED>
incidence_hull<CFG>::incidence_hull
(RANGE const& seed,
 typename incidence_hull<CFG>::stencil_type const& stencil,
 typename incidence_hull<CFG>::periodic_flag p_flag,
 PRED pred)
  : v_layers(seed.TheGrid()), 
    c_layers(seed.TheGrid()),
    visited(seed.TheGrid(), 0),
    far_distance_(-1)
{
  compute(seed, stencil, p_flag, pred);
}

template<class CFG>
void incidence_hull<CFG>::clear()
{
  v_layers.clear();
  c_layers.clear();
  visited.clear();
  celllevel2layer.clear();
  vertexlevel2layer.clear();
  // far_distance_ = -1;
}

template<class CFG>
template<class RANGE>
void incidence_hull<CFG>::init
(RANGE const& seed,
 typename incidence_hull<CFG>::stencil_type const& stencil,
 typename incidence_hull<CFG>::periodic_flag p_flag)
{
  v_layers.set_grid(seed.TheGrid());
  c_layers.set_grid(seed.TheGrid());
  visited .set_grid(seed.TheGrid(), 0);
  compute(seed, stencil, p_flag, constant<typename gt::Cell, bool>(true) );
}


template<class CFG>
template<class RANGE, class PRED>
void incidence_hull<CFG>::init
(RANGE const& seed,
 typename incidence_hull<CFG>::stencil_type  const& stencil,
 typename incidence_hull<CFG>::periodic_flag p_flag,
 PRED pred)
{
  v_layers.set_grid(seed.TheGrid());
  c_layers.set_grid(seed.TheGrid());
  visited .set_grid(seed.TheGrid(), 0);
  compute(seed, stencil, p_flag, pred);
}

 
template<class CFG>
template<class RANGE, class PRED>
void incidence_hull<CFG>::compute
(RANGE const& seed,
 typename incidence_hull<CFG>::stencil_type const& stencil,
 typename incidence_hull<CFG>::periodic_flag p_flag,
 PRED pred)
{
  the_stencil = stencil;
  typedef grid_types<RANGE> rgt;
  int  lev = 1;
  int  cnt = 1; 
  bool end = false;
  stencil_type s = stencil;
  if(s.front() == vertex_tag) {
    s.pop();
    v_layers.append_layer();
    for(typename rgt::VertexIterator v=seed.FirstVertex(); !v.IsDone(); ++v) {
      v_layers.append(*v);
      visited[*v] = lev;
    }
    while(!end && cnt > 0) { 
      stencil_type s2 = s;
      mark_on_vertices<gt>(v_layers.LastLayer().FirstVertex(),
			   v_layers, c_layers,
			   visited,
			   s2,
			   lev,
			   pred,
			   end);
      if(p_flag == non_periodic)
	--cnt;
    }
  } // vertex
  else if(s.front() == cell_tag) {
    s.pop();
    c_layers.append_layer();
    for(typename rgt::CellIterator c(seed.FirstCell()); !c.IsDone(); ++c) {
      c_layers.append(*c);
      visited[*c] = lev;
    }
    while( ! end && cnt > 0) { 
      stencil_type s2 = s;
      mark_on_cells<gt>(c_layers.LastLayer().FirstCell(),
			v_layers, c_layers,
			visited,
			s2,
			lev,
			pred,
			end);
      if(p_flag == non_periodic)
	--cnt;
    }
  } // if cell_tag
  else {
     std::cerr << "Incidence hull: Unsupported element type: " << s.front() <<  std::endl;
  }
  // remove possible empty layers
  if(v_layers.NumOfLayers() > 0 && v_layers.LastLayer().NumOfVertices() == 0)
    v_layers.remove_layer();
  if(c_layers.NumOfLayers() > 0 && c_layers.LastLayer().NumOfCells() == 0)
    c_layers.remove_layer();

  // init level<->layer conversion
  // note that positions which do not correspond to level numbers remain unassigned.
  if(NumOfVertexLayers() > 0) {
    vertexlevel2layer.clear();
    vertexlevel2layer.resize(level(* v_layers.LastLayer().FirstVertex()) + 1, -1);
    for(int layer = 1; layer <= NumOfVertexLayers(); ++layer) {
      vertexlevel2layer[level(* vertices(layer)->FirstVertex())] = layer;
    }
  }

  if(NumOfCellLayers() > 0) {
    celllevel2layer  .clear();
    celllevel2layer  .resize(level(* c_layers.LastLayer().FirstCell())   + 1, -1);
    for(int layer = 1; layer <= NumOfCellLayers(); ++layer) {
      celllevel2layer[level(*cells(layer)->FirstCell())] = layer;
    }
  }

  if(far_distance_ == -1) {
    far_distance_ = 1 + std::max( (vertexlevel2layer.empty() ? 0 : vertexlevel2layer.back()),
				  (celllevel2layer  .empty() ? 0 : celllevel2layer  .back()));
  }

} 




template<class CFG>
template<class RANGE>
void incidence_hull<CFG>::add_seed_range_incr(RANGE const& range)
{
  //FIXME: invalidates layers!
  // level2layer does not change, as the same stencil is used.
  typedef grid_types<RANGE> rgt;
  if(!range.empty()) {
    incidence_hull<CFG> range_hull(range, the_stencil, non_periodic, constant<typename gt::Cell, bool>(true)); 
    for(LayerVertexIterator v(range_hull.FirstVertex()); !v.IsDone(); ++v)
      // if( ! visited.defined(*v) || (*this)(*v) > range_hull(*v))
      if( (*this)(*v) > range_hull(*v))
	visited[*v] = range_hull.visited(*v);
    for(LayerCellIterator   c(range_hull.FirstCell()); !c.IsDone(); ++c)
      if( (*this)(*c) > range_hull(*c))
	visited[*c] = range_hull.visited(*c);
  }
}

template<class CFG>
template<class RANGE>
void incidence_hull<CFG>::add_seed_range(RANGE const& range)
{
  // could be done much more economically.
  typedef grid_types<RANGE> rgt;
  if(!range.empty()) {
    if(the_stencil.front() == vertex_tag) {
      enumerated_subrange<grid_type> union_range(range.TheGrid()); 
      //enumerated_vertex_range<grid_type> union_range(range.TheGrid()); 
      typedef grid_types<enumerated_vertex_range<grid_type> > egt;
      partial_grid_function<typename gt::Vertex, bool> in_old_range(union_range.TheGrid(), false);
      if(NumOfVertexLayers() > 0) {
	for(LayerVertexIterator v(* vertices(1)); !v.IsDone(); ++v) {
	  in_old_range[*v] = true;
	  union_range.push_back(*v);
	}
      }
      for(typename rgt::VertexIterator v(range.FirstVertex()); ! v.IsDone(); ++v)
	if(! in_old_range(*v))
	  union_range.push_back(*v);

      clear();
      compute(union_range, the_stencil, non_periodic, constant<typename gt::Cell, bool>(true));
    }
    else if(the_stencil.front() == cell_tag) {
      enumerated_subrange<grid_type> union_range(range.TheGrid()); 
      //enumerated_cell_range<grid_type> union_range(range.TheGrid()); 
      typedef grid_types<enumerated_cell_range<grid_type> > egt;
      partial_grid_function<typename gt::Cell, bool> in_old_seed(union_range.TheGrid(), false);
      if(NumOfCellLayers() > 0) {
	for(LayerCellIterator c(* cells(1)); !c.IsDone(); ++c) {
	  in_old_seed[*c] = true;
	  union_range.push_back(*c);
	}
      }
      for(typename rgt::CellIterator v(range.FirstCell()); ! v.IsDone(); ++v)
	if(! in_old_seed(*v))
	  union_range.push_back(*v);
      clear();
      compute(union_range, the_stencil, non_periodic, constant<typename gt::Cell, bool>(true));
    }  
  }
}




template<class CFG>
template<class RANGE>
void incidence_hull<CFG>::remove_seed_range(RANGE const& range)
{
  // could be done much more economically.
  typedef grid_types<RANGE> rgt;
  if(! range.empty()) {
    if(the_stencil.front() == vertex_tag) {
      if(NumOfVertexLayers() > 0) {
	enumerated_subrange<grid_type> diff(range.TheGrid());
	typedef grid_types<enumerated_vertex_range<grid_type> > egt;
	partial_grid_function<typename gt::Vertex, bool> in_range(range.TheGrid(), false);
	for(typename rgt::VertexIterator v(range); !v.IsDone(); ++v)
	  in_range[*v] = true;
	for(typename egt::VertexIterator v(* vertices(1)); ! v.IsDone(); ++v)
	  if(! in_range(*v))
	    diff.push_back(*v);
	clear();
	compute(diff, the_stencil, non_periodic, constant<typename gt::Cell, bool>(true));
      }
    }
    else if(the_stencil.front() == cell_tag) {
      if(NumOfCellLayers() > 0) {
	enumerated_subrange<grid_type> diff(range.TheGrid());
	typedef grid_types<enumerated_cell_range<grid_type> > egt;
	partial_grid_function<typename gt::Cell, bool> in_range(range.TheGrid(), false);
	for(typename rgt::CellIterator c(range); !c.IsDone(); ++c)
	  in_range[*c] = true;
	for(typename egt::CellIterator c(* cells(1)); ! c.IsDone(); ++c)
	  if(! in_range(*c))
	    diff.push_back(*c);
	clear();
	compute(diff, the_stencil, non_periodic, constant<typename gt::Cell, bool>(true));
      }
    }  
  }
}




} // namespace GrAL 

#endif
