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
//
// copy_traits<T> : c-t branch on how to create a dynamically 
//    allocated copy of an object t.
//    Default is simply operator new.
//    For T being an abstract class, this has to be overridden with
//    a method like T::clone() by specializing copy_traits<>.
//
//----------------------------------------------------------------

template<class T>
struct copy_traits {
  static T* clone(T const& t) { return (new T(t));}
};


#endif
