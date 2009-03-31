#ifndef NMWR_GB_COMMAND_LINE_H
#define NMWR_GB_COMMAND_LINE_H




/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */



#include <string>
// #include <strstream.h>

namespace GrAL {

class  Commandline {
public:
  enum format { plain, protect_args_with_spaces };
  /*! \brief Construct from command line 
     If f = \c plain, a simple string is constructed.
     If f = \c protect_args_with_spaces, arguments that contain spaces
       are surrounded with double quotes.
   */
  Commandline(int argc, char* argv[], format f = plain);

  const char*        c_str() const;
  std::string const&   str() const;
private:
  std::string commands;
};

} // namespace GrAL 

#endif
