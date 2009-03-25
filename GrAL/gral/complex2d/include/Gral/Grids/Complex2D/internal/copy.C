#ifndef GRAL_GB_COMPLEX2D_INTERNAL_COPY_CELLS_C
#define GRAL_GB_COMPLEX2D_INTERNAL_COPY_CELLS_C



/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti, 
    Contact: guntram AT users DOT berlios DOT de

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include "Gral/Grids/Complex2D/internal/copy.h"
#include "Gral/Base/grid-morphism.h"

#include "Container/bijective-mapping.h"
#include <vector>


namespace GrAL {

template<class Conn, class VtxCorr, class CellCorr>
void copy_cells_no_archetypes(Complex2D& G, 
			      const Conn& G_src,
			      /* const */ VtxCorr&  vtx_corr,
			      CellCorr&       cell_corr)
{
  typedef grid_types<Conn>                      src_gt;
  typedef typename src_gt::CellIterator         src_cell_it;
  typedef typename src_gt::VertexOnCellIterator src_vertex_on_cell_it;

  typedef grid_types<Complex2D>     gt;
  typedef typename  gt::Cell        Cell;
  typedef typename  gt::cell_handle cell_handle;
  //typedef typename gt::BoundaryFacetIterator BdFacetIterator;    

  friend_for_input cc(G);

  cell_handle boundary(cc.outer_cell_handle()); // invalid reference to a cell
                                                // (used to mark unknown neighbours)
  for(src_cell_it src_c = G_src.FirstCell(); ! src_c.IsDone(); ++src_c){
    cell_handle pc = cc._new_cell((*src_c).NumOfVertices());
    cell_corr[src_c.handle()] = pc;

    typedef cell2d_connectivity c2d;
    typedef c2d::cell_list   cell_list;
    typedef c2d::vertex_list vertex_list;
    typename cell_list::iterator   ln(cc._cell_neighbours(pc).begin()); 
    typename vertex_list::iterator lv(cc._cell_vertices(pc).begin());
    src_vertex_on_cell_it src_lv((*src_c).FirstVertex());
    for(; ! src_lv.IsDone(); ++src_lv) {
      (*lv) = vtx_corr[src_lv.handle()];
      (*ln) = boundary; // correct neighbour is calculated later
      ++lv;
      ++ln;
    }

  }

}

template<class GSRC, class VtxCorr, class CellCorr>
void copy_cells_archetypes(Complex2D      & G_dst, 
			   GSRC      const& G_src,
			   VtxCorr&         vtx_corr,
			   CellCorr&        cell_corr)
{
  typedef grid_types<GSRC>                      src_gt;
  typedef typename src_gt::CellIterator         src_cell_it;
  typedef typename src_gt::VertexOnCellIterator src_vertex_on_cell_it;
  
  typedef grid_types<Complex2D>     gt;
  typedef           gt::Cell        Cell;
  typedef           gt::cell_handle cell_handle;

  typedef              gt::archetype_type   dst_archetype;
  typedef typename src_gt::archetype_type   src_archetype;
  typedef              gt::archetype_handle src_arch_handle;
  typedef typename src_gt::archetype_handle dst_arch_handle;


  //typedef typename gt::BoundaryFacetIterator BdFacetIterator;    

  friend_for_input cc(G_dst);

  // (2) copy archetypes
  typedef vertex_morphism<src_archetype, dst_archetype> arch_morphism;
  ::std::vector<arch_morphism> morphism;
  bijective_mapping<src_arch_handle, dst_arch_handle> arch_corr;
  typename src_gt::archetype_iterator arch_src = G_src.BeginArchetype();
  for(; arch_src != G_src.EndArchetype() ; ++arch_src) {
    //  dst_arch_handle a_dst = cc.add_archetype(); //G_dst.add_archetype();
    src_arch_handle a_src = G_src.handle(arch_src);

    dst_archetype  arch_dst;
    dst_arch_handle a_dst = cc.add_archetype(arch_dst, (*arch_src).NumOfVertices());
    morphism.push_back(arch_morphism(*arch_src,G_dst.Archetype(a_dst)));

    ConstructGrid0(G_dst.Archetype(a_dst),
                   G_src.Archetype(a_src),
                   morphism.back());
    arch_corr[a_src] = a_dst;
  }


  for(typename src_gt::CellIterator c_src = G_src.FirstCell(); ! c_src.IsDone(); ++c_src){
    src_arch_handle a_src = G_src.archetype_of(*c_src);
    //  dst_arch_handle a_dst = arch_corr(a_src);

    cell_handle pc = cc._new_cell(GrAL::size<typename src_gt::Vertex>(*c_src)); // correct arch gets selected automatically a_dst);
    cell_corr[c_src.handle()] = pc;
    gt::Cell c_dst(G_dst, pc);

    typedef cell2d_connectivity c2d;
    c2d::vertex_list&   verts (cc._cell_vertices(pc));
 
    src_vertex_on_cell_it vc_src((*c_src)); // .FirstVertex());
    typename grid_types<src_archetype>::VertexIterator
       lv_src(G_src.Archetype(a_src).FirstVertex());
    for(; ! vc_src.IsDone(); ++vc_src, ++lv_src) {
      int lv = morphism[a_src][lv_src.handle()];
      verts [lv] = vtx_corr[vc_src.handle()];
    }


    // initialize cell neighbors
    cell_handle boundary(cc.outer_cell_handle()); // invalid reference to a cell
                                                // (used to mark unknown neighbours)
    c2d::cell_list&  neighb(cc._cell_neighbours(pc));
    for(int ln = 0; ln < c_dst.NumOfFacets(); ++ln)
      neighb[ln] = boundary; // correct neighbour is calculated later
  }

}

} // namespace GrAL 

#endif
