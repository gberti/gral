#ifndef NMWR_GB_GRIDS_OUTPUT_ALGORITHMS_C
#define NMWR_GB_GRIDS_OUTPUT_ALGORITHMS_C

// $LICENSE

#include "Gral/Distributed/grid-to-mpi-graph.h"

#include "Utility/pre-post-conditions.h"

#include "Gral/Base/common-grid-basics.h"
#include "Gral/Base/grid-functors.h"

#include "Gral/Algorithms/sequence-copy.h"
#include "Gral/Subranges/element-sequence-references.h"


template<class Grid>
int mpi_graph_size_of_index_array(const Grid& G)
{ return (G.NumOfCells() == 0 ? 1 : G.NumOfCells() ); }

template<class Grid>
int mpi_graph_size_of_edges_array(const Grid& G)
{ 
  int i = 0;
  int sz = range_sum(CellSet(G),CellNbDegree(G),i);
  return (sz == 0 ? 1 : sz);
}

template<class Grid>
void mpi_graph_format(const Grid& G, 
		      int* index, int* index_end, 
		      int* edges, int* edges_end)
{
  REQUIRE(( (index_end-index) >= (int) G.NumOfCells() -1), "index array to small!\n",1);
  int n2 = mpi_graph_size_of_edges_array(G);
  REQUIRE( ((edges_end -edges) >= n2), "edges array to small!\n",1);
  n2 = n2; // use n2 to suppress warnings

  typedef grid_types<Grid> gt;
  typedef typename gt::Cell               Cell;
  typedef typename gt::CellIterator       CellIterator;
  typedef typename gt::CellOnCellIterator CellOnCellIterator;

  grid_function<Cell,int> numbers(G);
  CellIterator C;
  int cnt = 0; 
  for(C = G.FirstCell(); ! C.IsDone(); ++C,++cnt)
    numbers[*C] = cnt;
  
  int i = 0;
  int e = 0;
  for(C = G.FirstCell(); ! C.IsDone(); ++C,++i) {
    for(CellOnCellIterator Nb = (*C).FirstNeighbour(); !Nb.IsDone(); ++Nb) {
      edges[e] = numbers(*Nb);
      ++e;
    }
    //    if(i+1 < G.NumOfCells()) // index[i] is information for cell i+1
    index[i] = e; // last index gives end of edges[] array
  }
}


#endif 
