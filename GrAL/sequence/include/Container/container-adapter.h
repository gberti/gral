#ifndef NMWR_GB_CONTAINER_ADAPTER_H
#define NMWR_GB_CONTAINER_ADAPTER_H

//----------------------------------------------------------------
//   (c) Guntram Berti, 1998
//   Chair for Numerical Mathematics & Scientific Computing (NMWR)
//   TU Cottbus - Germany
//   http://math-s.math.tu-cottbus.de/NMWR
//   
//----------------------------------------------------------------

#include "Container/mapped-value-iterator.h"

//----------------------------------------------------------------
//
// template<class Cont> class ValueMask:
//
// This class provides a wrapper for an associative container 
// storing <key,value> pairs.
// It gives a const view to the sequence of value-items of the container.
//
// CONSTRAINTS on type of template parameter:
//  * Cont
//    This is a sequence (STL) of pair<key,value> items.
//    typedef pair<key,value> value_type;
//    typedef const_iterator; (which derefences to value_type)
//
// could be extended to KeyMask<Cont>.
//----------------------------------------------------------------

/* use gives internal compiler error with gcc 2.8 */

template<class Cont>
class ValueMask {
private:
  const Cont* the_container;
public:
  typedef typename Cont::value_type          base_value_type;
  typedef typename Cont::const_iterator      base_iter;
  typedef get_second<base_value_type>        functor_type;

  typedef mapped_value_const_iterator<base_iter,functor_type> const_iterator;
  typedef typename const_iterator::value_type                 value_type;

  ValueMask(const Cont& c) : the_container(&c) {}
  
  const_iterator begin() const 
    { return const_iterator(the_container->begin(),functor_type()); }
  const_iterator end() const 
    { return const_iterator(the_container->end(),functor_type()); }

  unsigned size() const { return the_container->size();}
};


template<class Cont>
inline ValueMask<Cont>
values_of(const Cont& c) { return ValueMask<Cont>(c);}

#endif
