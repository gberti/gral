#ifndef GRAL_GB_CONTAINER_BACK_PUSHER_H
#define GRAL_GB_CONTAINER_BACK_PUSHER_H

/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti,
    Contact: guntram AT users DOT berlios DOT de

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */

#include <iostream>

namespace GrAL {

  namespace detail {
    struct reader {
      template<class T>
      static void read(std::istream& in, T& t) { in >> t;}
    };
  }

  /*! \brief Utility to read items into vector
    
  \ingroup helperfunctors
  This class is particularly useful when reading repeated items from the 
  commandline into a container.

  \templateparams
  - CONTAINER: standard container, having \c push_back(T)
  - READER: configuration hook, having \c read(istream&, T&)
  
  Example 1:
  \code
  std::vector<int> v;
  GrAL::back_pusher<std::vector<int> > rv(v);
  // read 3 integers into v 
  cin >> rv >> rv >> rv;
  \endcode

  Example 2:
  \code
   GrAL::CtrlDevice Ctrl = ...
   std::vector<int> seeds;
   GrAL::back_pusher<std::vector<int> > read_seeds(seeds);
   GrAL::RegisterAt(Ctrl, "+i", read_seeds);
  \endcode

  The program can be called with repeated \c +i arguments:

  \code
   $ myprog +i 2 +i 44 +i 63
  \endcode
  */
template<class CONTAINER, class READER = detail::reader>
class back_pusher {
private:
  CONTAINER* cont;
public:
  back_pusher() : cont(0) {}
  back_pusher(CONTAINER& c) : cont(&c) {}

  void read(std::istream& in)
    { 
      REQUIRE_ALWAYS( cont != 0, "", 1);
      typename CONTAINER::value_type tmp;  
      READER::read(in,tmp);
      cont->push_back(tmp);
    }
};


  template<class CONTAINER, class READER>
inline
std::istream& operator>>(std::istream& in, 
			 back_pusher<CONTAINER, READER> & b)
{
  b.read(in);
  return in;
}

template<class CONTAINER, class READER>
inline
std::ostream& operator<<(std::ostream& out, 
			 back_pusher<CONTAINER, READER> & b)
{
  return out;
}

} // namespace GrAL 

#endif
