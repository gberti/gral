#ifndef NMWR_GB_GRID_REDUCTION_H
#define NMWR_GB_GRID_REDUCTION_H

//----------------------------------------------------------------
//   (c) Guntram Berti, 1998
//   Chair for Numerical Mathematics & Scientific Computing (NMWR)
//   TU Cottbus - Germany
//   http://math-s.math.tu-cottbus.de/NMWR
//   
//----------------------------------------------------------------

#include "algobase.h"


template<class ElemIter, class F>
inline
typename F::result_type it_range_min(ElemIter e, F f)
{
  typename F::result_type res = f(*e);
  while(! e.IsDone()) {
    res = min(res,f(*e));
    ++e;
  }
  return res;
}

template<class ElemSet, class F>
inline
typename F::result_type range_min(const ElemSet& S, F f)
{  return it_range_min(S.FirstElement(),f); }


template<class ElemSet, class F>
inline
typename F::result_type range_max(const ElemSet& S, F f)
{
  typename ElemSet::ElementIterator e = S.FirstElement();
  typename F::result_type res = f(*e);
  while(! e.IsDone()) {
    res = max(res,f(*e));
    ++e;
  }
  return res;
}


template<class ElemSet, class F, class T>
inline
T range_sum(const ElemSet& S, F f, T& res)
{
  typename ElemSet::ElementIterator e = S.FirstElement();
  while(! e.IsDone()) {
    res += f(*e);
    ++e;
  }
  return res;
}

/*
template<class ElemSet, class F>
inline
typename F::result_type
range_sum(const ElemSet& S, F f, T& res)
{
  typename F::result_type res(0);
  typename ElemSet::ElementIterator e = S.FirstElement();
  while(! e.IsDone()) {
    res += f(*e);
    ++e;
  }
  return res;
}
*/

template<class GF, class Iterator, class T>
void range_assign(GF& gf, Iterator e, const T& t)
{
  while(! e.IsDone()) {
    gf[*e] = t;
    ++e;
  }
}

template<class EltIter, class OutputIter>
EltIter copy_it_range(EltIter src, OutputIter dest)
{
  while(! src.IsDone()) {
    *dest = *src;
    ++dest; ++src;
  }
  return src;
}

template<class EltIter, class OutputIter, class F>
EltIter copy_it_range_filter(EltIter src, OutputIter dest, const F& f)
{
  while(! src.IsDone()) {
    *dest = f(*src);
    ++dest; ++src;
  }
  return src;
}

template<class EltIter, class OutputIter, class Pred>
EltIter copy_it_range_if(EltIter src, OutputIter dest, const Pred& p)
{
  while(! src.IsDone()) {
    if(p(*src))
      *dest = *src;
    ++dest; ++src;
  }
  return src;
}

template<class EltIter, class OutputIter, class Pred, class F>
EltIter copy_it_range_if_filter(EltIter src, OutputIter dest, const Pred& p, const F& f)
{
  while(! src.IsDone()) {
    if(p(*src))
      *dest = f(*src);
    ++dest; ++src;
  }
  return src;
}

#endif
