#ifndef NMWR_GB_COMMAND_LINE_H
#define NMWR_GB_COMMAND_LINE_H



// $LICENSE


#include <string>
// #include <strstream.h>

namespace GrAL {

class  Commandline {
public:
  Commandline(int argc, char* argv[]);

  const char*        c_str() const;
  std::string const&   str() const;
private:
  std::string commands;
};

} // namespace GrAL 

#endif
