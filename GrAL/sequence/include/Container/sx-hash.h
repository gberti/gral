#ifndef  GRAL_GB_SX_HASH_H
#define  GRAL_GB_SX_HASH_H

// $LICENSE_NEC

#include <hash_map>

// namespace GrAL {

namespace my_hash_sx {

  template<class I>
  struct hash {
  public:
    typedef I key_type;
    size_t operator()(I const& t) const { ::std::hash_compare<I> h; return h(t);}
  };

  template<class T>
  struct hash<T*> {
    typedef T*     key_type;
    typedef T*     argument_type;
    typedef size_t result_type;
    size_t operator()(T* t) const { T* t0 = 0; return t - t0; }// reinterpret_cast<unsigned>(t);}
  };

  template<class H, class _Pr>
  class hash_compare {
    typedef typename H::key_type _Kty;
  public:
    enum
      {       // parameters for hash table
	bucket_size = 4,        // 0 < bucket_size
	min_buckets = 8};       // min_buckets = 2 ^^ N, 0 < N
    
    hash_compare()
      : comp()
    {       // construct with default comparator
    }
    
    hash_compare(_Pr _Pred)
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
  

  // Wrapper to sxc++'s hash_map, behaving like the SGI hash_map
  template<class K, class T, class H = hash<K> >
  class hash_map : public ::std::hash_map<K,T, hash_compare<H, ::std::less<K> > >
  {
    typedef ::std::hash_map<K,T, hash_compare<H, ::std::less<K> > > base;
  public:
    hash_map() {}
    hash_map(unsigned /* sz */) : base() {}
  };

}


// } // namespace GrAL 


#endif
