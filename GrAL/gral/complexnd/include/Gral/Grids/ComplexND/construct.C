#ifndef GRAL_GB_COMPLEXND_CONSTRUCT_C
#define GRAL_GB_COMPLEXND_CONSTRUCT_C

#include "Gral/Grids/ComplexND/complexnd.h"
#include "Gral/Base/grid-morphism.h"

namespace complexnd  {

  template<class G_SRC, class PHI>
  void ConstructGrid0_0(ComplexND<0>& g, G_SRC const& g_src, PHI& phi) {
    typedef grid_types<ComplexND<0> > gt;
    typedef grid_types<G_SRC>         srcgt;
    /*
    g.incidences[0].resize(g_src.NumOfVertices());
    unsigned vcnt = 0;
    for(typename srcgt::VertexIterator v(g_src); ! v.IsDone(); ++v) {
      phi[v.handle()] = typename gt::vertex_handle(vcnt);
      ++vcnt;
    }
    */
  }

  template<int D, class G_SRC, class PHI>
  void ConstructGrid0(ComplexND<D>& g, G_SRC const& g_src, PHI& phi) {

    typedef grid_types<G_SRC>         srcgt;
    typedef grid_types<ComplexND<D> > gt;

    // copy vertices
    g.incidences[0].resize(g_src.NumOfVertices());
    unsigned vcnt = 0;
    for(typename srcgt::VertexIterator v(g_src); ! v.IsDone(); ++v) {
      phi[v.handle()] = typename gt::vertex_handle(vcnt);
      ++vcnt;
    }

    if(g.dimension() > 0) {
      // copy archetypes
      typedef vertex_morphism<typename srcgt::archetype_type, typename gt::archetype_type> arch_morphism;
      std::vector<arch_morphism>  morphism;
      for(typename srcgt::ArchetypeIterator a_src(g_src.BeginArchetype()); a_src != g_src.EndArchetype(); ++a_src) {
	typename gt::archetype_handle a = g.add_archetype();
	morphism.push_back(arch_morphism(*a_src,g.Archetype(a)));
	ConstructGrid0(g.Archetype(a), *a_src,  morphism.back());
      }
      
      // copy cells
      g.incidences[g.dimension()].resize(g_src.NumOfCells());
      for(typename srcgt::CellIterator c_src(g_src); ! c_src.IsDone(); ++c_src) {
	typename srcgt::archetype_handle a = g_src.archetype_of(c_src.handle());
	typename gt   ::cell_handle      c = g.add_cell(a);
	// phi[c] = c_src.handle();    
	g.incidences[g.dimension()][c].resize(g.dimension()+1);
	g.incidences[g.dimension()][c][0].resize(g_src.Archetype(a).NumOfVertices());
	typedef typename srcgt::archgt srcarchgt;
	typename srcarchgt::VertexIterator       lv    (g_src.Archetype(a));
	typename srcgt    ::VertexOnCellIterator vc_src(*c_src);        
	for( ; !vc_src.IsDone(); ) { // ++lv, ++vc_src) {
	  // morphism[a](*lv) is a vertex_handle of c's archetype
	  g.incidences[g.dimension()][c][0][morphism[a](lv.handle())] = phi(vc_src.handle());
	  ++lv;
	  ++vc_src;
	}
      }
      
      // calculate missing incidences
      g.calculate_incidences();
    }
  }


} // namespace complexnd


#endif
