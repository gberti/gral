#ifndef TRIANGLE_OPTIONS_PARSER
#define TRIANGLE_OPTIONS_PARSER


/* ------------------------------------------------------------

    Copyright (C) 2005 - 2009 Roman Putanowicz

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */

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
TriangleOptionsParser::Parse() {
    while(yylex() != 0){
    }
}

inline void 
TriangleOptionsParser::Parse(const char* s) {
     options_text="\'";
     options_text+=s;
     options_text+="\'";
     std::istringstream is(s);
     switch_streams(&is, 0);
     Parse();
}

#endif
