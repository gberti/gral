#ifndef NMWR_GB_SKIP_COMMENTS_H
#define NMWR_GB_SKIP_COMMENTS_H


//----------------------------------------------------------------
//   (c) Guntram Berti, 1997
//   Chair for Numerical Mathematics & Scientific Computing (NMWR)
//   TU Cottbus - Germany
//   http://math-s.math.tu-cottbus.de/NMWR
//   
//----------------------------------------------------------------


#include <iostream.h>
#include <limits.h>
#include <string>

class skip_comments_istream {
public:
  skip_comments_istream(std::istream& in, 
			char comment_begin = '#',
			char comment_end   = '\n') 
    : in_(&in),
      comment_begin_(comment_begin), 
      comment_end_(comment_end) {}
 
  istream& the_istream() { return skip_comments(*in_);} 
  char begin_comment() const {return comment_begin_;}

  istream& skip_comments(std::istream& in) 
  {
    while(true) {
      in >> ws;
      if (in.peek() != comment_begin_)
	break;
      in.get();
      in.ignore(INT_MAX,comment_end_);
    }
    return in;
  } 

private:
  std::istream* in_;
  char comment_begin_, comment_end_;
};

inline std::istream& skip_comment(std::istream& in) {
  if(in) {
    skip_comments_istream sk(in);
    sk.skip_comments(in);
  }
  return in;
}

// Spezialfall fuer strings: sonst probleme bei
// xyz#Kommentar : wird als *ein* string gelesen,
// und nicht als:  xyz ( #Kommentar wird ueberlesen).
// scheint soweit zu klappen.

inline skip_comments_istream& operator>>(skip_comments_istream& in, std::string& s)
{
  istream& in1(in.the_istream());
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


template<class T>
inline skip_comments_istream& operator>>(skip_comments_istream& in, T& t)
{ 
  in.the_istream() >> t;
  return in;
}

// special case for strings:
// if we had e.g. blurb#comment in an input,
// it would be read as one string.

template<>
inline skip_comments_istream& operator>>(skip_comments_istream& in, std::string& s);



#endif
