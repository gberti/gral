#ifndef NMWR_GB_CONTAINER_TUPLE_POINT_TRAITS_H
#define NMWR_GB_CONTAINER_TUPLE_POINT_TRAITS_H

/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */

#include "Container/tuple.h"
#include "Geometry/point-traits.h"

namespace GrAL {


// this goes into an extra file because not all users of tuple<>
// should depend on geometry module

template<class T, unsigned N>
struct point_traits<tuple<T,N> > 
  : public point_traits_fixed_size_array<tuple<T,N>, T, N> 
{};

} // namespace GrAL 

#endif
