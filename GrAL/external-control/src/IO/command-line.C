
// $LICENSE

#include "IO/command-line.h"

#ifdef GRAL_HAS_SSTREAM
#include <sstream>
#else
#include <strstream.h>
#endif

namespace GrAL {


Commandline::Commandline(int argc, char* argv[]) : commands("") {
  //  ostringstream cmds;
#ifdef GRAL_HAS_SSTREAM
  ::std::ostringstream cmds;
  for(int i = 1; i < argc; ++i)
    cmds << argv[i] << " ";
  cmds << '\n';
  commands = cmds.str();
#else
  ostrstream cmds;
  for(int i = 1; i < argc; ++i)
    cmds << argv[i] << " ";
  cmds << '\n' << ::std::ends;
  int n = strlen(cmds.str());
  char* copy = new char[n+1];
  strcpy(copy,cmds.str());
  commands = ::std::string(copy);
#endif
}

//istringstream Commandline::get() const { return istringstream(commands);}
//strstream Commandline::get() const { return strstream(commands.c_str());}
const char* Commandline::c_str() const { 
  char* copy = new char[strlen(commands.c_str())+1];
  strcpy(copy,commands.c_str());
  return copy;
}

} // namespace GrAL 
