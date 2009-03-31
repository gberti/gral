#ifndef GRAL_GB_BASE_ARCHETYPE_MAP_H
#define GRAL_GB_BASE_ARCHETYPE_MAP_H


/* ------------------------------------------------------------

    Copyright (C) 2001 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include "Gral/Base/common-grid-basics.h"
#include "Gral/Base/partial-grid-morphism.h"
#include "Gral/Algorithms/construct-isomorphism.h"


#include "Container/bijective-mapping.h"

namespace GrAL {

/*! \brief Mapping between the archetypes of two grids
     
   \ingroup gridarchetypes

    This class holds isomorphisms between the archetypes of two grids of types \c G1 and \c G2.
 */
template<class G1, class G2>
class archetype_map 
{
  typedef grid_types<G1> gt1;
  typedef grid_types<G2> gt2;
 
  typedef typename gt1::archetype_handle archetype1_handle;
  typedef typename gt2::archetype_handle archetype2_handle;
  typedef typename gt1::archetype_type   archetype1_type;
  typedef typename gt2::archetype_type   archetype2_type;

  typedef typename gt1::Cell Cell1;
  typedef typename gt1::archetype_iterator Arch1Iterator;
  typedef typename gt2::archetype_iterator Arch2Iterator;

public:
  typedef partial_grid_morphism<archetype1_type, archetype2_type>  arch_isomorphism;

private:
  G1 const* g1;
  G2 const* g2;
  bijective_mapping<archetype1_handle, arch_isomorphism>  arch_map;
  bijective_mapping<archetype1_handle, archetype2_handle> arch_corr;
  bijective_mapping<archetype1_handle, bool>              mapping_found;
public:
  archetype_map(G1 const& gg1, G2 const& gg2)
    : g1(&gg1), g2(&gg2) 

    {
     for(Arch1Iterator a1(g1->BeginArchetype()); 
	 a1 != g1-> EndArchetype(); ++a1) {
       bool found = false;
       for(Arch2Iterator a2(g2->BeginArchetype()); 
	   a2 != g2-> EndArchetype(); ++a2) {
	 arch_isomorphism phi(*a1,*a2);
	 found = construct_isomorphism(*a1,*a2,phi);
	 if(found) {
	   arch_corr[g1->handle(a1)] = g2->handle(a2);
	   arch_map [g1->handle(a1)] = phi;
	   mapping_found[g1->handle(a1)] = true;
	   break;
	 }
       }
       if(! found)
	 mapping_found[g1->handle(a1)] = false;
     }
    }


  arch_isomorphism const& phi(Cell1 const& c) const 
    { 
      REQUIRE(mapping_exists(c), "no corresponding archetype!\n",1);
      return arch_map[g1->archetype_of(c)];
    }
  archetype2_handle   archetype_of(Cell1 const& c) const
    { 
      REQUIRE(mapping_exists(c), "no corresponding archetype!\n",1);
      return arch_corr[g1->archetype_of(c)];
    }

  archetype2_type  const&  ArchetypeOf(Cell1 const& c) const
    { 
      REQUIRE(mapping_exists(c), "no corresponding archetype!\n",1);
      return g2->Archetype(arch_corr[g1->archetype_of(c)]);
    }

  bool mapping_exists(Cell1 const& c) const 
    { return mapping_found[g1->archetype_of(c)];}

};

} // namespace GrAL 

#endif
