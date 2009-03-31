#ifndef NMWR_GB_GRAL_TEST_ITERATORS_H
#define NMWR_GB_GRAL_TEST_ITERATORS_H


/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


/*! \file  

   Convenience header for including all iterator test functions

 */

// sequence iterators
#include "Gral/Test/test-vertex-iterator.h"
#include "Gral/Test/test-edge-iterator.h"
#include "Gral/Test/test-face-iterator.h"
#include "Gral/Test/test-facet-iterator.h"
#include "Gral/Test/test-cell-iterator.h"
#include "Gral/Test/test-sequence-iterator.h"

// incidence iterators

#include "Gral/Test/test-incidence-iterator.h"

// on cells
#include "Gral/Test/test-vertex-on-cell-iterator.h"
#include "Gral/Test/test-edge-on-cell-iterator.C"
#include "Gral/Test/test-facet-on-cell-iterator.h"
#include "Gral/Test/test-cell-on-cell-iterator.h"

// on facets
#include "Gral/Test/test-vertex-on-facet-iterator.h"
#include "Gral/Test/test-edge-on-facet-iterator.h"

// on faces
#include "Gral/Test/test-vertex-on-face-iterator.h"
#include "Gral/Test/test-edge-on-face-iterator.h"

// on edges
#include "Gral/Test/test-vertex-on-edge-iterator.h"

// on vertices
#include "Gral/Test/test-vertex-on-vertex-iterator.h"
#include "Gral/Test/test-cell-on-vertex-iterator.h"


#endif
