#ifndef NMWR_CONTAINER_ALGORITHMS_H
#define NMWR_CONTAINER_ALGORITHMS_H

//----------------------------------------------------------------
//   (c) Guntram Berti, 1998
//   Chair for Numerical Mathematics & Scientific Computing (NMWR)
//   TU Cottbus - Germany
//   http://math-s.math.tu-cottbus.de/NMWR
//   
//----------------------------------------------------------------


//----------------------------------------------------------------
//
//  some extensions to STL algorithms.
//
//----------------------------------------------------------------

template<class InputIt>
InputIt next(InputIt i)
{ ++i; return i;}

template<class InputIt>
InputIt next(InputIt i, unsigned n)
{ 
  while(n > 0) {
    ++i; 
    --n;
  }
  return i;
}

// this algorithm could also be expressed using std copy
// and an iterator-adaptor of InputIt using Filter,
// however, as a filter rather operates on the
// corresponding value_type, this version seems to be 
// more natural.
template<class InputIt, class OutputIt, class Filter>
OutputIt copy_filter(InputIt b, InputIt e, OutputIt dest, Filter f)
{
  while( b != e) {
    *dest = f(*b);
    ++b; ++dest;
  }
  return dest;
}

// find_if_preference tries to find an element that satisfies both
// predicates.
// Return value:
//    i, where i is the first in [b,e) that satisfies must_be && should_be
// or i, where i is the first in [b,e) that satisfies must_be, 
//       and no element in [b,e) satisfies should_be
// e if no element in [b,e) satisfies must_be.
template<class FwdIt, class P1, class P2>
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
