#include "IO/command-line.h"

Commandline::Commandline(int argc, char* argv[]) : commands("") {
  //  ostringstream cmds;
  ostrstream cmds;
  for(int i = 1; i < argc; ++i)
    cmds << argv[i] << " ";
  cmds << '\n' << ends;
  int n = strlen(cmds.str());
  char* copy = new char[n+1];
  strcpy(copy,cmds.str());
  commands = string(copy);
}

//istringstream Commandline::get() const { return istringstream(commands);}
//strstream Commandline::get() const { return strstream(commands.c_str());}
const char* Commandline::c_str() const { 
  char* copy = new char[strlen(commands.c_str())+1];
  strcpy(copy,commands.c_str());
  return copy;
}
