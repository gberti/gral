#ifndef NMWR_GB_PRINT_RANGE_H
#define NMWR_GB_PRINT_RANGE_H

//----------------------------------------------------------------
//
//   (c) Guntram Berti, 2000
//   Chair for Numerical Mathematics & Scientific Computing (NMWR)
//   TU Cottbus - Germany
//   http://math-s.math.tu-cottbus.de/NMWR
//   
//----------------------------------------------------------------

#include <iostream.h>

//----------------------------------------------------------------
//  Output for a iterator range
//  Example: out << range(C.begin(), C.end());
//----------------------------------------------------------------

template<class T>
struct iter_range {
  T const* b;
  T const* e;
  iter_range(T const* bb, T const* ee) : b(bb), e(ee) {}
};

template<class T>
inline 
iter_range<T> range(T const* bb, T const* ee) 
{ return iter_range<T>(bb,ee); }

template<class T>
inline
ostream& operator<<(ostream& out, iter_range<T> r)
{
  while(r.b != r.e) {
    out << *(r.b) << ' ';
    ++r.b;
  }
  return out;
}

#endif
