#ifndef NMWR_GB_ENLARGE_GRID_COMPLEX2D_C
#define NMWR_GB_ENLARGE_GRID_COMPLEX2D_C


// $LICENSE

#include <map>  // STL

#include "Container/bijective-mapping.h"
#include "Container/set-primitives.h"
#include "Container/container-adapter.h"
#include "Container/function-adapter.h"

#include "Gral/Base/grid-functors.h"
#include "Gral/Base/mapped-iterators.h"
#include "Gral/Base/vtuple2d.h"

#include "Gral/Grids/Complex2D/enlarge.h"
#include "Gral/Grids/Complex2D/internal/copy.h"
#include "Gral/Grids/Complex2D/internal/adjacency.h"


template<class G2, class Geom2, class VertexIdent,
         class VertexMap, class CellMap>
extern
void
EnlargeGridVC(Complex2D        & destG,       // in/out
	      G2          const& srcG,        // in
	      Geom2       const& srcGeom,     // in
	      VertexIdent const& Iv,          // in  src -> dest
	      VertexMap        & VertexCorr,  // out src -> dest
	      CellMap          & CellCorr)    // out src -> dest
{ EnlargeGrid(destG,srcG,srcGeom,Iv,VertexCorr,CellCorr); }

template<class Conn, class Geom, class VertexIdent>
void EnlargeGrid(Complex2D& G,                 // in/out
		 const Conn& G_src,            // in
		 const Geom& Geo_src,          // in
		 const VertexIdent& Iv)        // in
{
  typedef grid_types<Conn>                      src_gt;
  typedef typename src_gt::vertex_handle        src_vertex_handle;
  typedef typename src_gt::cell_handle          src_cell_handle;
  typedef grid_types<Complex2D>                 gt;
  typedef typename gt::vertex_handle            vertex_handle;
  typedef typename gt::cell_handle              cell_handle;

  // hash<src_vertex_handle> has to be defined if it isnt a built-in integral type. 
  typedef bijective_mapping<src_vertex_handle,vertex_handle>  vertex_corr_map;                    
  vertex_corr_map VertexCorr(G_src.NumOfVertices());

  typedef bijective_mapping<src_cell_handle,cell_handle> cell_corr_map;
  //  typedef vector<cell_handle> cell_corr_map; 
 cell_corr_map CellCorr(G_src.NumOfCells());

  EnlargeGrid(G,G_src,Geo_src, Iv, VertexCorr, CellCorr);
}


//----------------------------------------------------------------
//
//  Outline of algorithm:
//
//  1) copy new cells to G
//  2) copy new vertices to G, but omit those in ran(Iv)
//     keep corr: V(G_src) --> V(G)
//  3) find facets in F(G) with V(F) \in dom(Iv), keep in FacetTable,
//  4) and remove them from boundary
//  5) calculate neighbors of new cells with FacetTable
//  6) and add remaining facets in FacetTable to boundary
//  7) calculate adjacent cells of new vertices
//
//----------------------------------------------------------------

template<class Conn, class Geom, class VertexIdent, class VertexMap, class CellMap>
void EnlargeGrid(Complex2D& G,                 // in/out
		 const Conn& G_src,            // in
		 const Geom& Geo_src,          // in
		 const VertexIdent& Iv,        // in: src->dest
		 VertexMap& VertexCorr,        // out
		 CellMap  & CellCorr)          // out
{

  //--- (0) import used types -----------------------

  typedef grid_types<Conn>                      src_gt;
  typedef typename src_gt::vertex_handle        src_vertex_handle;
  typedef typename src_gt::cell_handle          src_cell_handle;
  typedef typename src_gt::CellIterator         src_cell_it;
  typedef typename src_gt::VertexIterator       src_vertex_it;
  typedef typename src_gt::VertexOnCellIterator src_vertex_on_cell_it;

  typedef grid_types<Complex2D>             gt;
  typedef typename gt::vertex_handle        vertex_handle;
  typedef typename gt::cell_handle          cell_handle;
  typedef typename gt::Cell                 Cell;
  typedef typename gt::Facet                Facet;
  typedef typename gt::Vertex               Vertex;
  typedef typename gt::CellOnCellIterator   CellOnCellIterator;
  typedef typename gt::FacetOnCellIterator  FacetOnCellIterator;
  typedef typename gt::VertexOnCellIterator VertexOnCellIterator;
  typedef typename gt::CellOnVertexIterator CellOnVertexIterator;
  typedef typename gt::BoundaryFacetIterator BdFacetIterator;    

  friend_for_input gg(G);  // template routines cannot be friends,
                           // so we use a dirty trick


  //--- (1) construct vertices ---------------------------

  
  // cc._vertices().reserve(Connect.NumOfVertices());

  for(src_vertex_it src_v = G_src.FirstVertex(); ! src_v.IsDone(); ++src_v) {
    src_vertex_handle src_vh = G_src.handle(*src_v);
    if (! Iv.defined(src_vh)) // new vertex : create copy in G
      VertexCorr[src_vh] = gg._new_vertex(Geo_src.coord(*src_v));
    else // identified vertex, hence already in G
      VertexCorr[src_vh] = Iv(src_vh);
  }
  
  //--- (2) construct cells -------------------------------

  copy_cells(G,G_src, VertexCorr, CellCorr);

  //--- (3) set facet table --------------------------------

  typedef vtuple_2d<Complex2D> vtuple;
  typedef std::map<vtuple, FacetOnCellIterator, std::less<vtuple> > FacetTable;
  //typedef map<vtuple_2d, Facet, less<vtuple_2d> > FacetTable;
  FacetTable facet_id; // facets contained in Iv

  // range(Iv) \subset V_h(G) : vertices that are identified
  // with vertices of G_src.
  // We want to get the facets that have to be identified.
  typedef typename VertexIdent::range_type    range_type;
  typedef typename range_type::const_iterator range_iter;
  for(range_iter vi = Iv.range().begin(); vi != Iv.range().end(); ++vi) {
    // loop over all facets fcv of all cells cv adjacent to V,
    // and see if fcv is in the id-set.
    Vertex V = G.vertex(*vi);
    CellOnVertexIterator cv;
    for(cv = V.FirstCell(); ! cv.IsDone(); ++cv) {
      FacetOnCellIterator fcv; 
      for(fcv = (*cv).FirstFacet(); ! fcv.IsDone(); ++fcv) {
	vtuple vtf = get_vertices(fcv);
        if (is_subset(vtf.begin(),vtf.end(), Iv.range())) {
	  if( G.IsOnBoundary(fcv)) {
	    // NOTE: all vertices on boundary does not imply facet on boundary!
	    // cerr << "identfication facet not on boundary!\n";
	    // exit(1);
	    //}
	    facet_id[vtf] = fcv;
	  }
	  else {
	    cerr << "identfication facet not on boundary: vi = " 
		 << *vi << " cv = " << G.handle(*cv) << " fcv =  " << fcv << '\n';
	  }
	  //  facet_id[vtf] = *fcv;
	}
      }
    }
  }


  //--- (4) --- remove the found facets from boundary ---------------

  // note: STL remove does not really remove, but rather move the item
  // under question into the range [new_end,end), where new_end is the
  // return value of remove. Then container.erase(new_end,end) does the job.

  FacetOnCellIterator dummy;
  gg.boundary().
    erase(remove_if(gg.boundary().begin(),gg.boundary().end(), 
		    is_element_of(get2nd_c(facet_id.begin()),
				  get2nd_c(facet_id.end()),
				  &dummy)),
	  gg.boundary().end());
  

  /* gives internal compiler error with g++ 2.8 (in container-adapter.h)
  gg.boundary().
    erase(remove_if(gg.boundary().begin(),gg.boundary().end(), 
		    is_element_of(values_of(facet_id))),
	  gg.boundary().end());
  */ 

  //--- (5) calculate neighbors for new cells, using FacetId ----------
 
  //  The only "hairy" thing to do is to create a cell set on G from
  //  a cell set on G_src (G_src itself here).
  //  This is done by the "isomorphism" delivered by CellCorr.
  //
  //           F1                          F2               F3
  //       cell2handle(G_src)      o      CellCorr   o    handle2cell(G)
  // C(G_src) ---------------> C_h(G_src) -------> C_h(G) -------------> C(G)

  /* alternative code for the big call to CalculateNeighbourCells below
  typedef cell2handle_map<Conn>         F1;
  typedef unary_fct_ref<cell_corr_map > F2;
  typedef handle2cell_map<Complex2D>    F3;
  typedef unary_compose<F2,F1>          F21;
  typedef unary_compose<F3,F21>         F321;
  F321 f321(compose1(handle2cell(G),
		     compose1(make_unary_fct_ref(CellCorr), 
			      cell2handle(G_src))));

  typedef mapped_cell_sequence<Conn,F321>  CorrCells;
  CorrCells corr_cells(map_cell_sequence(G_src,f321));

  typedef typename CorrCells::CellIterator MappedCellIterator;

  CalculateNeighbourCells(G,corr_cells,facet_id);
  */
  
  CalculateNeighbourCells
    (G,
     map_cell_sequence // this is the seq of new cells in C(G) corr. to C(G_src)
     (G_src,  
      compose1(handle2cell(G), // C(G_src) --> C(G)
	       compose1(make_unary_fct_ref(CellCorr), // avoid copying CellCorr
			cell2handle(G_src)))
      ),
     facet_id);


  //--- (6) --- add facets to boundary ------------------

  // this does work only if iterator_traits are defined for mapped iterators,
  // which requires partial specialization. (ok with gcc 2.8 / egcs)
  // ( STL copy uses a dispatch on iterator_tag)
  /*
  copy(get2nd_c(facet_id.begin()),
       get2nd_c(facet_id.end()),
       back_inserter(gg.boundary()));
  */
  
  typedef typename FacetTable::const_iterator f_iter;
  for(f_iter bdf = facet_id.begin(); bdf != facet_id.end(); ++bdf)
    gg.boundary().push_back( (*bdf).second);
 

  //--- (7) --- calculate adjacent cells for vertices ----

  // loop over all new cells (by mapping C(G_src) --> C(G))
  for(src_cell_it src_c = G_src.FirstCell(); ! src_c.IsDone(); ++src_c) {
    // append cell C to all adjacent vertices
    Cell C =   G.cell(CellCorr(G_src.handle(*src_c)));
    for(VertexOnCellIterator vc = C.FirstVertex(); !vc.IsDone(); ++vc)
       gg.add_cell_on_vertex(*vc,C);
  }

  //  This may be done here, or outside if many subsequent changes to G.
  //  gg.notify_observers()
  //  or:
  //  gg.end_modification()
}


#endif
