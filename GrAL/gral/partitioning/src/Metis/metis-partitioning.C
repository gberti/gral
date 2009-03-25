

/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti, 
    Contact: guntram AT users DOT berlios DOT de

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


/*! \file
 */

#include "Gral/Partitioning/metis-partitioning.h"
#include "Gral/Partitioning/internal/metis-options.h"

namespace GrAL {

MetisPartitioning::MetisPartitioning()
{ options = new metis_options; }

MetisPartitioning::~MetisPartitioning()
{ delete options;}

} // namespace GrAL 
