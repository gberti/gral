#ifndef NMWR_GB_CONSTRUCT_OVERLAPPING_GRIDS_C
#define NMWR_GB_CONSTRUCT_OVERLAPPING_GRIDS_C


// $LICENSE

#include "Gral/Distributed/construct-local-overlapping-grids.h"

#include "Gral/Distributed/copy-overlap.h"
#include "Gral/Distributed/mark-ranges.h"

#include "Container/functions.h"
#include "Gral/Subranges/enumerated-subrange.h"

#include "Gral/Base/enlarge-grid.h"
#include "Gral/Base/construct-grid.h"



//----------------------------------------------------------------
//
// semantics:
// ovlp_grid.TheGrid()    = Grid(Cells(owned \cup ovlp.copied()))
// ovlp_grid.TheOverlap() = ovlp
//
//----------------------------------------------------------------

template<class OvlpGrid, class CellRange, class Geometry,
         class Overlap, class VertexCorr, class CellCorr>
void ConstructLocalOverlappingGrid(OvlpGrid       & ovlp_grid,
				   CellRange const& owned_c,
				   Geometry  const& geom, 
				   Overlap        & ovlp,
				   VertexCorr     & v_corr,
				   CellCorr       & c_corr)
{

  typedef typename CellRange::grid_type grid_type;

  enumerated_subrange<grid_type> owned;
  ConstructSubrangeFromCells(owned,owned_c.FirstElement());

  // Fixme!!
  Geometry dummy(ovlp_grid.TheGrid());
  ConstructGridVC(ovlp_grid.TheGrid(),
		  //	    SubrangeFromCells(owned_c),
                  dummy,
		  owned,
		  geom,  
		  v_corr,   // global -> local
		  c_corr);  // global -> local
 
  enumerated_subrange<grid_type> copied;
  ConstructSubrangeFromCells(copied, ovlp.cells().copied().FirstElement());

  // construct vertex identification by shared vertices
  typedef grid_types<grid_type>                  gt;
  typedef typename gt::vertex_handle             vertex_handle;
  bijective_mapping<vertex_handle,vertex_handle> id_shared;
  typedef typename Overlap::vertex_range_type         vrange;
  typedef typename vrange::const_iterator        v_range_iter;
  for(v_range_iter shv  = ovlp.vertices().shared().begin(); 
                   shv != ovlp.vertices().shared().end(); ++shv)
    id_shared[*shv] = v_corr(*shv);

  EnlargeGridVC(ovlp_grid.TheGrid(),
		//	      SubRangeFromCells(ovlp.cells().copied()),
		copied,
		geom,
		//	      restriction_map(v_corr, ovlp.vertices().shared()), // identify shared 
		id_shared,
		v_corr,  // v_corr remains unchanged on shared vertices
		c_corr);

  ovlp_grid.fine_grid_complete();

  //--------------------- copy the overlap ranges -------------------------------

  // make private ranges
  mark_private_range(ovlp.cells().privee(), owned_c.FirstCell(),
		     ovlp.cells().exposed(),ovlp.cells().shared(), ovlp.cells().copied());
  mark_private_range(ovlp.vertices().privee(), VerticesOfCells(owned_c).FirstVertex(),
		     ovlp.vertices().exposed(),ovlp.vertices().shared(), ovlp.vertices().copied());

  //-- not necessary:  ovlp_grid.init_overlap();

  typedef typename Overlap::CoarseCell CoarseCell;
  typedef typename Overlap::coarse_grid_type coarse_grid_type;
  typedef typename grid_types<coarse_grid_type>::cell_handle coarse_cell_handle;
  // semantics: ovlp_grid.TheOverlap() = ovlp  mod  global --> local ({v,c}_corr)
  CopyOverlap(ovlp_grid.TheOverlap(),
	      ovlp,
              ovlp_grid.TheCoarseGrid(), 
	      identity<coarse_cell_handle>(),   // ovlp.TheCoarse() == ovlp_grid.TheCoarse()
              ovlp_grid.TheGrid(),      // ovlp_grid.TheGrid() == local grid,
	                                // ovlp     .TheGrid() == global_grid
	      v_corr,                   // ovlp.TheGrid() --> ovlp_grid.TheGrid()
	      c_corr);                  // (   global     -->  local  )
}                                   


#endif
