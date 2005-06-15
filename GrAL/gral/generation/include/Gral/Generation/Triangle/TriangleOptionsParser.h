#ifndef TRIANGLE_OPTIONS_PARSER
#define TRIANGLE_OPTIONS_PARSER

#undef yyFlexLexer
#define yyFlexLexer TriangleFlexLexer
#include  "FlexLexer.h"
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <exception>

class TriangleOptionsException : public std::exception {
  public:
      TriangleOptionsException()  {}
      TriangleOptionsException(const std::string &message) : _message(message){}
      TriangleOptionsException(const char * message) : _message(message) {}
      ~TriangleOptionsException() throw() {}
      virtual const char* what() const throw() {
        return _message.c_str();
      };
  private:
      std::string _message;
};

#include "TriangleOptionsParser_classdecl.h"

inline void 
TriangleOptionsParser::parse() {
    while(yylex() != 0){
    }
}

inline void 
TriangleOptionsParser::parse(const char* s) {
     options_text="\'";
     options_text+=s;
     options_text+="\'";
     std::istringstream is(s);
     switch_streams(&is, 0);
     parse();
}

#endif
