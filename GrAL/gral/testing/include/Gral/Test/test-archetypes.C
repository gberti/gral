#ifndef NMWR_GB_GRAL_TEST_ARCHETYPES_C
#define NMWR_GB_GRAL_TEST_ARCHETYPES_C


/* ------------------------------------------------------------

    Copyright (C) 2001 - 2009 Guntram Berti, 
    Contact: guntram AT users DOT berlios DOT de

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include "Gral/Test/test-archetypes.h"
#include "Utility/pre-post-conditions.h"
#include "Container/partial-mapping.h"

namespace GrAL {

template<class G>
bool test_archetypes(G const& g, ::std::ostream & out)
{
  return test_archetypes(g,out, grid_types<G>());
}

template<class G, class GT>
bool test_archetypes(G const& g, ::std::ostream & out, GT)
{
  typedef typename GT::archetype_type     archetype_type;
  typedef typename GT::archetype_handle   archetype_handle;
  typedef typename GT::archetype_iterator archetype_iterator;
  typedef typename get_archgt<GT, grid_types<archetype_type> >::type agt;
  
  REQUIRE_ALWAYS((g.NumOfCells() == 0 || g.NumOfArchetypes() > 0), "",1);

  archetype_type A;
  for(archetype_iterator a = g.BeginArchetype(); a != g.EndArchetype(); ++a)
    A = *a;
  
  partial_mapping<archetype_handle, unsigned> narch(0);

  for(typename GT::CellIterator c(g.FirstCell()); ! c.IsDone(); ++c) {
    archetype_type   A  = g.ArchetypeOf(*c);
    REQUIRE_ALWAYS(GrAL::size<typename agt::Vertex>(A) == GrAL::size<typename GT::Vertex>(*c),
		   "A.NumOfVertices()="  
		   << GrAL::size<typename agt::Vertex>(A)
		   << "  (*c).NumOfVertices()="   
		   << GrAL::size<typename GT::Vertex>(*c), 1);

    archetype_handle a  = g.archetype_of(*c);
    archetype_type   A1 = g.Archetype(a);
    REQUIRE_ALWAYS(GrAL::size<typename agt::Vertex>(A) == GrAL::size<typename agt::Vertex>(A1), "", 1);
    narch[a]++;
  }
  REQUIRE_ALWAYS(narch.size() == g.NumOfArchetypes(), 
		 "narch.size()=" << narch.size() << " g.NumOfArchetypes()=" << g.NumOfArchetypes(), 1);

  return true;
}

} // namespace GrAL 

#endif
