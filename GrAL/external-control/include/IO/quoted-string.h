#ifndef GRAL_GB_IO_QUOTED_STRING_H
#define GRAL_GB_IO_QUOTED_STRING_H


/* ------------------------------------------------------------

    Copyright (C) 2008 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include <string>
#include <istream>
#include <ostream>

namespace GrAL {
  /*! \brief A string wrapper for reading quoted strings

    A string '"A B C D"' (including the double quotes) can be read at once into a quoted string qs.
    qs.unquoted() will yield 'A B C D'.

    Usage example:
     \code
     quoted_string qs;
     stringstream in("\"A B C D\"");
     in >> qs;
     cout << qs << endl; // prints '"A B C D"'
     \endcode

     \see Test in \ref test-quoted-string.C
  */  
  class quoted_string {
    std::string s;
    char begin_quote, end_quote;
    bool is_quoted;
  public:
    quoted_string();
    quoted_string(std::string const& str);
    quoted_string& operator=(std::string const& str);

    void read (std::istream& in);
    void print(std::ostream& out) const;

    std::string const& unquoted() const { return s;}
    operator std::string const&() const { return unquoted();}

    const char* c_str() const { return s.c_str();}
  };

  inline bool operator==(quoted_string const&  qs, std::string   const& s)   { return s == qs.unquoted();}
  inline bool operator==(std::string   const&   s, quoted_string const& qs)  { return s == qs.unquoted();}
  inline bool operator==(quoted_string const& qs1, quoted_string const& qs2) { return qs1.unquoted() == qs2.unquoted();}
  inline bool operator!=(quoted_string const&  qs, std::string   const& s)   { return s != qs.unquoted();}
  inline bool operator!=(std::string   const&   s, quoted_string const& qs)  { return s != qs.unquoted();}
  inline bool operator!=(quoted_string const& qs1, quoted_string const& qs2) { return qs1.unquoted() != qs2.unquoted();}
  
  inline std::ostream& operator<<(std::ostream& out, quoted_string const& qs)
  { qs.print(out); return out;}
  
  inline std::istream& operator>>(std::istream& in,  quoted_string & qs)
  { qs.read(in); return in;}

}

#endif
