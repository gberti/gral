#ifndef NMWR_GB_CONTAINER_FUNCTIONS_H
#define NMWR_GB_CONTAINER_FUNCTIONS_H

#include "compiler-config.h"

#ifdef __KCC
#include <functional.h>

template <class T>
struct identity : public std::unary_function<T, T> {
  const T& operator()(const T& x) const { return x; }
};
#endif

#endif
