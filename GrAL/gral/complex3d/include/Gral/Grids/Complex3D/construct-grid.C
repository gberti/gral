#ifndef GRAL_GB_COMPLEX3D_CONSTRUCT_GRID_C
#define GRAL_GB_COMPLEX3D_CONSTRUCT_GRID_C

// $LICENSE_NEC


#include "Gral/Grids/Complex3D/construct-grid.h"

#include "Gral/Grids/Complex2D/construct.h"
#include "Gral/Grids/Complex2D/grid-functions.h"

#include "Gral/Base/grid-morphism.h"


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

  // (1) copy vertices
  for(typename sgt::VertexIterator v_src(G_src); ! v_src.IsDone(); ++v_src)
    v_corr[v_src.handle()] = G_dest.add_vertex();

  // (2) copy archetypes
  typedef typename  gt::archetype_type arch_dest_t; 
  typedef typename sgt::archetype_type arch_src_t;
  typedef vertex_morphism<arch_src_t, arch_dest_t> arch_morphism;
  std::vector<arch_morphism> morphism;
  typename sgt::archetype_iterator arch_src = G_src.BeginArchetype(); 
  for(; arch_src != G_src.EndArchetype() ; ++arch_src) {
    gt::archetype_handle a = G_dest.add_archetype();
    morphism.push_back(arch_morphism(*arch_src,G_dest.Archetype(a)));

    ConstructGrid0(G_dest.Archetype(a),
		   *arch_src,
		   morphism.back());
  }

  // (3) copy cells
  for(typename sgt::CellIterator c_src(G_src); ! c_src.IsDone(); ++c_src) {
    gt::archetype_handle a = G_src.archetype_of(c_src.handle());
    gt::cell_handle c = G_dest.add_cell(a);
    c_corr[c] = c_src.handle();    

    int lv = 0; // should be sgt::archetype_type::VertexIterator
    Complex3D::cell_vertex_incidence_map cv_incidence(G_dest,c);
    for(typename sgt::VertexOnCellIterator vc_src(*c_src); 
	!vc_src.IsDone(); ++lv, ++vc_src) {
      cv_incidence[morphism[a](lv)] = v_corr(vc_src.handle());
      
    }
  }
  G_dest.calculate_dependent();
   
}

#endif
