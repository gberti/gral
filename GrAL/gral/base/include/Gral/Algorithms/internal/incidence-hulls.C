#ifndef NMWR_GB_GRID_ALGORITHM_INCIDENCE_HULL_C
#define NMWR_GB_GRID_ALGORITHM_INCIDENCE_HULL_C



/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include "Gral/Algorithms/incidence-hulls.h"
#include "Gral/Base/common-grid-basics.h"
#include "Gral/Subranges/enumerated-element-range.h"

#include "Utility/pre-post-conditions.h"

namespace GrAL {

template<class GT, class CellIt, class VtxSeq, class EltMarker>
void mark_vertices_on_cells(CellIt      seed,
			    VtxSeq    & vertex_seq,
			    EltMarker & visited,
			    int         level)
{
  typedef typename CellIt::grid_type        grid_type;
  typedef GT                                gt;
  typedef typename gt::Cell                 Cell;
  typedef typename gt::Vertex               Vertex;
  typedef typename gt::VertexOnCellIterator VertexOnCellIterator;

  while(! seed.IsDone()) {
    Cell C = *seed;
    for(VertexOnCellIterator vc(C); ! vc.IsDone(); ++vc) {
      Vertex V(*vc);
      if(visited(V) == 0) {
	visited[V] = level;
	vertex_seq.append(V);
      }
    }
    ++seed;
  }
}


//----------------------------------------------------------------
//----------------------------------------------------------------

template<class GT, class CellIt, class CellSeq, class EltMarker, class CellPred>
void mark_cells_on_cells(CellIt     seed,
			 CellSeq&   cell_seq,
			 EltMarker& visited,
			 int        level,
			 CellPred   inside)
{
  typedef typename CellIt::grid_type        grid_type;
  typedef GT                                gt;
  typedef typename gt::Cell                 Cell;
  typedef typename gt::CellOnCellIterator   CellOnCellIterator;

  enumerated_cell_range<grid_type> new_cells(cell_seq.TheGrid()); 
  while(! seed.IsDone()) {
    Cell C = *seed;
    for(CellOnCellIterator cc(C); ! cc.IsDone(); ++cc) 
      if(inside(*cc)) {
	Cell CC(*cc);
	if(visited(CC) == 0) {
	  visited[CC] = level;
	  //cell_seq.append(CC);
	  new_cells.append(CC);
	}
      }
    ++seed;
  }
  cell_seq.append(new_cells.FirstCell(), new_cells.EndCell());
}

template<class GT, class VertexIt, class CellSeq, class EltMarker, class CellPred>
void mark_cells_on_vertices(VertexIt   seed,
			    CellSeq&   cell_seq,
			    EltMarker& visited,
			    int        level,
			    CellPred   inside)
{
  typedef typename VertexIt::grid_type       grid_type;
  typedef GT                                 gt;
  typedef typename gt::Cell                  Cell;
  typedef typename gt::Vertex                Vertex;
  typedef typename gt::CellOnVertexIterator  CellOnVertexIterator;
 
  while(! seed.IsDone()) {
    Vertex V = *seed;
    for(CellOnVertexIterator cv(V); ! cv.IsDone(); ++cv)
      if( inside(*cv)) {
	Cell C(*cv);
	if(visited(C) == 0) {
	  visited[C] = level;
	  cell_seq.append(C);
	}
      }
    ++seed;
  }
}

//----------------------------------------------------------------
//----------------------------------------------------------------
template<class GT, class VertexIt, class VSeq, class CSeq, class EltMarker, class AdjSeq, class CellPred>
void mark_on_vertices(VertexIt    seed,         // in : cell seed set
		      VSeq&       vertex_seq,   // out: visited vertices
		      CSeq&       cell_seq,     // out: visited cells
		      EltMarker&  visited,      // inout: already visited elements
		      AdjSeq&     adj_seq,      // inout: seq of adjacencies to handle
		      int&        level,        // inout: current level of adj.
		      CellPred    inside,
		      bool&       end);


template<class GT, class CellIt, class VSeq, class CSeq, class EltMarker, class AdjSeq, class CellPred>
void mark_on_cells(CellIt      seed,         // in : cell seed set
		   VSeq&       vertex_seq,   // out: visited vertices
		   CSeq&       cell_seq,     // out: visited cells
		   EltMarker&  visited,      // inout: already visited elements
		   AdjSeq&     adj_seq,      // inout: seq of adjacencies to handle
		   int&        level,        // inout: current level of adj.
		   CellPred    inside,
		   bool&       end)
{
  if(adj_seq.empty())
    return;
  if(seed.IsDone()) {
    end = true;
    return;
  }

  grid_element_tag et = adj_seq.front();
  adj_seq.pop();

  switch(et) {
  case vertex_tag:
    vertex_seq.append_layer();
    mark_vertices_on_cells<GT>(seed,vertex_seq,visited,level);
    ++level;
    mark_on_vertices<GT>(vertex_seq.LastLayer().FirstVertex(),
			 vertex_seq,cell_seq,visited,adj_seq,level, inside, end);
    break;
  case cell_tag:
    cell_seq.append_layer();
    mark_cells_on_cells<GT>(seed,cell_seq,visited,level, inside);
    ++level;
    mark_on_cells<GT>(cell_seq.LastLayer().FirstCell(),
		  vertex_seq,cell_seq,visited,adj_seq,level, inside, end);
    break;
  default:
    ENSURE_ALWAYS(false, "element type " << (int)et << " not implemented!\n",1);
    break;
  }
}


template<class GT, class VertexIt, class VSeq, class CSeq, class EltMarker, class AdjSeq, class CellPred>
void mark_on_vertices(VertexIt    seed,         // in : cell seed set
		      VSeq&       vertex_seq,   // out: visited vertices
		      CSeq&       cell_seq,     // out: visited cells
		      EltMarker&  visited,      // inout: already visited elements
		      AdjSeq&     adj_seq,      // inout: seq of adjacencies to handle
		      int&        level,        // inout: current level of adj.
		      CellPred    inside,
		      bool&       end)
{
  if( adj_seq.empty())
    return;
  if(seed.IsDone()) {
    end = true;
    return;
  }

  grid_element_tag et = adj_seq.front();
  adj_seq.pop();

  switch(et) {
  case cell_tag:
    cell_seq.append_layer();
    mark_cells_on_vertices<GT>(seed,cell_seq,visited,level, inside);
    ++level;
    mark_on_cells<GT>(cell_seq.LastLayer().FirstCell(),
		  vertex_seq,cell_seq,visited,adj_seq,level, inside, end);
    break;
  default:
    ENSURE_ALWAYS(false,"element type " << (int)et << " not implemented!\n",1);
    break;
  }
}

} // namespace GrAL 

#endif

