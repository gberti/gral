#ifndef GRAL_GB_TEST_PRIMITIVES_H
#define GRAL_GB_TEST_PRIMITIVES_H


/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include <iostream>

namespace GrAL {

/*! \brief Test dimension-independent primitives.

    POINT must be capable of representing a point 
    with exactly 3 components.
 */
template<class POINT>
struct test_algebraic_primitives 
{
  void do_tests(::std::ostream & out);
};

} // namespace GrAL 


#ifdef NMWR_INCLUDE_TEMPLATE_DEFS
#include "Geometry/internal/test-primitives.C"
#endif


#endif
