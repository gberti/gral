#ifndef GRAL_GB_SEQUENCE_BIVECTOR_H
#define GRAL_GB_SEQUENCE_BIVECTOR_H

// $LICENSE_NEC


#include <deque>
#include "Utility/pre-post-conditions.h"

namespace GrAL {


/*! \brief Double-ended dynamic vector with stable indexing
  
   \author Guntram Berti
   \ingroup mappings

   Similarly to std::deque, this class offers a random-access container
   which can grow at both ends.
   Unlike std::deque (but like std::vector), indices will continue to be valid
   if insertion is performed only at the beginning/end (for std::vector: only at the end).
   The first item inserted will get index 0, items inserted later on at the beginning will
   get negative indices.

   This behavior is useful if we want to maintain external indexing into the container.
   Because arbitrary insertions would destroy this property, bivector does not support it.

   \see Tested in \ref test-bivector.C
*/


template<class T>
class bivector {
private:
  typedef std::deque<T> table_type;
  typedef table_type    tt;

  table_type table;
  int        offset;
public:
  typedef typename tt::value_type       value_type;
  typedef typename tt::reference        reference;
  typedef typename tt::const_reference  const_reference;
  typedef typename tt::iterator         iterator;
  typedef typename tt::const_iterator   const_iterator;
  typedef typename tt::size_type        size_type;
  typedef typename tt::difference_type  difference_type;
  typedef typename tt::difference_type  index_type;

  //! Construct empty bivector
  bivector()                         :              offset(0) {}
  /*! \brief Construct bivector with \c n items
    \post <tt>begin_index() == 0 && end_index() == n </tt>
   */
  bivector(size_type n)              : table(n),    offset(0) {} 
  /*! \brief Construct bivector with \c n items of value \c t0
    \post <tt>begin_index() == 0 && end_index() == n </tt>
   */
  bivector(size_type n, T const& t0) : table(n,t0), offset(0) {} 

  /*! \brief Set the first index to \c i
     \post <tt>begin_index() == i && end_index() == i + size() </tt>
   */
  void init_begin_index(index_type i) {
    REQUIRE_ALWAYS(table.empty(), "",1);
    offset = -i;
  }
  //! Insert \c t at the end and return its permanent index
  int push_back (T const& t) { table.push_back(t); return table.size() - offset -1;}
  //! Insert \c t at the beginning and return its permanent index
  int push_front(T const& t) {
    offset++; 
    table.push_front(t);
    return -offset;
  }

  void pop_back()  { ce(); table.pop_back (); }
  void pop_front() { ce(); table.pop_front(); offset--; }

  void clear() { offset = 0; table.clear();}
  void swap(bivector<T> & rhs) { std::swap(offset,rhs.offset); std::swap(table,rhs.table); }

  //! index of first item
  int begin_index() const { return -offset;}
  //! index of last item
  int back_index()  const { return table.size() - offset -1;}
  //! for comptibility with iterators: Index of past-the-end item
  int end_index()   const { return table.size() - offset;}

  reference       operator[](int i)        { return table[size_type(i + offset)];}
  const_reference operator[](int i)  const { return table[size_type(i + offset)];}
  const_reference operator()(int i)  const { return table[size_type(i + offset)];}

  reference front() { return table.front();}
  reference back () { return table.back ();}
  const_reference front() const { return table.front();}
  const_reference back () const { return table.back ();}

  size_type size()  const { return table.size();}
  bool      empty() const { return table.empty();}
  void      ce()    const { REQUIRE(! empty(), "",1);}

  size_type max_size() const { return table.max_size();}

  iterator       begin()       { return table.begin();}
  iterator       end  ()       { return table.end();}
  const_iterator begin() const { return table.begin();}
  const_iterator end  () const { return table.end();}
};

} // namespace GrAL 

#endif
