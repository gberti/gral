#ifndef GRAL_GB_COMPLEXND_CONSTRUCT_C
#define GRAL_GB_COMPLEXND_CONSTRUCT_C

/*! \file

*/

/* ------------------------------------------------------------

    Copyright (C) 2001 - 2009 Guntram Berti,
    Contact: guntram AT users DOT berlios DOT de

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */

#include "Gral/Grids/ComplexND/complexnd.h"
#include "Gral/Base/grid-morphism.h"
#include "Gral/Base/construct-grid.h"

namespace GrAL {

namespace complexnd  {

  template<class G_SRC, class PHI>
  void ConstructGrid0_0(ComplexND<0>& g, G_SRC const& g_src, PHI& phi) {
    typedef grid_types<ComplexND<0> > gt;
    typedef grid_types<G_SRC>         srcgt;
    
    g.incidences[0].resize(g_src.NumOfVertices());
    unsigned vcnt = 0;
    for(typename srcgt::VertexIterator v(g_src); ! v.IsDone(); ++v) {
      phi[v.handle()] = typename gt::vertex_handle(vcnt);
      ++vcnt;
    }
   
  }


  template<int D, class G_SRC, class VCORR, class CCORR>
  void ConstructGrid0_dispatched(ComplexND<D>& g, G_SRC const& g_src, VCORR& v_corr, CCORR& c_corr) {

    typedef grid_types<G_SRC>         srcgt;
    typedef grid_types<ComplexND<D> > gt;

    // copy vertices
    g.incidences[0].resize(g_src.NumOfVertices());
    unsigned vcnt = 0;
    for(typename srcgt::VertexIterator v_src(g_src.FirstVertex()); ! v_src.IsDone(); ++v_src) {
      v_corr[v_src.handle()] = typename gt::vertex_handle(vcnt);
      ++vcnt;
    }

    if(g.dimension() > 0) {
      // copy archetypes
      typedef vertex_morphism<typename srcgt::archetype_type, typename gt::archetype_type> arch_morphism;
      std::vector<arch_morphism>  morphism;
      for(typename srcgt::archetype_iterator a_src(g_src.BeginArchetype()); a_src != g_src.EndArchetype(); ++a_src) {
	typename gt::archetype_handle a = g.add_archetype();
	morphism.push_back(arch_morphism(*a_src,g.Archetype(a)));
	ConstructGrid0(g.Archetype(a), *a_src,  morphism.back());
      }
      
      // copy cells
      g.incidences[g.dimension()].resize(g_src.NumOfCells());
      for(typename srcgt::CellIterator c_src(g_src.FirstCell()); ! c_src.IsDone(); ++c_src) {
	typename srcgt::archetype_handle a = g_src.archetype_of(*c_src);
	typename gt   ::cell_handle      c = g.add_cell(a);
	c_corr[c_src.handle()] = c;
	g.incidences[g.dimension()][c].resize(g.dimension()+1);
	g.incidences[g.dimension()][c][0].resize(g_src.Archetype(a).NumOfVertices());
	typedef grid_types<typename srcgt::archetype_type> srcarchgt;
	typename srcarchgt::VertexIterator       lv    (g_src.Archetype(a).FirstVertex());
	typename srcgt    ::VertexOnCellIterator vc_src(*c_src);        
	for( ; !vc_src.IsDone(); ) { // ++lv, ++vc_src) {
	  // morphism[a](*lv) is a vertex_handle of c's archetype
	  g.incidences[g.dimension()][c][0][morphism[a](lv.handle())] = v_corr(vc_src.handle());
	  ++lv;
	  ++vc_src;
	}
      }
      
      // calculate missing incidences
      g.calculate_incidences();
    }
  }


  // force choice of correct specialization - partial ordering of functions unreliable
  template<int D>
  struct dispatch {
    template<class G_SRC, class VCORR, class CCORR>
    static void construct(ComplexND<D>& g, G_SRC const& g_src, VCORR& v_corr, CCORR& c_corr)
    { ConstructGrid0_dispatched(g,g_src,v_corr, c_corr);}
  };

  template<>
  struct dispatch<0> {
    template<class G_SRC, class PHI>
    static void construct(ComplexND<0>& g, G_SRC const& g_src, PHI & phi) 
    { ConstructGrid0_0(g,g_src,phi);}
    template<class G_SRC, class VCORR, class CCORR>
    static void construct(ComplexND<0>& g, G_SRC const& g_src, VCORR& v_corr, CCORR& c_corr)
    { ConstructGrid0_0(g,g_src,v_corr);}
  };


} // namespace complexnd

/*! \defgroup complexndcopy Generic copy operations for complexnd::ComplexND
    \ingroup complexndstuff

    \see $GrAL ConstructGrid
    \see Gral base module \ref copyoperations
*/

// here  namespace  GrAL

/*! \brief Copy any grid type to complexnd::ComplexND<>

    This function copies only the combinatorial information.
    \ingroup complexndcopy
*/
template<int D, class G_SRC, class VCORR, class CCORR>
void ConstructGrid0(complexnd::ComplexND<D>& g, G_SRC const& g_src, VCORR& v_corr, CCORR& c_corr)
{ complexnd::dispatch<D>::construct(g,g_src, v_corr, c_corr);}


/*! \brief Copy any grid type to complexnd::ComplexND<>

    This function copies combinatorial and geometric information.

   \ingroup complexndcopy
*/
template<int D, class GEOM_DEST,
	 class G_SRC,  class GEOM_SRC,
	 class VCORR,  class CCORR>
void ConstructGridVC (complexnd::ComplexND<D> & G_dest,
		      GEOM_DEST               & Geom_dest,
		      G_SRC              const& G_src,
		      GEOM_SRC           const& Geom_src,
		      VCORR                   & v_corr,
		      CCORR                   & c_corr)
{ ConstructGridVC_generic(G_dest, Geom_dest, G_src, Geom_src, v_corr, c_corr);}

} // namespace GrAL 

#endif
