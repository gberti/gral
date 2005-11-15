#ifndef GRAL_GB_IO_TEXTLINE_H
#define GRAL_GB_IO_TEXTLINE_H

// $LICENSE_NEC_2005

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
