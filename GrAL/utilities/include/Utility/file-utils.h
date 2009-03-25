#ifndef GRAL_GB_UTILITY_FILE_UTILS_H
#define GRAL_GB_UTILITY_FILE_UTILS_H


/* ------------------------------------------------------------

    Copyright (C) 2007 - 2009 Guntram Berti, 
    Contact: guntram AT users DOT berlios DOT de

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include "Utility/ref-ptr.h"

#include <istream>
#include <ostream>
#include <string>

namespace GrAL {

  /*! \brief Get open input file or stdin
      \ingroup io
      
      If <tt> name == "-" </tt>, return \c cin, else a file open for reading.
      
   */
  ref_ptr<std::istream> get_file_or_stdin(std::string const& nm);
 
/*! \brief Get open output file or stdout
    \ingroup io
      
    If <tt> name == "-" </tt>, return \c cout, else a file open for writing.
      
   */
  ref_ptr<std::ostream> get_file_or_stdout(std::string const& nm);



} // namespace GrAL

#endif
