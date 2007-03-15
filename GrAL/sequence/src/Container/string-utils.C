#include "Container/string-utils.h"

// $LICENSE_NEC_2004

namespace GrAL {
  
  std::string get_non_empty_line(std::istream& in, std::string line_end_chars) {
     std::string res;
     while((res.empty() || is_whitespace_only(res)) && in) {
       // char cline[255];
       // std::string line(255, '\n');
       // seems to be buggy for gcc 2.96
       std::string line;
       getline(in, line, '\n');
       //in.getline(cline,255,'\n');
       // std::string line = cline;
       res = line.substr(0, line.find_first_of(line_end_chars));
     }
     // std::cout << "Line: " << res << std::endl;
     return res;
  }

} // namespace GrAL
