#ifndef NMWR_GB_COMMAND_LINE_H
#define NMWR_GB_COMMAND_LINE_H


//----------------------------------------------------------------
//   (c) Guntram Berti, 1997
//   Chair for Numerical Mathematics & Scientific Computing (NMWR)
//   TU Cottbus - Germany
//   http://math-s.math.tu-cottbus.de/NMWR
//   
//----------------------------------------------------------------


/* #include "Iso-std/sstream" */
#include "mystring.h"
#include <strstream.h>

class  Commandline {
public:
  Commandline(int argc, char* argv[]);
  //  istringstream get() const;
  //strstream get() const;
  const char* c_str() const;
private:
  string commands;
};


#endif
