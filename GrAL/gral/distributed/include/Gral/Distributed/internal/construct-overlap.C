#ifndef NMWR_GB_CONSTRUCT_OVERLAP_C
#define NMWR_GB_CONSTRUCT_OVERLAP_C

// $LICENSE



#include "Gral/Distributed/construct-overlap.h"

#include "Gral/Distributed/internal/add-total-shared.h"
#include "Gral/Distributed/internal/add-partial-shared.h"
#include "Gral/Distributed/internal/add-total-copied.h"
#include "Gral/Distributed/internal/add-partial-copied.h"


template<class Overlap, class CoarseGrid, 
         class Partition, class OVPattern,
         class Part2Cell, class Cell2Part,
         class GridMap>
void ConstructOverlap(Overlap          & Ovlp,         // out
		      CoarseGrid  const& CrsG,         
		      Partition   const& Prtng,
		      OVPattern   const& ovlp_pattern,
		      Part2Cell   const& part2cell,
		      Cell2Part   const& cell2part,
		      GridMap     const& grid_map)

{
  typedef grid_types<CoarseGrid>                cgt;
  typedef typename cgt::Cell                    CoarseCell;
  typedef typename cgt::CellIterator            CoarseCellIterator;
  
  typedef typename Partition::grid_type         fine_grid_type;
  //  typedef grid_types<fine_grid_type>            fgt;
  // WORKAROUND: gcc 2.95 does not understand the above defn for fgt!
  typedef typename Partition::gt                fgt;
  typedef typename fgt::Vertex                  Vertex;
  typedef typename fgt::Facet                   Facet;
  typedef typename fgt::Cell                    Cell;

  for(CoarseCellIterator P = CrsG.FirstCell(); ! P.IsDone(); ++P) {
    AddTotalSharedRanges(Ovlp[*P].vertices().shared(),
			 Ovlp[*P].facets  ().shared(),
			 Prtng, 
			 cell2part(*P), 
			 Prtng.TheGrid());
  }

  for(CoarseCellIterator P0 = CrsG.FirstCell(); ! P0.IsDone(); ++P0) {
    AddPartialSharedRanges(Ovlp, 
			   Prtng, cell2part(*P0),
			   part2cell,  
			   Ovlp[*P0].vertices().shared(), 
			   Ovlp[*P0].facets  ().shared(), 
			   grid_map,  // may be identity<Vertex>()
			   grid_map); // may be identity<Facet>()
  }
 
  for(CoarseCellIterator P1 = CrsG.FirstCell(); ! P1.IsDone(); ++P1) {
    AddTotalRanges(Ovlp[*P1], 
		   Prtng, InPartition(cell2part(*P1),Prtng),
		   ovlp_pattern,
		   Ovlp[*P1].vertices().shared(), 
		   Ovlp[*P1].facets  ().shared());
  }

  for(CoarseCellIterator P2 = CrsG.FirstCell(); ! P2.IsDone(); ++P2) {
    AddPartialRanges(Ovlp,
		     Prtng, cell2part(*P2),
		     part2cell,
		     Ovlp[*P2].vertices().copied(),
		     Ovlp[*P2].cells   ().copied(),
		     grid_map,  // may be identity<Vertex>()
		     grid_map); // may be identity<Cell>()
  }
}

#endif
