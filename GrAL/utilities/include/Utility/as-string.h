#ifndef NMWR_GB_AS_STRING_H
#define NMWR_GB_AS_STRING_H


//----------------------------------------------------------------
//   (c) Guntram Berti, 1997
//   Chair for Numerical Mathematics & Scientific Computing (NMWR)
//   TU Cottbus - Germany
//   http://math-s.math.tu-cottbus.de/NMWR
//   
//----------------------------------------------------------------


// #ifdef __GNUC__
#if defined(__GNUC__) && __GNUC_MINOR__ < 8

#include <Iso-std/sstream.h>

template<class T>
inline  string as_string(const T& t)
{
  ostringstream s;
  s << t;
  return string(s.str());
}

#else

#include <strstream.h>
#include "mystring.h"

template<class T>
inline string as_string(const T& t)
{
  strstream s;
  s << t << ends;
  int n = strlen(s.str());
  char* copy = new char[n+1];
  strcpy(copy,s.str());
 
  return string(copy);
}

#endif


#endif
