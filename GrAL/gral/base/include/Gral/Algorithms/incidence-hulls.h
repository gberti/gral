#ifndef NMWR_GB_GRID_ALGORITHM_INCIDENCE_HULL_H
#define NMWR_GB_GRID_ALGORITHM_INCIDENCE_HULL_H

//----------------------------------------------------------------
//   (c) Guntram Berti, 1998
//   Chair for Numerical Mathematics & Scientific Computing (NMWR)
//   TU Cottbus - Germany
//   http://math-s.math.tu-cottbus.de/NMWR
//   
//----------------------------------------------------------------


//----------------------------------------------------------------
//  mark_hull_on_cells
//  mark_hull_on_vertices
//
//  calculate sequences of cells and vertices, given a seed-set 
//  of cells or vertices, and a sequence of adjacencies ( == nested
//  applications of adjacency iterators)
//----------------------------------------------------------------


// after execution, vertex_seq contains all vertices
// not marked as visited (visited[v] != 0) that are
// adjacent to cells in range(seed), 
// and visited(v) == level for these vertices.
template<class CellIt, class VtxSeq, class EltMarker>
void mark_vertices_on_cells(CellIt      seed,       // in
			    VtxSeq    & vertex_seq, // out
			    EltMarker & visited,    // inout
			    int         level);     // in

// after execution, cell_seq contains all cells
// not marked as visited (visited[c] != 0) that are
// adjacent to cells in range(seed), 
// and visited(c) == level for these cells.
// If cells in range(seed) are not marked as visited
// before execution, they may be contained in cell_seq
template<class CellIt, class CellSeq, class EltMarker, class CellPred>
void mark_cells_on_cells(CellIt     seed,       // in
			 CellSeq&   cell_seq,   // out
			 EltMarker& visited,    // inout
			 int        level,      // in
			 CellPred   inside);


template<class VertexIt, class CellSeq, class EltMarker, class CellPred>
void mark_cells_on_vertices(VertexIt   seed,       // in
			    CellSeq&   cell_seq,   // out
			    EltMarker& visited,    // inout
			    int        level,      // in
			    CellPred   inside);


template<class CellIt, class VSeq, class CSeq, class EltMarker, class AdjSeq,  class CellPred>
void mark_on_cells(CellIt      seed,         // in : cell seed set
		   VSeq&       vertex_seq,   // out: visited vertices
		   CSeq&       cell_seq,     // out: visited cells
		   EltMarker&  visited,      // inout: already visited elements
		   AdjSeq&     adj_queue,    // inout: seq of adjacencies to handle
		   int&        level,        // inout: current level of adj.
		   CellPred    inside);


template<class VertexIt, class VSeq, class CSeq, class EltMarker, class AdjSeq, class CellPred>
void mark_on_vertices(VertexIt    seed,         // in : vertex seed set
		      VSeq&       vertex_seq,   // out: visited vertices
		      CSeq&       cell_seq,     // out: visited cells
		      EltMarker&  visited,      // inout: already visited elements
		      AdjSeq&     adj_queue,    // inout: seq of adjacencies to handle
		      int&        level,        // inout: current level of adj.
		      CellPred    inside);


#ifdef NMWR_INCLUDE_TEMPLATE_DEFS
#include "Gral/Algorithms/internal/incidence-hulls.C"
#endif

#endif
