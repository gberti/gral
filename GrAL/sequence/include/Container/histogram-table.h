#ifndef GRAL_GB_CONTAINER_HISTOGRAM_TABLE_H
#define GRAL_GB_CONTAINER_HISTOGRAM_TABLE_H

#include "Container/my-hash-map.h"

template<class T>
class histogram_table {
public:
  typedef size_t size_type;
  typedef T      key_type;
  typedef size_type data_type;
  typedef key_type  argument_type;
  typedef data_type result_type;
  typedef STDHASH::hash_map<T, size_type>     table_type;
  typedef typename table_type::iterator       iterator;
  typedef typename table_type::const_iterator const_iterator;
  typedef typename table_type::value_type     table_value_type;

private:
  table_type the_map;
public:
  histogram_table() {}

  size_type& operator[](key_type const& k)
  { 
    iterator ik = the_map.find(k);
    if( ik == the_map.end()) {
      ik = (the_map.insert(table_value_type(k,0))).first;
    }
    return ik->second;
  }

  size_type operator()(key_type const& k)
  {
    iterator ik = the_map.find(k);
    if( ik != the_map.end())
      return ik->second;
    return 0;
  }

  iterator begin() { return the_map.begin();}
  iterator end()   { return the_map.end();}
  const_iterator begin() const { return the_map.begin();}
  const_iterator end()   const { return the_map.end();}
};

// could be specialized for small data types, say range is one byte
// must transform key to range [0, max]

#endif
