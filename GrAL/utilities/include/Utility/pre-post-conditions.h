#ifndef NMWR_GB_PRE_POST_CONDITION_H
#define NMWR_GB_PRE_POST_CONDITION_H



// $LICENSE

#include <iostream.h>
#include <stdlib.h>

//@{
  
//----------------------------------------------------------------
/*! \file pre-post-conditions.h
  \brief Some useful macros for checking pre/post conditions.

  Two different sets of macros are defined:
 - REQUIRE/ENSURE (condition, message, severity)
   are defined only #ifdef DEBUG
 - REQUIRE_ALWAY/ENSURE_ALWAYS (condition, message, severity)
   are always defined

 Example:
 \code
 int* f(int* p) {
  REQUIRE((p != 0), "Pointer null!", 1);
  // work on p ...
  ENSURE((p != 0), "Pointer null!", 1);
 }
 \endcode
*/ 
//----------------------------------------------------------------


#define _ERRORLOG cerr

#define _PRECONDITION_ERROR  _ERRORLOG << "\nERROR in FILE "  << __FILE__ << ", LINE " << __LINE__\
   << "\n(compiled on " << __DATE__ << " at " __TIME__  << " )\n"\
   << "precondition violated:\n"

#define _POSTCONDITION_ERROR  _ERRORLOG << "\nERROR in FILE "  << __FILE__ << ", LINE " << __LINE__\
   << "\n(compiled on " << __DATE__ << " at " __TIME__  << " )\n"\
                                       << ": postcondition violated:\n"

#define REQUIRE_ALWAYS(condition, error_msg, severity)\
 if(! (condition))  { _PRECONDITION_ERROR << #condition << ' ' << error_msg << endl; abort();}

#define ENSURE_ALWAYS(condition, error_msg, severity)\
 if(! (condition)) { _POSTCONDITION_ERROR << #condition << ' ' << error_msg << endl; abort();}

#ifdef NMWR_DEBUG

#define REQUIRE(condition, error_msg, severity)\
 if(! (condition))  { _PRECONDITION_ERROR << #condition << ' ' << error_msg << endl; abort();}

#define ENSURE(condition, error_msg, severity)\
 if(! (condition)) { _POSTCONDITION_ERROR << #condition << ' '  << error_msg << endl; abort();}

#else
#define REQUIRE(condition, error_msg, severity) 
#define ENSURE(condition, error_msg, severity)
#endif

//@}
/*
template <class STRING>
void _handle_my_errors(
*/

#endif
