#ifndef NMWR_GB_PARTIAL_MAPPING_H
#define NMWR_GB_PARTIAL_MAPPING_H


// $LICENSE

#include "my-hash-map.h" // STL

#include "Container/proxies.h"
#include "Utility/pre-post-conditions.h"

namespace GrAL {

//----------------------------------------------------------------
/*!  
   \brief A representation of a partial mapping T1 --> T2, using a default
    value for items not explicitely set.

    \ingroup mappings
 
  partial_mapping<T1,T2> is a model of Adaptable Unary Function.

 \b Template parameters:
  - T1 is Hashable, that is,  <TT> hash<T1> </TT> is defined.
  - T1 is Equality Comparable, that is, <TT> equal_to<T1> </TT> is defined.
  - T2 is Assignable and Default Constructible.

  \b Invariants:

  If pm is of type partial_mapping<T1,T2>, t1 of type T1 and t2 of type T2, then
    -  pm[t1] = t2  => t2 == pm(t1)
    -  pm(t1) may be evaluated for every t1 of type T1.
    -  pm.defined(t1)   == true  <=> the value of t1 has been set explicitely before.
    -  pm.undefined(t1) == true  <=> pm(t1) == pm.default_value()

  \b Note:

  statement of the form: t2 =  pm[t1] would mean that the value of
  pm(t1) gets undefined if pm.undefined(t1) == true before!
  This is prohibited by returning a  proxy in operator[] which only 
  can be assigned to (i.e. occur only on the left side of an assignment).


 \todo
   There should also be a corresponding domain-class and 
   perhaps an inverse-mapping class : T2 --> P(T1)
 \todo
    Test directory for partial_mapping

*/
//----------------------------------------------------------------


template<class T1, class T2>
class partial_mapping {
public:
  // STL unary function conformance
  typedef T1                         argument_type;
  typedef T2                         result_type;

private:
  typedef STDHASH::hash_map<T1,T2 >  map_table_type;

  //   typedef write_only_proxy<T2>                    proxy_type;

  //------ DATA --------
  map_table_type mapping;
  T2             default_val;
public:
  typedef typename map_table_type::size_type      size_type;
  typedef typename map_table_type::iterator       iterator;
  typedef typename map_table_type::const_iterator const_iterator;

  //---------------------- construction --------------------------

  partial_mapping(const T2& def = T2()) : default_val(def) {}

  const T2& set_default(const T2& t2) { default_val = t2; return t2;}

  void clear() { mapping.clear();}

  //---------------------- queries -------------------------------

  size_type size()        const { return mapping.size();}
  size_type size_of_dom() const { return mapping.size();}
  bool            empty() const { return mapping.empty();}

  bool defined  (const T1& t1) const { return (mapping.find(t1) != mapping.end());}
  bool undefined(const T1& t1) const { return (mapping.find(t1) == mapping.end());}

  void undefine(const T1& t1) { mapping.erase(t1);}

  const T2& default_value() const { return default_val;}

  //---------------------- data access ---------------------------


  // should use domain/image types
  const_iterator begin() const { return mapping.begin();}
  const_iterator end()   const { return mapping.end();}

  // no non-const versions of begin/end: writing should occur
  // only via mapping interface: op[]/() below.

  //   proxy_type operator[](const T1& t1) { return proxy_type(mapping[t1]);}
  T2      & operator[](T1 const& t1) {
    if(undefined(t1))
      mapping[t1] = default_val;
    return mapping[t1];
  }
  T2 const& operator()(T1 const& t1) const {
    const_iterator i = mapping.find(t1);
    return ( i != mapping.end() ? (*i).second : default_val);
  }

};

} // namespace GrAL 

#endif
