#ifndef NMWR_GB_PRINT_RANGE_H
#define NMWR_GB_PRINT_RANGE_H


// $LICENSE

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
  iter_range(It bb, It ee) : b(bb), e(ee) {}
};

/*! \brief creator function for iter_range<T>
   \ingroup iteratorrange
 */
template<class It>
inline 
iter_range<It> range(It bb, It ee) 
{ return iter_range<It>(bb,ee); }


/*! \brief output operator for iter_range<T>
   \ingroup iteratorrange
*/
template<class It>
inline
std::ostream& operator<<(::std::ostream& out, iter_range<It> r)
{
  while(r.b != r.e) {
    out << *(r.b) << ' ';
    ++r.b;
  }
  return out;
}

/*! \brief inputput operator for iter_range<T>
   \ingroup iteratorrange
*/
template<class It>
inline
std::istream& operator>>(::std::istream& in, iter_range<It> r)
{
  while(r.b != r.e) {
    in  >> *(r.b);
    ++r.b;
  }
  return in;
}

} // namespace GrAL 

#endif
