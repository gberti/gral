#ifndef GRAL_GB_UTILITY_STRING_UTILS_H
#define GRAL_GB_UTILITY_STRING_UTILS_H

// $LICENSE_NEC_2004

#include <string>
#include <iostream>

namespace GrAL {

  /*! \defgroup stringalgorithms Algorithms on strings
   */

  /*! \brief Get the first non-empty line from stream \c in
      \ingroup stringalgorithms
      
      A line is considered empty if all characters before occurence of a character
      in \c line_end_chars are whitespaces (i.e. \c is_whitespace_only() is true for this part).
      By specifying additional characters like "#" in \c line_end_chars, 
      one can also filter out comment lines.
   */
  std::string get_non_empty_line(std::istream& in, std::string line_end_chars = "\n");

  /*! \brief Test if \c s consist of white space only
     \ingroup stringalgorithms
   */
  inline bool is_whitespace_only(std::string const& s) { return s.find_first_not_of(" \t\n\r\f") == std::string::npos; }

} // namespace GrAL

#endif
