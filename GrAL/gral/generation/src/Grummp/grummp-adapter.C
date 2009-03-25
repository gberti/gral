
/* ------------------------------------------------------------

    Copyright (C) 2005 - 2009 Roman Putanowicz

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */

#include "Gral/Generation/Grummp/grummp-adapter.h"

namespace GrAL {
namespace grummp_adapter {
  
  Mesh2DAdapter::SD::SD() {
  the_archetype[0] = archetype_type(3);
  } 
  Mesh2DAdapter::SD Mesh2DAdapter::sd;

} // namespace grummp_adapter
} // namespace GrAL
