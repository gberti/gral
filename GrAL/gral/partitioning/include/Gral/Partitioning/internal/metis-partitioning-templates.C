#ifndef NMWR_GB_METIS_PARTITIONING_TEMPLATES_C
#define NMWR_GB_METIS_PARTITIONING_TEMPLATES_C

// $LICENSE

/*! \file
 */


#include "Gral/Partitioning/metis-partitioning.h"
#include "Gral/Partitioning/grid-to-metis.h"

#include "Gral/Partitioning/internal/metis-wrap.h"
#include "Gral/Partitioning/internal/metis-options.h"

template<class GRID, class PARTITIONING>
void MetisPartitioning::calculate_cell_partitioning
(GRID         const& G, //<! IN
 PARTITIONING      & P, //<! OUT
 int               np)  //<! IN
{
   typedef GRID                          grid_type;
   typedef grid_types<grid_type>         gt;
   typedef typename gt::CellIterator     CellIterator;


  if (np < 2) return;

  int n = metis_cell_graph_size_of_xadj(G);

  int *xadj = new int[n+1];
  int *vwgt = new int[n+1];

  int m = metis_cell_graph_size_of_adjncy(G);
  
  int *adjncy = new int[m];
  int *adjwgt = new int[m];

  metis_cell_graph_format (G,  xadj,   vwgt,   adjncy, adjwgt);

  int wgtflag = 0; // no weights        (input)
  int numflag = 0; // C-style numbering (input)
  int edgecut = 0; // # of edgecuts (output)
  int *part = new int[n]; // partitioning (output)

  if(options->algorithm_flag == 2) 
    if (np <= 8)
      options->use_pmetis();
    else
      options->use_kmetis();

  if (options->algorithm_flag == 0) {
    
    metis_partGraphRecursive(n, xadj, adjncy, vwgt,
                             adjwgt, wgtflag, numflag,  np, 
                             options->opt, edgecut, part);
  }

  else // (options->algorithm_flag == 1)
    metis_partGraphKway(n, xadj, adjncy, vwgt,
                        adjwgt, wgtflag, numflag, np, 
                        options->opt, edgecut, part);

  int i=0;
  for(CellIterator c(G); !c.IsDone(); ++c, ++i) {
    P[*c] = part[i];
  }


}

#endif
