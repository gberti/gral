#ifndef NMWR_GB_SKIP_COMMENTS_H
#define NMWR_GB_SKIP_COMMENTS_H



// $LICENSE


#include <iostream>
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
 
  std::istream& the_istream() { return skip_comments(*in_);} 
  char begin_comment() const {return comment_begin_;}

  std::istream& skip_comments(std::istream& in) 
  {
    while(true) {
      in >> std::ws;
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
skip_comments_istream& operator>>(skip_comments_istream& in, std::string& s);



#endif
