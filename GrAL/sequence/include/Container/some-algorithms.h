#ifndef NMWR_CONTAINER_ALGORITHMS_H
#define NMWR_CONTAINER_ALGORITHMS_H


// $LICENSE


/*! \file 
  Some extensions to STL algorithms
*/

template<class InputIt>
inline
InputIt next(InputIt i)
{ ++i; return i;}

template<class InputIt>
inline
InputIt next(InputIt i, unsigned n)
{ 
  while(n > 0) {
    ++i; 
    --n;
  }
  return i;
}

/*! \brief copy [b,e) to [dest, ...), mapping values with f.
    \ingroup algorithms

    The output is the sequence f(*b), ... f(*(e-1)).

    This algorithm could also be expressed using std::copy
    and an iterator-adaptor of InputIt using Filter.
    However, as a filter rather operates on the
    corresponding value_type, this version seems to be 
    more natural.

   \todo Use std::transform(b,e,dest,f) instead.
*/
template<class InputIt, class OutputIt, class Filter>
inline
OutputIt copy_filter(InputIt b, InputIt e, OutputIt dest, Filter f)
{
  while( b != e) {
    *dest = f(*b);
    ++b; ++dest;
  }
  return dest;
}

/*! \brief find_if_preference tries to find an element that satisfies both predicates.
    \ingroup algorithms

 Return value:
 -# i, where i is the first in [b,e) that satisfies must_be && should_be
 -# i, where i is the first in [b,e) that satisfies must_be, 
        and no element in [b,e) satisfies should_be
 -# e, if no element in [b,e) satisfies must_be.
*/
template<class FwdIt, class P1, class P2>
inline
FwdIt find_if_preference(FwdIt b, FwdIt e, const P1& must_be, const P2& should_be)
{
  FwdIt found = e;
  while( b != e) {
    if(must_be(*b)) {
     if(should_be(*b))
       return b;
     else
       found = b;
    }
    ++b;
  }
  return found;
}

/*! \brief Copy a mapping by iterating through its domain
    \ingroup algorithms

    <b> Template parameters </b>
    - M1: operator[]
    - M2:
      - operator[]
      - typedef domain_type
      - domain_type domain()
 */
template<class M1, class M2>
void mapping_assign(M1& dest, M2 const& src)
{
  typedef typename M2::domain_type dom_type;
  typedef typename dom_type::const_iterator dom_iter;
  dom_iter end = src.domain().end();
  for(dom_iter d = src.domain().begin(); d != end; ++d)
    dest[*d] = src[*d];
}

#endif
