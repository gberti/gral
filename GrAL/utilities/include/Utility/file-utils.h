#ifndef GRAL_GB_UTILITY_FILE_UTILS_H
#define GRAL_GB_UTILITY_FILE_UTILS_H

// $LICENSE_NEC_2007

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
