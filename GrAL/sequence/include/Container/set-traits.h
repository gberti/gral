#ifndef NMWR_GB_SET_TRAITS_H
#define NMWR_GB_SET_TRAITS_H



/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti, 
    Contact: guntram AT users DOT berlios DOT de

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


namespace GrAL {


/*! \brief empty frame for generic access to functionality of representations of sets.
    \ingroup setalgorithms

   This has to be specialized for user classes.
   Many classes may be seen a sets in different ways,
   so this opens the door to use generic set predicates on them,
   without having to change the classes themselves.

   Specializations must define
   - <tt> typedef element_type; </tt>
   - <tt> bool is_member(element_type const&); </tt>

  \see point_traits  (Geometry/point-traits.h)
  \see \ref setalgorithms
*/


template<class S> class set_traits {
  // to define:
  // typedef ... element_type
  // bool is_member(const element_type&);
};

} // namespace GrAL 

#endif
