#ifndef NMWR_GB_PRE_POST_CONDITION_H
#define NMWR_GB_PRE_POST_CONDITION_H




/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include <iostream>
#include <stdlib.h>

#ifdef GRAL_NO_ABORT
#include <exception>
#endif

//@{
  
//----------------------------------------------------------------
/*! \file pre-post-conditions.h
  \brief Some useful macros for checking pre/post conditions.

  Two different sets of macros are defined:
 - REQUIRE/ENSURE (condition, message, severity)
   are defined only if  \c NMWR_DEBUG is defined
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

 To modify the default behavior, you can define the macros
 GRAL_NO_ABORT (to throw an exception instead of aborting)
 and GRAL_NO_SHOW_DATE to inhibit printing date/time of compilation
 (useful for regression tests).
*/ 
//----------------------------------------------------------------


#define GRAL_ERRORLOG ::std::cerr
#define GRAL_ABORT ::abort() 
#define GRAL_DATE_INFO  " (compiled on " << __DATE__ << " at " __TIME__  << " )"


#ifdef  GRAL_NO_ABORT
#undef  GRAL_ABORT
#define GRAL_ABORT throw ::std::exception()
#endif

#ifdef  GRAL_NO_SHOW_DATE
#undef  GRAL_DATE_INFO
#define GRAL_DATE_INFO  ""
#endif

#ifdef  GRAL_NO_SHOW_FUNCTION
#define GRAL_FUNCTION_INFO ""
#else
#define GRAL_FUNCTION_INFO "      FUNCTION " << __FUNCTION__ << ", "
#endif

#ifdef  GRAL_NO_SHOW_LINE
#define GRAL_LINE_INFO ""
#else
#define GRAL_LINE_INFO "  LINE " << __LINE__ << ", "
#endif



#define GRAL_ERROR  GRAL_ERRORLOG \
   << "\n" \
   << "GRAL_ERROR in FILE "  << __FILE__ << "\n" \
   << GRAL_FUNCTION_INFO \
   << GRAL_LINE_INFO \
   << GRAL_DATE_INFO << "\n"


#define GRAL_PRECONDITION_ERROR   GRAL_ERROR  << "      precondition violated: "
#define GRAL_POSTCONDITION_ERROR  GRAL_ERROR  << "      postcondition violated: "




#define REQUIRE_ALWAYS(condition, error_msg, severity)\
 if(! (condition))  { GRAL_PRECONDITION_ERROR << #condition << ' ' << error_msg << ::std::endl; GRAL_ABORT;}

#define ENSURE_ALWAYS(condition, error_msg, severity)\
 if(! (condition)) { GRAL_POSTCONDITION_ERROR << #condition << ' ' << error_msg << ::std::endl; GRAL_ABORT;}

#ifdef NMWR_DEBUG

#define REQUIRE(condition, error_msg, severity)  REQUIRE_ALWAYS(condition, error_msg, severity) 
// if(! (condition))  { GRAL_PRECONDITION_ERROR << #condition << ' ' << error_msg << ::std::endl; GRAL_ABORT;}

#define ENSURE(condition, error_msg, severity) ENSURE_ALWAYS(condition, error_msg, severity)
// if(! (condition)) { GRAL_POSTCONDITION_ERROR << #condition << ' '  << error_msg << ::std::endl; GRAL_ABORT;}

#else
#define REQUIRE(condition, error_msg, severity) 
#define ENSURE(condition, error_msg, severity)
#endif

//@}

#endif
