#ifndef GRAL_GB_UTILITY_SELECTIVE_OSTREAM_H
#define GRAL_GB_UTILITY_SELECTIVE_OSTREAM_H

// $LICENSE_NEC_2004



#include "Utility/pre-post-conditions.h"

#include <iostream>

namespace GrAL {

  /*! \brief Ostream wrapper which can be switched on and off
      
      \ingroup io

      Example:
      \code
      selective_ostream out(std::cout, 
                            (verbose ? 
			     selective_ostream::pass : 
			     selective_ostream::skip)); 
      \endcode

      \see Tested in \ref test-selective-ostream.C
   */
  class selective_ostream {
  public:
    //! \brief skip output or pass output to ostream
    enum flag { skip, pass}; 
  private:
    flag           f;
    std::ostream * out;
  public:
    selective_ostream() : f(skip), out(0) {}
    explicit
    selective_ostream(std::ostream& o, flag  ff = pass) : f(ff), out(&o) {}

    //! \brief Is output on?
    bool is_on()  const { return (f==pass);}
    //! \brief Is output off?
    bool is_off() const { return (f==skip);}
    //! \brief Switch output on
    void on () { REQUIRE_ALWAYS(out != 0, "cannot do output: no stream!",1); f = pass;}
    //! \brief Switch output off
    void off() { f = skip;}

    void redirect(std::ostream& new_out) { out = &new_out;}

    // manipulator like std::endl are templates, so we need a special
    // version to guide overloading resolution
    //! \brief Output for manipulators like \c std::endl
    selective_ostream operator<<( std::ostream& (*f)(std::ostream&) )
    {
      if(is_on())
	*out << f; 
      return *this;
    }

    // if this is on global namespace, problems with gcc 2.96
    //! \brief Output operator
    template<class T>
    selective_ostream operator<<(T const& t)
    {
      if(is_on())
	*out << t;
      return *this;
    } 
  };

} // namespace GrAL

#endif
