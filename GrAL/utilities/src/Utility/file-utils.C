

/* ------------------------------------------------------------

    Copyright (C) 2007 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include "Utility/file-utils.h"
#include "Utility/pre-post-conditions.h"

#include <iostream>
#include <fstream>

namespace GrAL {
 
  ref_ptr<std::istream> get_file_or_stdin(std::string const& nm) {
    if(nm == "-")
      return ref_ptr<std::istream>(std::cin);
    else {
      std::ifstream * in = new std::ifstream(nm.c_str());
      REQUIRE_ALWAYS(in->is_open(), "Could not open file \"" << nm << "\", quitting.\n",1);
      return new_ref_ptr(in);
    }
  }

 
  ref_ptr<std::ostream> get_file_or_stdout(std::string const& nm) {
    if(nm == "-")
      return ref_ptr<std::ostream>(std::cout);
    else {
      std::ofstream * out = new std::ofstream(nm.c_str());
      REQUIRE_ALWAYS(out->is_open(), "Could not open file \"" << nm << "\", quitting.\n",1);
      return new_ref_ptr(out);
    }
  }

} // namespace GrAL
