#ifndef GRAL_GB_IO_TEXTLINE_H
#define GRAL_GB_IO_TEXTLINE_H


/* ------------------------------------------------------------

    Copyright (C) 2005 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include <string>
#include <iostream>


namespace GrAL {

  class textline {
  public:
    textline() {}
    textline  (std::istream& in)  { read(in);}
    void read (std::istream& in);
    void print(std::ostream& out) const { out << line << "\n";}

    operator std::string const&() const { return line;}
  private:
    std::string line;
  };

  inline void textline::read(std::istream& in) { 
    std::getline(in,line);
  } 

  inline std::ostream& operator<<(std::ostream& out, textline const& tl)
  { tl.print(out); return out;}

  inline std::istream& operator>>(std::istream& in, textline       & tl)
  { tl.read(in); return in;}

} // namespace GrAL

#endif
