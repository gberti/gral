#ifndef GRAL_GB_UTILITY_CONTEXT_H
#define GRAL_GB_UTILITY_CONTEXT_H

// $LICENCE_NEC_2007

#include <ostream>

namespace GrAL {

  /*! \brief Provide an abstraction layer  for I/O operations

   The purpose of this class is to intercept output from programs that is usually written
   to the command line directly using std::cerr etc. 
   If this is hard-coded, it is difficult to influence from the outside, except redirecting 
   the output of the whole program.

  \note This class is experimental.

   \todo Test
   \todo More complete design. Must be able to control context via command line, environment, maybe message.
  */
  class io_context {

    static std::ostream * info_stream;
    static std::ostream * null_stream;
    static std::ostream * warning_stream;
    static std::ostream * error_stream;
    
    
    enum message_type { info_type, warning_type, error_type, null_type };

    static void init();
    static std::ostream& get_stream(int type);

  public:
    static std::ostream& skip()    { return get_stream(null_type);}
    static std::ostream& info()    { return get_stream(info_type);}
    static std::ostream& warning() { return get_stream(warning_type);}
    static std::ostream& error()   { return get_stream(error_type);}

  }; // class io_context


} //  namespace GrAL

#endif
