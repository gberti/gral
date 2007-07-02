#ifndef GRAL_VTK_UGRID_ADAPTER_CONSTRUCT_GRID_C
#define GRAL_VTK_UGRID_ADAPTER_CONSTRUCT_GRID_C

// $LICENSE_CUT_NEC_2007


/*! \file
 */

#include "Gral/Adapters/VTK/vtk-ugrid-adapter-construct.h"

#include "Gral/Base/grid-morphism.h"
#include "Gral/Base/archetype-map.h"

namespace GrAL {
namespace vtk_ugrid {
template<int D, class G_SRC, class VCORR, class CCORR>
void ConstructGrid0_dispatched(UGridVTKAdapter<D>  & G_dest,
		    G_SRC    const& G_src,
		    VCORR         & v_corr,
		    CCORR         & c_corr)
{
  // (0) Make G_dest an empty grid
  G_dest.clear();
  G_dest.allocate(G_src.NumOfVertices(), G_src.NumOfCells());
  std::cerr << "allocating " << G_src.NumOfVertices() << " " << G_src.NumOfCells() <<"\n";
  typedef grid_types<UGridVTKAdapter<D> > gt;
  typedef grid_types<G_SRC>     sgt;
  typedef typename gt::archetype_type   dst_archetype;
  typedef typename sgt::archetype_type   src_archetype;
  typedef typename gt::archetype_handle  src_arch_handle;
  typedef typename sgt::archetype_handle dst_arch_handle;

  // (1) copy vertices
  for(typename sgt::VertexIterator v_src(G_src); ! v_src.IsDone(); ++v_src) {
    v_corr[v_src.handle()] = G_dest.add_vertex();
  }
  // map archetypes of G_SRC to those of UGridAdapter
  archetype_map<G_SRC,UGridVTKAdapter<D> > a_map(G_src, G_dest);

  typedef typename archetype_map<G_SRC,UGridVTKAdapter<D> >::arch_isomorphism morphism_type;

  // archetype grid type
  typedef grid_types<dst_archetype> vtk_agt;

  // map cells 
  for(typename sgt::CellIterator c(G_src); ! c.IsDone(); ++c) {
    morphism_type const& phi_a = a_map.phi(*c);

    dst_archetype const& archetype_vtk(phi_a.ImgGrid());

    vtkIdType pts[100];
    int i=0;
    for(typename vtk_agt::VertexIterator vc(archetype_vtk); 
                                        !vc.IsDone(); ++i, ++vc) {
      //FIXME: some "wrapped" types have no V() or maybe the wrong type
      // (enumerated_subrange<Grid> or the like).
      // perhaps arch2grid(Cell,arch_vertex) ?
      typename sgt::Vertex v_src(G_src, (*c).v(phi_a.inverse()(*vc).handle()));
      pts[i] = v_corr( v_src ).handle();
    }  
    int vtktype = UGridVTKArchetypes<D>::Get().vtk_type(a_map.archetype_of(*c));
    typename gt::cell_handle dstc = G_dest.GetAdaptee()->InsertNextCell(vtktype,i, pts);
    c_corr[(*c).handle()] = dstc; 
  }
  G_dest.calculate_dependent();
}

template<int D>
 struct dispatch {
  template<class G_SRC, class VCORR, class CCORR>
   static void construct(UGridVTKAdapter<D>& g, G_SRC const& g_src, VCORR& v_corr, CCORR& c_corr)
    { ConstructGrid0_dispatched(g,g_src,v_corr, c_corr);}
  };
} // namespace vtk_ugrid

template<int D, class G_SRC, class VCORR, class CCORR>
void ConstructGrid0(vtk_ugrid::UGridVTKAdapter<D>& g, G_SRC const& g_src, VCORR& v_corr, CCORR& c_corr)
{ vtk_ugrid::dispatch<D>::construct(g,g_src, v_corr, c_corr);}

} // namespace GrAL 

#endif
