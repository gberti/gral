#ifndef NMWR_GB_COPY_TRAITS_H
#define NMWR_GB_COPY_TRAITS_H

//----------------------------------------------------------------
//   (c) Guntram Berti, 1998
//   Chair for Numerical Mathematics & Scientific Computing (NMWR)
//   TU Cottbus - Germany
//   http://math-s.math.tu-cottbus.de/NMWR
//   
//----------------------------------------------------------------

//----------------------------------------------------------------
/*!
   \brief Compile-time branch on how to copy pointees.

    If we have a pointer to an abstract base of a polymorphic
    type, we cannot copy the content by simply calling <TT>operator new</TT>.
    Instead, we must use some virtual member function like <TT>clone()</TT>

    This decision is hiden by <TT> copy_traits<T>::clone()</TT>
    Default implementation for non-polymorphic types
    is simply operator new.

    For T being an abstract class, this implmentation
    has to be overridden with
    a method like <TT>T::clone()</TT>
    by specializing <TT> copy_traits<> </TT> for T..

*/
//----------------------------------------------------------------

template<class T>
struct copy_traits {
  static T* clone(T const& t) { return (new T(t));}
};


#endif
