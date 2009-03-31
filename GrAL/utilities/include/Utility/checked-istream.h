#ifndef NMWR_UTILITY_CHECKED_ISTREAM_H
#define NMWR_UTILITY_CHECKED_ISTREAM_H


/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include <iostream>
#include <typeinfo>
#include "Utility/pre-post-conditions.h"

namespace GrAL {

/*! \brief Wrapper for \c std::istream, checking for successful read operation.

    \ingroup io

    Example:
    \code
    int i,j;
    checked_istream checked_cin(&cin);
    checked_in >> i >> j;
    \endcode

    \see Tested in \ref test-checked-istream.C
 */
class checked_istream {
private:
  std::istream* in;
public:
  checked_istream(std::istream* in_ = 0) : in( in_) {}
  checked_istream(std::istream& in_)     : in(&in_) {}

  operator void*() const   { REQUIRE_ALWAYS(in != 0, "", 1); return (in->fail() ? (void *)0 : (void *)(-1));}

  /*! \brief Implicit conversion to \c istream
      
      This allows one to use the idiom
      \code
        (while in >> i) {
          // ...
        }
      \endcode
  */
  operator std::istream&() { REQUIRE_ALWAYS(in != 0, "",1); return *in;}

  template<class T>
  friend checked_istream& operator>>(checked_istream& in, T & t)
  { 
    REQUIRE_ALWAYS( (in.in != 0), "no istream!\n",1);
    REQUIRE_ALWAYS( (in.in->good()), "attempt to read from empty istream!",1);

    *(in.in) >> t; 

    // These macros must be on one line, else different compilers will report different line number
    // for the errors, and regression fails.
    REQUIRE_ALWAYS( (in.in->eof() || !in.in->fail()), "reading value " << t << " of type " << typeid(T).name() << " caused an error!\n",1);
    REQUIRE_ALWAYS( (!in.in->bad()), "bad istream, reading " << t << " of type " << typeid(T).name()  << " caused an error!\n",1);
    return in;
  }
};

} // namespace GrAL 
#endif
