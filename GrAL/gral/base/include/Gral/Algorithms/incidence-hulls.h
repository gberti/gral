#ifndef NMWR_GB_GRID_ALGORITHM_INCIDENCE_HULL_H
#define NMWR_GB_GRID_ALGORITHM_INCIDENCE_HULL_H


// $LICENSE


/*! \defgroup incidence_hull Incidence Hull Calculation
    \ingroup combinatoric_grid_algorithms

   
   \brief  Incidence hulls are subsets of a grid defined by a germ set
    and a stencil (incidence sequence).

    See the paper "A calculus for stencils on arbitrary grids"
    or $DISS.

   \todo Document the functions
   \todo Implement test cases and demos
   \todo Write more user-friendly wrappers to the mark_xxx routines.
 */

#ifdef NMWR_INCLUDE_TEMPLATE_DEFS
#include "Gral/Algorithms/internal/incidence-hulls.C"
// #else
#endif

#include "Gral/Base/partial-multi-grid-functions.h"
#include "Gral/Subranges/layered-subrange.h"
#include "Container/function-adapter.h"
#include "Utility/ref-ptr.h"

#include <iostream>

// after execution, vertex_seq contains all vertices
// not marked as visited (visited[v] != 0) that are
// adjacent to cells in range(seed), 
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
void mark_on_cells(CellIt      seed,         // in : cell seed set
		   VSeq&       vertex_seq,   // out: visited vertices
		   CSeq&       cell_seq,     // out: visited cells
		   EltMarker&  visited,      // inout: already visited elements
		   AdjSeq&     adj_queue,    // inout: seq of adjacencies to handle
		   int&        level,        // inout: current level of adj.
		   CellPred    inside);


template<class CellIt, 
         class VSeq, 
         class CSeq, 
         class EltMarker, 
         class AdjSeq, 
         class CellPred>
void mark_on_cells(CellIt      seed,         // in : cell seed set
		   VSeq&       vertex_seq,   // out: visited vertices
		   CSeq&       cell_seq,     // out: visited cells
		   EltMarker&  visited,      // inout: already visited elements
		   AdjSeq&     adj_queue,    // inout: seq of adjacencies to handle
		   int&        level,        // inout: current level of adj.
		   CellPred    inside)
{
 typedef grid_types<typename CellIt::grid_type> gt;
 mark_on_cells<gt>(seed, vertex_seq, cell_seq, visited, adj_queue, level, inside);
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
void mark_on_vertices(VertexIt    seed,         // in : vertex seed set
		      VSeq&       vertex_seq,   // out: visited vertices
		      CSeq&       cell_seq,     // out: visited cells
		      EltMarker&  visited,      // inout: already visited elements
		      AdjSeq&     adj_queue,    // inout: seq of adjacencies to handle
		      int&        level,        // inout: current level of adj.
		      CellPred    inside);

template<class VertexIt, 
         class VSeq, 
         class CSeq, 
         class EltMarker, 
         class AdjSeq, 
         class CellPred>
void mark_on_vertices(VertexIt    seed,         // in : vertex seed set
		      VSeq&       vertex_seq,   // out: visited vertices
		      CSeq&       cell_seq,     // out: visited cells
		      EltMarker&  visited,      // inout: already visited elements
		      AdjSeq&     adj_queue,    // inout: seq of adjacencies to handle
		      int&        level,        // inout: current level of adj.
		      CellPred    inside)
{
 typedef grid_types<typename VertexIt::grid_type> gt;
 mark_on_vertices<gt>(seed, vertex_seq, cell_seq, visited, adj_queue, level, inside);
}


/*! \brief Calculate incidence hull

    \see test-incidence-hulls.C
*/
template<class RANGE, class STENCIL, class GT>
class incidence_hull
{
public:
  typedef RANGE                  range_type;
  typedef STENCIL                stencil_type;
  typedef typename GT::grid_type grid_type;
  typedef typename GT::Vertex    Vertex;
  typedef typename GT::Cell      Cell;

  typedef layered_vertex_range<grid_type>  vertex_range;
  typedef layered_cell_range<grid_type>    cell_range;
  typedef typename vertex_range::range_type_ref    vertex_layer_type;
  typedef typename cell_range::range_type_ref      cell_layer_type;

  typedef vertex_range::VertexIterator rgeVertexIterator;
  typedef cell_range  ::CellIterator   rgeCellIterator;

  enum  periodic_flag { periodic, non_periodic};
private: 
  vertex_range v_layers;
  cell_range   c_layers;
  partial_multi_grid_function<grid_type, int> visited;
public:
  incidence_hull() {}
  incidence_hull(range_type const& seed, stencil_type const& stencil, periodic_flag p_flag = non_periodic);
  template<class PRED>
  incidence_hull(range_type const& seed, stencil_type const& stencil, periodic_flag p_flag, PRED pred);

  void init   (range_type const& seed, stencil_type const& stencil, periodic_flag p_flag = non_periodic);
  template<class PRED>
  void init   (range_type const& seed, stencil_type const& stencil, periodic_flag p_flag, PRED pred);


  template<class PRED>
  void compute(range_type const& seed, stencil_type const& stencil, periodic_flag p_flag, PRED pred);

  grid_type    const& TheGrid()  const { return v_layers.TheGrid();}
  vertex_range const& vertices() const { return v_layers;}
  cell_range   const& cells   () const { return c_layers;}

  //! This is not the layer number, because vertex and cell layers are counts are interleaved.
  int level(Vertex const& v) const { return visited(v);}
  int level(Cell   const& c) const { return visited(c);}

  temporary<vertex_layer_type> vertices(int i) const { return temporary<vertex_layer_type>(v_layers.Layer(i));}
  temporary<cell_layer_type  > cells   (int i) const { return temporary<cell_layer_type  >(c_layers.Layer(i));}

};


template<class RANGE, class STENCIL, class GT>
incidence_hull<RANGE, STENCIL, GT>::incidence_hull
(typename incidence_hull<RANGE, STENCIL, GT>::range_type const& seed,
 typename incidence_hull<RANGE, STENCIL, GT>::stencil_type const& stencil,
 typename incidence_hull<RANGE, STENCIL, GT>::periodic_flag p_flag)
  : v_layers(seed.TheGrid()), 
    c_layers(seed.TheGrid()),
    visited(seed.TheGrid(), 0)
{
  compute(seed, stencil, p_flag, constant<typename GT::Cell, bool>(true));
}

template<class RANGE, class STENCIL, class GT>
template<class PRED>
incidence_hull<RANGE, STENCIL, GT>::incidence_hull
(typename incidence_hull<RANGE, STENCIL, GT>::range_type const& seed,
 typename incidence_hull<RANGE, STENCIL, GT>::stencil_type const& stencil,
 typename incidence_hull<RANGE, STENCIL, GT>::periodic_flag p_flag,
 PRED pred)
  : v_layers(seed.TheGrid()), 
    c_layers(seed.TheGrid()),
    visited(seed.TheGrid(), 0)
{
  compute(seed, stencil, p_flag, pred);
}

template<class RANGE, class STENCIL, class GT>
void incidence_hull<RANGE, STENCIL, GT>::init
(typename incidence_hull<RANGE, STENCIL, GT>::range_type   const& seed,
 typename incidence_hull<RANGE, STENCIL, GT>::stencil_type const& stencil,
 typename incidence_hull<RANGE, STENCIL, GT>::periodic_flag p_flag)
{
  v_layers.set_grid(seed.TheGrid());
  c_layers.set_grid(seed.TheGrid());
  visited .set_grid(seed.TheGrid(), 0);
  compute(seed, stencil, p_flag, constant<bool, typename GT::Cell>(true) );
}


template<class RANGE, class STENCIL, class GT>
template<class PRED>
void incidence_hull<RANGE, STENCIL, GT>::init
(typename incidence_hull<RANGE, STENCIL, GT>::range_type   const& seed,
 typename incidence_hull<RANGE, STENCIL, GT>::stencil_type const& stencil,
 typename incidence_hull<RANGE, STENCIL, GT>::periodic_flag p_flag,
 PRED pred)
{
  v_layers.set_grid(seed.TheGrid());
  c_layers.set_grid(seed.TheGrid());
  visited .set_grid(seed.TheGrid(), 0);
  compute(seed, stencil, p_flag, pred);
}

 
template<class RANGE, class STENCIL, class GT>
template<class PRED>
void incidence_hull<RANGE, STENCIL, GT>::compute
(typename incidence_hull<RANGE, STENCIL, GT>::range_type const& seed,
 typename incidence_hull<RANGE, STENCIL, GT>::stencil_type const& stencil,
 typename incidence_hull<RANGE, STENCIL, GT>::periodic_flag p_flag,
 PRED pred)
{
  int level = 1;
  int cnt   = 1; 
  stencil_type s = stencil;
  if(s.front() == vertex_tag) {
    s.pop();
    v_layers.append_layer();
    for(typename range_type::VertexIterator v=seed.FirstVertex(); !v.IsDone(); ++v) {
      v_layers.append(*v);
      visited[*v] = level;
    }
    while(v_layers.LastLayer().NumOfVertices() > 0 && cnt > 0) {
      stencil_type s2 = s;
      mark_on_vertices<GT>(v_layers.LastLayer().FirstVertex(),
			   v_layers, c_layers,
			   visited,
			   s2,
			   level,
			   pred);
      if(p_flag == non_periodic)
	--cnt;
    }
  } // vertex
  else if(s.front() == cell_tag) {
    s.pop();
    c_layers.append_layer();
    for(typename range_type::CellIterator c=seed.FirstCell(); !c.IsDone(); ++c) {
      c_layers.append(*c);
      visited[*c] = level;
    }
    while(c_layers.LastLayer().NumOfCells() > 0 && cnt > 0) {
      stencil_type s2 = s;
      mark_on_cells<GT>(c_layers.LastLayer().FirstCell(),
			v_layers, c_layers,
			visited,
			s2,
			level,
			pred);
      if(p_flag == non_periodic)
	--cnt;
    }
  } // cell
  else {
    std::cerr << "Incidence hull: Unsupported element type: " << s.front() << std::endl;
  }
  if(p_flag == periodic) {
    if(v_layers.LastLayer().NumOfVertices() == 0)
      v_layers.remove_layer();
    if(c_layers.LastLayer().NumOfCells() == 0)
      c_layers.remove_layer();
  }
}



#endif
