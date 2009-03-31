#ifndef GRAL_IS_SPECIALIZED_H
#define GRAL_IS_SPECIALIZED_H


/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


namespace GrAL {

template<class V>
struct is_specialized {
  static bool specialized() { return false;}
  static const char* name() { return "";}
};

} // namespace GrAL {
#endif
