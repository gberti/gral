#ifndef NMWR_GB_CONTAINER_FUNCTIONS_H
#define NMWR_GB_CONTAINER_FUNCTIONS_H

#include "Config/compiler-config.h"

#include <functional>

template <class T>
struct my_identity : public std::unary_function<T, T> {
  const T& operator()(const T& x) const { return x; }
};
#endif

#endif
