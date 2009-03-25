#ifndef GRAL_GB_GMV_FMT_OUTPUT_TT
#define GRAL_GB_GMV_FMT_OUTPUT_TT

/* ------------------------------------------------------------

    Copyright (C) 2001 - 2009 Guntram Berti,
    Contact: guntram AT users DOT berlios DOT de

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */

#include "Gral/IO/gmv-format-output.h"

namespace GrAL {

template<class GRID,class GEOM>
void ConstructGrid(OstreamGMV3DFmt& Out, 
		   GRID const& G,
		   GEOM const& GEO)
{
  typedef heterogeneous_list::END END;
  heterogeneous_list::List<END,END> L;
  ConstructGrid(Out,G,GEO,L);
}

template<class GRID,class GEOM, class GF, class MOREGFS>
void ConstructGrid(OstreamGMV3DFmt& Out, 
		   GRID const& G,
		   GEOM const& GEO,
		   heterogeneous_list::List<GF,MOREGFS> GFS)
{
  typedef OstreamGMV3DFmt GMV3D;
  std::ostream& out = Out.Out();
  typedef grid_types<GRID> gt;
  typedef grid_types<GMV3D::archetype_type> gmv_agt;

  out << "gmvinput ascii\n";
  out << "nodev " << G.NumOfVertices() << '\n';

  for(typename gt::VertexIterator v(G); ! v.IsDone(); ++v)
    out << GEO.coord(*v) << '\n';

  // map G's vertices to GMV numbers, starting from 1.
  element_numbering<typename gt::Vertex> G2GMV(G,1);

  // map archetypes of G to those of GMV
  typedef partial_grid_morphism<typename gt::archetype_type,
                                GMV3D::archetype_type>  
    morphism_type;
 
  bijective_mapping<typename gt::archetype_type const*,
                    std::pair<morphism_type, int> > 
    phi;

  for(typename gt::archetype_iterator a = G.BeginArchetype();
      a != G.EndArchetype(); ++a) {
    bool found = false;
    int cnt_a = 0; // could be GMV::archetype_handle
    for(GMV3D::archetype_iterator a_gmv = Out.BeginArchetype();
	a_gmv != Out.EndArchetype(); ++a_gmv, ++cnt_a) {
      morphism_type phi_a(*a,*a_gmv);
      found = construct_isomorphism(*a,*a_gmv, phi_a);
      if(found) {
	phi[&(*a)] = std::make_pair(phi_a,cnt_a);
	break;
      }
    }
    ENSURE(found, "sorry, no mapping to GMV cell type found!\n",1);
    // should use the general cell feature of GMV.
  }

  // map cells
  out << "cells " << G.NumOfCells() << "\n";
  for(typename gt::CellIterator c(G); ! c.IsDone(); ++c) {
    //            phi_a.inv        c.V()        G2GMV   
    // gmv-archetype -> G::archetype -> G::global -> GMV::global
    morphism_type const& phi_a = phi(&G.ArchetypeOf(*c)).first;
    int                  cnt_a = phi(&G.ArchetypeOf(*c)).second;
    GMV3D::archetype_type const& archetype_gmv(phi_a.ImgGrid());
    out << Out.name(cnt_a) << " "
	<< archetype_gmv.NumOfVertices() << " ";
    for(gmv_agt::VertexIterator vc(archetype_gmv); !vc.IsDone(); ++vc)
      out << G2GMV( (*c).V(phi_a.inverse()(*vc)))  << " ";
    out << '\n';
  }

  Out.copy_grid_functions(GFS);
  out << "endgmv\n";
}

} // namespace GrAL 

#endif
