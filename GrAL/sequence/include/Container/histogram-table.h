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
  //! true if <code>size() == 0</code>
  bool empty() const { return the_map.empty();} 

  /*! \brief STL conforming iterators
   */
  //@{
  iterator       begin()       { return the_map.begin();}
  iterator       end()         { return the_map.end();}
  const_iterator begin() const { return the_map.begin();}
  const_iterator end()   const { return the_map.end();}
  //@}

  void print(std::ostream& out, 
	     std::string const& key_label = " ",
	     std::string const& num_label = "\n") const
  {
    for(const_iterator it = begin(); it != end(); ++it) {
      // int w = int(ceil(log10((*this)(max_entry()))));
      out << (*it).first << key_label << (*it).second << num_label;
    }
  }

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

  template<class T>
  class histogram_printer {
  private:
    histogram_table<T> const& hist;
    std::string key_label;
    std::string num_label;
  public:
    histogram_printer(histogram_table<T> const& h,
		      std::string const& key_label_ = " ",
		      std::string const& num_label_ = "\n") 
			: hist(h), key_label(key_label_), num_label(num_label_) {}
    void p(std::ostream& out) const { hist.print(out, key_label, num_label);}
 };

  template<class T>
  std::ostream& operator<<(std::ostream& out, histogram_printer<T> const& h) { h.p(out); return out;}

  template<class T>
  histogram_printer<T> print_histogram(histogram_table<T> const& hist, 
				       std::string const& key_label = " ",
				       std::string const& num_label = "\n")
  { return histogram_printer<T>(hist, key_label, num_label);}
  

} // namespace GrAL 

#endif
