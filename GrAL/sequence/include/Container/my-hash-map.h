#ifndef NMWR_GB_MY_HASH_MAP_H
#define NMWR_GB_MY_HASH_MAP_H

// $LICENSE

#include "Config/compiler-config.h"

// define STDEXT as the namespace where hash<> templates live.
// put hash_map into global namespace
// (intel defines hash<> in std::, but our substitute for their hash_map
// cannot be in std::)

// KCC currently not supported
#if defined __KCC
//#include "KCC/hash_map.h"
#error "hash_map for KCC is currently not supported!"

#elif defined INTEL_CC
// Intel compiler uses Dinkumware STL
// make a SGI-STL like wrapper
#include "Container/intel-hash.h"
using my_hash_intel::hash_map;
//namespace ext = std;
#define STDEXT std

#elif defined __GNUC__ && __GNUC__ >= 3 && __GNUC_MINOR__>= 1
// use SGI-like hash_map
#include <ext/hash_map>
using __gnu_cxx::hash_map;
#define STDEXT __gnu_cxx
namespace STDEXT {

  template<class T>
  struct hash<T*> {
    typedef T* key_type;
    typedef T* argument_type;
    typedef size_t result_type;
    size_t operator()(T* t) const { return reinterpret_cast<unsigned>(t);}
  };
}

#else 
#include <hash_map>
using std::hash_map;
#define STDEXT std 

namespace STDEXT {

  template<class T>
  struct hash<T*> {
    typedef T* key_type;
    typedef T* argument_type;
    typedef size_t result_type;
    size_t operator()(T* t) const { return reinterpret_cast<unsigned>(t);}
  };
}

#endif


#endif
