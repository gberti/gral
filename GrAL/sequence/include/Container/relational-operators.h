#ifndef GRAL_GB_SEQUENCE_RELATIONAL_OPERATORS_H
#define GRAL_GB_SEQUENCE_RELATIONAL_OPERATORS_H


/* ------------------------------------------------------------

    Copyright (C) 2001 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


namespace GrAL {

/*! \brief Relational operators for use in templates
   
    \ingroup operators
*/

namespace relational_operators {

  struct eq  { template<class T, class U> bool operator()(T const& t, U const& u) const { return t == u;}};
  struct neq { template<class T, class U> bool operator()(T const& t, U const& u) const { return t != u;}};
  struct lt  { template<class T, class U> bool operator()(T const& t, U const& u) const { return t <  u;}};
  struct gt  { template<class T, class U> bool operator()(T const& t, U const& u) const { return t >  u;}};
  struct leq { template<class T, class U> bool operator()(T const& t, U const& u) const { return t <= u;}};
  struct geq { template<class T, class U> bool operator()(T const& t, U const& u) const { return t >= u;}};

}

} // namespace GrAL 

#endif
