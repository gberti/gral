#ifndef NMWR_GB_COMMAND_LINE_H
#define NMWR_GB_COMMAND_LINE_H



// $LICENSE


#include <string>
// #include <strstream.h>

class  Commandline {
public:
  Commandline(int argc, char* argv[]);
  //  istringstream get() const;
  //strstream get() const;
  const char* c_str() const;
private:
  std::string commands;
};


#endif
