#ifndef GRAL_GB_CONTAINER_HISTOGRAM_TABLE_H
#define GRAL_GB_CONTAINER_HISTOGRAM_TABLE_H

// $LICENSE_NEC_2003

#include "Container/my-hash-map.h"
#include "Utility/pre-post-conditions.h"

namespace GrAL {


/*! \brief Table for managing histograms

    \ingroup containers

    \see compute_histogram
    \see Tested in \ref test-histogram.C
 
    \todo could be specialized for small data types, say range is one byte.
          Must transform key to range [0, max]   
*/
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

  template<class IT>
  histogram_table(IT b, IT e) 
  {
    init(b,e);
  }
  
  template<class IT>
  void init(IT b, IT e) {
    while(b != e) {
      this->operator[](*b)++;
      ++b;
    }
  }

  void clear() { the_map.clear();}
  
  /*! \brief Write access. Returns 0 if key \k does not exist.
    \Note: This inserts a new entry if none exists for the key \c k.
   */
  size_type& operator[](key_type const& k)
  { 
    iterator ik = the_map.find(k);
    if( ik == the_map.end()) {
      ik = (the_map.insert(table_value_type(k,0))).first;
    }
    return ik->second;
  }

  //! Read-only access. Returns 0 if key \c k does not exist.
  size_type operator()(key_type const& k)
  {
    iterator ik = the_map.find(k);
    if( ik != the_map.end())
      return ik->second;
    return 0;
  }

  //! number of different entries
  size_type size() const { return the_map.size();}

  /*! \brief STL conforming iterators
   */
  //@{
  iterator begin() { return the_map.begin();}
  iterator end()   { return the_map.end();}
  const_iterator begin() const { return the_map.begin();}
  const_iterator end()   const { return the_map.end();}
  //@}

  //! returns key with maximal number of entries 
  key_type max_entry() const {
    REQUIRE_ALWAYS(! the_map.empty(), "max_entry() called with empty histogram!",1);
    size_type mx = 0;
    key_type res = begin()->first;
    for(const_iterator e = begin(); e != end(); ++e)
      if((*e).second > mx) {
	mx  = (*e).second;
	res = (*e).first;
      }
    return res;
  }
};

} // namespace GrAL 

#endif
