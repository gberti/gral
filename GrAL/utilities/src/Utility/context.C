
/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */

#include "Utility/context.h"

#include <iostream>
#include <fstream>
#include <streambuf>

namespace GrAL {
  



  /*! \brief Ostream ignoring any output.

    \note Stolen from <a href="http://www.thescripts.com/forum/thread428285.html">here</a>.
   */
  template <class cT, class traits = std::char_traits<cT> >
  class basic_onullstream: public std::basic_ostream<cT, traits> {

    class basic_nullbuf: public std::basic_streambuf<cT, traits> {
      typename traits::int_type overflow(typename traits::int_type c)
      {
	return traits::not_eof(c); // indicate success
      }
  };
  public:
    basic_onullstream():
      std::basic_ios    <cT, traits>(&m_sbuf),
      std::basic_ostream<cT, traits>(&m_sbuf)
    {
      init(&m_sbuf);
    }
    
  private:
    basic_nullbuf  m_sbuf;
  };

  typedef basic_onullstream<char>    onullstream;
  typedef basic_onullstream<wchar_t> wonullstream;


  void io_context::init()
  {
    info_stream    = & std::cerr;
    warning_stream = & std::cerr;
    error_stream   = & std::cerr;
    null_stream    = new onullstream;  
    // not portable:
    // null_stream = new std::ofstream("/dev/null");
  } 

  std::ostream& io_context::get_stream(int type) {
    static bool initialised = false;
    if(! initialised) {
      io_context::init();
      initialised = true;
    }
    return (type == info_type ? *info_stream 
	    : ( type == warning_type ? *warning_stream
		: ( type == error_type ?  *error_stream : *null_stream)));
  }

  std::ostream* io_context::info_stream    = 0;
  std::ostream* io_context::warning_stream = 0;
  std::ostream* io_context::error_stream   = 0;
  std::ostream* io_context::null_stream    = 0;

} // namespace GrAL
