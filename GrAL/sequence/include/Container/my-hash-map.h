#ifndef NMWR_GB_MY_HASH_MAP_H
#define NMWR_GB_MY_HASH_MAP_H


/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include "Config/compiler-config.h"
#include <string>

#if defined GRAL_USE_STLPORT

#include <hash_map>
#define STDEXT std
#define STDHASH std

namespace STDEXT {

  template<class T>
  struct hash<T*> {
    typedef T* key_type;
    typedef T* argument_type;
    typedef size_t result_type;
    size_t operator()(T* t) const { return reinterpret_cast<unsigned>(t);}
  };

  template<>
  struct hash<std::string> {
    typedef std::string key_type;
    typedef std::string argument_type;
    typedef size_t result_type;
    size_t operator()(key_type const& k) const { hash<char const*> h; return h(k.c_str());}
  };}


#else
// define STDEXT  as the namespace where hash<> templates live.
// define STDHASH as the namespace where hash_map<> templates live
// (These may be different, e.g. intel defines hash<> in ::std::, 
//  but our substitute for their hash_map cannot be in ::std::)

// KCC currently not supported
#if defined __KCC
//#include "KCC/hash_map.h"
#error "hash_map for KCC is currently not supported!"

#elif defined INTEL_CC && __ICC >= 800
#include <ext/hash_map>
#define STDEXT  __gnu_cxx
#define STDHASH __gnu_cxx

namespace STDEXT {
  /* specialization for pointers */
  template<class T>
  struct hash<T*> {
    typedef T* key_type;
    typedef T* argument_type;
    typedef size_t result_type;
    size_t operator()(T* t) const { return reinterpret_cast<size_t>(t);}
  };
 
  template<>
  struct hash<std::string> {
    typedef std::string key_type;
    typedef std::string argument_type;
    typedef size_t result_type;
    size_t operator()(key_type const& k) const { hash<char const*> h; return h(k.c_str());}
  };
}


#elif defined INTEL_CC
// Intel compiler uses Dinkumware STL
// We provide an SGI-STL like wrapper
#include "Container/intel-hash.h"
#define STDEXT  std
#define STDHASH my_hash_intel


#elif defined SXCPP
// SXCPP uses Dinkumware
#include "Container/sx-hash.h"
#define STDEXT  my_hash_sx
#define STDHASH my_hash_sx

#elif defined __GNUC__ && ( ( __GNUC__ >= 3 && __GNUC_MINOR__>= 1 ) || __GNUC__ >= 4) 

// use SGI-like hash_map
#include <ext/hash_map>
#define STDEXT  __gnu_cxx
#define STDHASH __gnu_cxx

namespace STDEXT {
  /* specialization for pointers */
  template<class T>
  struct hash<T*> {
    typedef T* key_type;
    typedef T* argument_type;
    typedef size_t result_type;
    size_t operator()(T* t) const { return reinterpret_cast<size_t>(t);}
  };
 
  template<>
  struct hash<std::string> {
    typedef std::string key_type;
    typedef std::string argument_type;
    typedef size_t result_type;
    size_t operator()(key_type const& k) const { hash<char const*> h; return h(k.c_str());}
  };
}


#else 
#include <hash_map>
#define STDEXT  std 
#define STDHASH std

namespace STDEXT {

  template<class T>
  struct hash<T*> {
    typedef T* key_type;
    typedef T* argument_type;
    typedef size_t result_type;
    size_t operator()(T* t) const { return reinterpret_cast<unsigned>(t);}
  };

  template<>
  struct hash<std::string> {
    typedef std::string key_type;
    typedef std::string argument_type;
    typedef size_t result_type;
    size_t operator()(key_type const& k) const { hash<char const*> h; return h(k.c_str());}
  };}

#endif

// GRAL_USE_STLPORT
#endif

#endif
