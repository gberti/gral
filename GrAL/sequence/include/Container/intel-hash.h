#ifndef  GRAL_GB_INTEL_HASH_H
#define  GRAL_GB_INTEL_HASH_H


/* ------------------------------------------------------------

    Copyright (C) 2001 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include <ext/hash_map>

namespace GrAL {

namespace my_hash_intel {

  template<class I>
  struct intel_hash {
  public:
    typedef I key_type;
    size_t operator()(I const& t) const { ::std::hash<I> h; return h(t);}
  };

  template<class H, class _Pr>
  class intel_hash_compare {
    typedef typename H::key_type _Kty;
  public:
    enum
      {       // parameters for hash table
	bucket_size = 4,        // 0 < bucket_size
	min_buckets = 8};       // min_buckets = 2 ^^ N, 0 < N
    
    intel_hash_compare()
      : comp()
    {       // construct with default comparator
    }
    
    intel_hash_compare(_Pr _Pred)
      : comp(_Pred)
    {       // construct with _Pred comparator
    }
    
    size_t operator()(const _Kty& _Keyval) const
    {       // hash _Keyval to size_t value
      return ((size_t)hash_value(_Keyval));
    }
    bool operator()(const _Kty& _Keyval1, const _Kty& _Keyval2) const
    {       // test if _Keyval1 ordered before _Keyval2
      return (comp(_Keyval1, _Keyval2));
    }
    
  protected:
    _Pr comp;       // the comparator object
    H hash_value;
  };
  

  // Wrapper to icc's hash_map, behaving like the SGI hash_map
  template<class K, class T, class H = intel_hash<K> >
  class hash_map : public ::std::hash_map<K,T, intel_hash_compare<H, ::std::less<K> > >
  {
    typedef ::std::hash_map<K,T, intel_hash_compare<H, ::std::less<K> > > base;
  public:
    hash_map() {}
    hash_map(unsigned /* sz */) : base() {}
  };

}

} // namespace GrAL 

#endif
