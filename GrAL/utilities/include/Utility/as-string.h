#ifndef NMWR_GB_AS_STRING_H
#define NMWR_GB_AS_STRING_H



// $LICENSE



#include <strstream.h>
#include <string>

template<class T>
inline std::string as_string(const T& t)
{
  std::strstream s;
  s << t << std::ends;
  int n = strlen(s.str());
  char* copy = new char[n+1];
  strcpy(copy,s.str());
 
  return std::string(copy);
}

#endif
