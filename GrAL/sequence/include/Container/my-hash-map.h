#ifndef NMWR_GB_MY_HASH_MAP_H
#define NMWR_GB_MY_HASH_MAP_H

// $LICENSE


#include "Config/compiler-config.h"

#ifdef __KCC
// namespace nmwr_kai_hash {
#include "KCC/hash_map.h"
//}
// using nmwr_kai_hash::hash_map;

#else
#include "hash_map.h"
#endif


#endif
