/*! \file
 */


/* ------------------------------------------------------------

    Copyright (C) 2008 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include "IO/quoted-string.h"

namespace GrAL {

  quoted_string::quoted_string() 
    : begin_quote('"'), end_quote('"'), is_quoted(false) {}

  quoted_string::quoted_string(std::string const& str) 
    : s(str), begin_quote('"'), end_quote('"'), is_quoted(false) {}

  quoted_string& quoted_string::operator=(std::string const& str) 
  {
    s = str;
    is_quoted = false;
    return *this;
  }
  


  void quoted_string::read(std::istream & in) 
  {
    in >> std::ws;
    char test_quote = in.peek();
    if(test_quote == begin_quote) {
      is_quoted = true;
      test_quote = in.get();
      std::getline(in, s, end_quote);
    }
    else {
      is_quoted = false;
      in >> s;
    }
  }

  void quoted_string::print(std::ostream & out) const
  {
    if(is_quoted) 
      out << begin_quote << s << end_quote; 
    else
      out << s;
  }
}
