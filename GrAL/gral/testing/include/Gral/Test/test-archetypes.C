#ifndef NMWR_GB_GRAL_TEST_ARCHETYPES_C
#define NMWR_GB_GRAL_TEST_ARCHETYPES_C

// $LICENSE_NEC

#include "Gral/Test/test-archetypes.h"
#include "Utility/pre-post-conditions.h"
#include "Container/partial-mapping.h"

template<class G>
bool test_archetypes(G const& g, std::ostream & out)
{
  return test_archetypes(g,out, grid_types<G>());
}

template<class G, class GT>
bool test_archetypes(G const& g, std::ostream & out, GT)
{
  typedef typename GT::archetype_type     archetype_type;
  typedef typename GT::archetype_handle   archetype_handle;
  typedef typename GT::archetype_iterator archetype_iterator;

  REQUIRE_ALWAYS((g.NumOfCells() == 0 || g.NumOfArchetypes() > 0), "",1);

  archetype_type A;
  for(archetype_iterator a = g.BeginArchetype(); a != g.EndArchetype(); ++a)
    A = *a;
  
  partial_mapping<archetype_handle, unsigned> narch(0);

  for(typename GT::CellIterator c(g.FirstCell()); ! c.IsDone(); ++c) {
    archetype_type   A  = g.ArchetypeOf(*c);
    REQUIRE_ALWAYS((unsigned)A.NumOfVertices() == (unsigned)(*c).NumOfVertices(), 
		   "A.NumOfVertices()=" << A.NumOfVertices() << "  (*c).NumOfVertices()="   << (*c).NumOfVertices(), 1);
    archetype_handle a  = g.archetype_of(*c);
    archetype_type   A1 = g.Archetype(a);
    REQUIRE_ALWAYS(A.NumOfVertices() == A1.NumOfVertices(), "", 1);
    narch[a]++;
  }
  REQUIRE_ALWAYS(narch.size() == g.NumOfArchetypes(), 
		 "narch.size()=" << narch.size() << " g.NumOfArchetypes()=" << g.NumOfArchetypes(), 1);

  return true;
}


#endif
