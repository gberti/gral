#ifndef GRAL_GB_COMPLEX3D_CONSTRUCT_GRID_C
#define GRAL_GB_COMPLEX3D_CONSTRUCT_GRID_C

// $LICENSE_NEC


#include "Gral/Grids/Complex3D/construct-grid.h"

#include "Gral/Grids/Complex2D/construct.h"
#include "Gral/Grids/Complex2D/grid-functions.h"

#include "Gral/Base/grid-morphism.h"

#include "Container/bijective-mapping.h"

template<class G_SRC, class VCORR, class CCORR>
void ConstructGrid0(Complex3D     & G_dest,
		    G_SRC    const& G_src,
		    VCORR         & v_corr,
		    CCORR         & c_corr)
{
  // (0) Make G_dest an empty grid
  G_dest.clear();

  typedef grid_types<Complex3D> gt;
  typedef grid_types<G_SRC>     sgt;
  typedef           gt::archetype_type   dst_archetype;
  typedef typename sgt::archetype_type   src_archetype;
  typedef           gt::archetype_handle src_arch_handle;
  typedef typename sgt::archetype_handle dst_arch_handle;

  // (1) copy vertices
  for(typename sgt::VertexIterator v_src(G_src); ! v_src.IsDone(); ++v_src)
    v_corr[v_src.handle()] = G_dest.add_vertex();

  // (2) copy archetypes
  typedef vertex_morphism<src_archetype, dst_archetype> arch_morphism;
  std::vector<arch_morphism> morphism;
  bijective_mapping<src_arch_handle, dst_arch_handle> arch_corr;
  typename sgt::archetype_iterator arch_src = G_src.BeginArchetype(); 
  for(; arch_src != G_src.EndArchetype() ; ++arch_src) {
    dst_arch_handle a_dst = G_dest.add_archetype();
    src_arch_handle a_src = G_src.handle(arch_src);
    arch_corr[a_src] = a_dst; 
    morphism.push_back(arch_morphism(*arch_src,G_dest.Archetype(a_dst)));

    ConstructGrid0(G_dest.Archetype(a_dst),
		   G_src .Archetype(a_src),
		   morphism.back());
  }


  // (3) copy cells
  for(typename sgt::CellIterator c_src(G_src); ! c_src.IsDone(); ++c_src) {
    src_arch_handle a_src = G_src.archetype_of(c_src.handle());
    dst_arch_handle a_dst = arch_corr(a_src);

    gt::cell_handle c = G_dest.add_cell(a_dst);
    c_corr[c] = c_src.handle();    

    //    int lv = 0; // should be sgt::archetype_type::VertexIterator
    typename grid_types<src_archetype>::VertexIterator 
      lv(G_src.Archetype(a_src).FirstVertex());
    Complex3D::cell_vertex_incidence_map cv_incidence(G_dest,c);
    for(typename sgt::VertexOnCellIterator vc_src(*c_src); 
	!vc_src.IsDone(); ++lv, ++vc_src) {
      cv_incidence[morphism[a_src](lv.handle())] = v_corr[vc_src.handle()];
      
    }
  }
  G_dest.calculate_dependent();
   
}

#endif
