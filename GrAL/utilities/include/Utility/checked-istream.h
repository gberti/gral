#ifndef NMWR_UTILITY_CHECKED_ISTREAM_H
#define NMWR_UTILITY_CHECKED_ISTREAM_H

// $LICENSE

#include <iostream>
#include "Utility/pre-post-conditions.h"

namespace GrAL {

/*! \brief Wrapper for istream, checking successful read operation.

    Example:
    \code
    int i,j;
    checked_istream checked_cin(&cin);
    checked_in >> in >> j;
    \endcode
 */
class checked_istream {
private:
  ::std::istream* in;
public:
  checked_istream(::std::istream* in_ = 0) : in(in_) {}

  template<class T>
  friend checked_istream& operator>>(checked_istream& in, T & t)
  { 
    REQUIRE( (in.in != 0), "no istream!\n",1);
    *(in.in) >> t; 
    REQUIRE( (in.in->good()), "bad istream, reading " << t << " failed!\n",1);
    return in;
  }
};

} // namespace GrAL 
#endif
