#ifndef GRAL_GB_SEQUENCE_INTEGER_ITERATOR_H
#define GRAL_GB_SEQUENCE_INTEGER_ITERATOR_H


/* ------------------------------------------------------------

    Copyright (C) 2001 - 2009 Guntram Berti, 
    Contact: guntram AT users DOT berlios DOT de

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */
_2003


namespace GrAL {


/*! \brief View an integral type as iterator

   \ingroup accessors
   
   \todo support full random-access iterator interface
 
*/
template<class INT, class C=INT>
class integer_iterator {
  typedef integer_iterator<INT, C> self;
  INT i;
public:
  typedef INT value_type;

  integer_iterator()   {}
  integer_iterator(value_type const& ii) : i(ii) {}
  
  value_type operator*() const { return i;}
  self& operator++() { ++i; return *this;}
  self& operator--() { --i; return *this;}
};


template<class INT, class C>
inline bool operator==(integer_iterator<INT, C> i1, integer_iterator<INT, C> i2) { return *i1 == *i2;}

template<class INT, class C>
inline bool operator!=(integer_iterator<INT, C> i1, integer_iterator<INT, C> i2) { return *i1 != *i2;}

} // namespace GrAL 





#endif
