#ifndef NMWR_GB_AS_STRING_H
#define NMWR_GB_AS_STRING_H



// $LICENSE



#include <strstream.h>
#include <string>

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
