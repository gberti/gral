#ifndef NMWR_GB_AS_STRING_H
#define NMWR_GB_AS_STRING_H

/*! \file

*/

// $LICENSE

#include <string>

#ifdef GRAL_HAS_SSTREAM
#include <sstream>
#else
#include <strstream.h>
#endif

namespace GrAL {

/*! \brief Convert any type into a string
   
    The type T is required to have an output operator <tt> >> </tt>
    to <tt>::std::ostream</tt> defined.
*/
template<class T>
inline ::std::string as_string(const T& t)
{
#ifdef GRAL_HAS_SSTREAM
  ::std::ostringstream s;
  s << t;
  return s.str();
#else
  ::std::strstream s;
  s << t << ::std::ends;
  int n = strlen(s.str());
  char* copy = new char[n+1];
  strcpy(copy,s.str());
 
  return ::std::string(copy);
#endif
}

} // namespace GrAL {

#endif
