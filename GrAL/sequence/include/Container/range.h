#ifndef NMWR_GB_SEQUENCE_PRINT_RANGE_H
#define NMWR_GB_SEQUENCE_PRINT_RANGE_H



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

/*! \defgroup iteratorrange Iterator ranges
    \ingroup accessors
*/

/*! \brief  Output for an iterator range
    \ingroup iteratorrange    

    \b Example:
    \code
     out << range(C.begin(), C.end());
    \endcode

*/

template<class It>
struct iter_range {
  It b;
  It e;
  std::string sep;
  std::string opening;
  std::string closing;
  iter_range(It bb, It ee, 
	     std::string s = " ",
	     std::string o = "", 
	     std::string c = "")
    : b(bb), e(ee), sep(s), opening(o), closing(c) {}
};

/*! \brief creator function for iter_range<T>
   \ingroup iteratorrange
 */
template<class It>
inline 
iter_range<It> range(It bb, It ee, 
		     std::string sep     = " ",
		     std::string opening = "", 
		     std::string closing = "") 
{ return iter_range<It>(bb,ee, sep, opening, closing); }

/*! \brief creator function for iter_range<T>
   
   Input is a container.
   \ingroup iteratorrange
 */
template<class C>
inline 
iter_range<typename C::const_iterator> range_c(C const& c,  
					       std::string sep     = " ",
					       std::string opening = "", 
					       std::string closing = "")
{ return iter_range<typename C::const_iterator>(c.begin(),c.end(), sep, opening, closing); }



/*! \brief output operator for iter_range<T>
   \ingroup iteratorrange
*/
template<class It>
inline
std::ostream& operator<<(std::ostream& out, iter_range<It> r)
{
  while(r.b != r.e) {
    out << r.opening <<  *(r.b) << r.closing << r.sep;
    ++r.b;
  }
  return out;
}

/*! \brief input operator for iter_range<T>
   \ingroup iteratorrange
*/
template<class It>
inline
std::istream& operator>>(std::istream& in,  iter_range<It> r)
{
  while(r.b != r.e) {
    in  >> *(r.b);
    ++r.b;
  }
  return in;
}

} // namespace GrAL 






#endif
