#ifndef NMWR_GB_GRAL_TEST_ARCHETYPES_H
#define NMWR_GB_GRAL_TEST_ARCHETYPES_H

// $LICENSE_NEC

/*! \file
    
    Test grid archetype functionality.
*/

#include <iostream>
#include "Gral/Base/common-grid-basics.h"

#ifdef NMWR_INCLUDE_TEMPLATE_DEFS
#include "Gral/Test/test-archetypes.C"
#else

/*! \brief Test grid archetype functionality

    \templateparams
      - \c G is a model of $GrAL ArchetypedGrid.

    \ingroup  archetypestests
 */

template<class G>
bool test_archetypes(G const& g, std::ostream & out);

/*! \brief Test grid archetype functionality
 
    \templateparams
      - \c G is a model of $GrAL ArchetypedGrid.
      - \c GT is  model of $GrAL GridTypes
           and can be used to specify e.g. enhanced grid types.

    \ingroup  archetypestests
 */
template<class G, class GT>
bool test_archetypes(G const& g, std::ostream & out, GT);


#endif

#endif
