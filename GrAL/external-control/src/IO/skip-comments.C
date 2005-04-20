
// $LICENSE

#include "IO/skip-comments.h"

namespace GrAL {


template<>
skip_comments_istream& operator>>(skip_comments_istream& in, std::string& s)
{
  std::istream& in1(in.the_istream());
  int max_len = 256;
  char* ss = new char[max_len];
  // string ss = "";
  int i = 0;
  char c = 'a';
  while(in1 && i < max_len-1 && (c != in.begin_comment()) && ! isspace(c)) {
    c = in1.get();
    ss[i] = c;
    i++;
  };
  
  if(c == in.begin_comment() || isspace(c)) {
    if(in1)
      in1.putback(c);
    i--;
  }

   ss[i] = '\0';
  s = ss;
  return in;
}


} // namespace GrAL 
