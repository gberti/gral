#ifndef NMWR_GB_SET_TRAITS_H
#define NMWR_GB_SET_TRAITS_H


// $LICENSE


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

#endif
