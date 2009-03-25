

/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti, 
    Contact: guntram AT users DOT berlios DOT de

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include "IO/command-line.h"

#ifdef GRAL_HAS_SSTREAM
#include <sstream>
#else
#include <strstream.h>
#endif

namespace GrAL {

  static bool has_spaces(std::string const& s) { return s.find(' ') < s.size();}
  static bool has_spaces(const char*        s) { return has_spaces(std::string(s));}

  Commandline::Commandline(int argc, char* argv[], Commandline::format f) : commands("") {

  std::string pre, post;
#ifdef GRAL_HAS_SSTREAM
  std::ostringstream cmds;
#else
  ostrstream cmds;
#endif
  for(int i = 1; i < argc; ++i) {
    if(f == protect_args_with_spaces) 
      if(has_spaces(argv[i]))
	pre = post = "\"";
      else
	pre = post = "";
    cmds << pre << argv[i] << post << " ";
  }
  cmds << '\n';

#ifdef GRAL_HAS_SSTREAM
  commands = cmds.str();
#else
  cmds << std::ends;
  int n = strlen(cmds.str());
  char* copy = new char[n+1];
  strcpy(copy,cmds.str());
  commands = std::string(copy);
#endif
}

//istringstream Commandline::get() const { return istringstream(commands);}
//strstream Commandline::get() const { return strstream(commands.c_str());}
const char* Commandline::c_str() const { 
  // char* copy = new char[strlen(commands.c_str())+1];
  //strcpy(copy,commands.c_str());
  //return copy;
  return commands.c_str();
}

  std::string const& Commandline::str() const { return commands;}

} // namespace GrAL 
