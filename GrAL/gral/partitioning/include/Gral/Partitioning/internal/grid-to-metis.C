#ifndef NMWR_GB_GRID_TO_METIS_C
#define NMWR_GB_GRID_TO_METIS_C

// $LICENSE

#include "Gral/Partitioning/grid-to-metis.h"
#include "Gral/Base/common-grid-basics.h"

template<class GRID>
void write_metis_cell_graph(GRID const& G, 
                            std::ostream& out)
{
  typedef grid_types<GRID>                gt;
  typedef typename gt::Cell               Cell;
  typedef typename gt::Facet              Facet;
  typedef typename gt::CellIterator       CellIterator;
  typedef typename gt::CellOnCellIterator CellOnCellIterator;

  //     num of graph nodes       num of graph edges
  out << G.NumOfCells() << ' ' << G.NumOfFacets() - G.NumOfBoundaryFacets()
    //    no weights
      << "  0" << "\n"; // !! NO blank line, as Metis takes it for the first vertex

  grid_function<Cell,int> numC(G);
  CellIterator c;

  // enumerate cells
  int c_cnt = 1; // Metis starts at 1.
  for(c = G.FirstCell(); ! c.IsDone(); ++c, ++c_cnt) {
    numC[*c] = c_cnt;
  }
  // output cells = graph nodes.
  // each lines i contains nodes v_k adj to node v_i, 
  // and weights w_k associated to (v_i,v_k).
  for(c = G.FirstCell(); ! c.IsDone(); ++c, ++c_cnt) {
    for(CellOnCellIterator cc = (*c).FirstCell(); !cc.IsDone(); ++cc) {
      out << numC(*cc) << ' '; 
    }
    out << '\n'; // Important! Metis distinguishes records by newlines!
  }
  
  
}

template<class GRID, class FACETFCT>
void write_metis_cell_graph_edge_weights(GRID const& G, 
                                         FACETFCT const& weights,
                                         std::ostream& out)
{
  typedef grid_types<GRID>                gt;
  typedef typename gt::Cell               Cell;
  typedef typename gt::Facet              Facet;
  typedef typename gt::CellIterator       CellIterator;
  typedef typename gt::CellOnCellIterator CellOnCellIterator;

  //     num of graph nodes       num of graph edges  
  out << G.NumOfCells() << ' ' << G.NumOfFacets() - G.NumOfBoundaryFacets()
    //  weights on edges
      << "  1" << "\n"; // !! NO blank line, as Metis takes it for the first vertex

  grid_function<Cell,int> numC(G);
  CellIterator c;

  // enumerate cells
  int c_cnt = 1; // Metis starts at 1.
  for(c = G.FirstCell(); ! c.IsDone(); ++c, ++c_cnt) {
    numC[*c] = c_cnt;
  }
  // output cells = graph nodes.
  // each lines i contains nodes v_k adj to node v_i, 
  // and weights w_k associated to (v_i,v_k).
  for(c = G.FirstCell(); ! c.IsDone(); ++c, ++c_cnt) {
    for(CellOnCellIterator cc = (*c).FirstCell(); !cc.IsDone(); ++cc) {
      out << numC(*cc) << ' ' << (int) (weights(Facet(cc))) << ' ';
    }
    out << '\n'; // Important! Metis distinguishes records by newlines!
  }
  
  
}

template<class GRID>
int  write_metis_mesh(GRID const& G, std::ostream& out)
{

  typedef grid_types<GRID> gt;  
  typename gt::CellIterator ci( G.FirstCell() );
  int              num( (*ci).NumOfVertices() );
  for ( ; !ci.IsDone(); ++ci ) {
    if ( (*ci).NumOfVertices() != num ) {
      cerr << "Cells with different number of vertices detected!\n"
	   << "This grid can't be converted to METIS mesh!\n"
	   << "(Hint: try to convert to METIS graph, using write_metis_graph())\n";
      return 1;
    }
  }

  //enumerate vertices
  grid_function<typename gt::Vertex,int>  VNum(G);
  int vnum(1);
  typename gt::VertexIterator  vi( G.FirstVertex() );
  for ( ; !vi.IsDone(); ++vi, ++vnum )
    VNum[*vi] = vnum;

  //write METIS mesh file
  out << G.NumOfCells() << ' ' << ((num-3) ? 4 : 1) << "\n\n";
  for ( ci=G.FirstCell(); !ci.IsDone(); ++ci ) {
    typename gt::VertexOnCellIterator vc;
    for ( vc=(*ci).FirstVertex(); !vc.IsDone(); ++vc )
      out << VNum[*vc] << ' ';
    out << '\n';
  }

  return 0;
};


template<class GRID>
int metis_cell_graph_size_of_xadj(GRID const& G) { return G.NumOfCells(); };

template<class GRID>
int metis_cell_graph_size_of_adjncy(GRID const& G) {
 
  typedef grid_types<GRID>                gt;
  typedef typename gt::Cell               Cell;
  typedef typename gt::CellIterator       CellIterator;
  typedef typename gt::CellOnCellIterator CellOnCellIterator;
  
  CellIterator       c;
  CellOnCellIterator cc;

  int sum = 0;

  for (c = G.FirstCell(); !c.IsDone(); ++c) {
    int c_cnt = 0;
    for(cc = (*c).FirstCell(); !cc.IsDone(); ++cc,++c_cnt) {  }
    sum += c_cnt;
  };

  return sum;
};





template<class GRID>
extern void metis_cell_graph_format(GRID const& G, 
			      int * xadj,   int * vwgt, 
			      int * adjncy, int * adjwgt) {

  typedef grid_types<GRID> gt;
  typedef typename gt::Cell Cell;
  typedef typename gt::CellIterator CellIterator;
  typedef typename gt::CellOnCellIterator CellOnCellIterator;
  
  CellIterator c;
  CellOnCellIterator cc;

  int i,j;

  grid_function<Cell,int> numC(G);
  // enumerate cells
  int c_cnt = 0;
  for(c = G.FirstCell(); ! c.IsDone(); ++c, ++c_cnt) {
  numC[*c] = c_cnt;
  }

  xadj[0] = 0; i=1; vwgt[0]=1;
  for (c = G.FirstCell(); !c.IsDone(); ++c,i++) {

    int c_cnt = 0;
    for(cc = (*c).FirstCell(); !cc.IsDone(); ++cc,++c_cnt) {  }
    xadj[i] = xadj[i-1] + c_cnt;
    vwgt[i] = 1;
   };
  i=1;

  for (c = G.FirstCell(); !c.IsDone(); ++c,i++) {

    cc = (*c).FirstCell();
    for (j = xadj[i-1]; j < xadj[i]; ++cc, j++) {
      adjncy[j] = numC(*cc);
      adjwgt[j] = 1;
    };
  };
};

#endif
