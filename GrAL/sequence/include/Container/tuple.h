#ifndef NMWR_GB_TUPLE_T_N_H
#define NMWR_GB_TUPLE_T_N_H

///////////////////////////////////////////
//
//  simple fixed-size container
//  with "STL- begin() / end()"
//
///////////////////////////////////////////
#include "Work-around/do-nothing-but-suppress-unused-warning.h"

template<class T, unsigned N>
class tuple {
public:
  typedef       T*       iterator;
  typedef const T* const_iterator;
  typedef T value_type;
  typedef T c_array[N];

  tuple() {}
  tuple(const T& t)        { for(iterator i = begin(); i != end(); ++i)  *i = t;}
  tuple(const c_array& rs) { for(iterator i = begin(); i != end(); ++i)  *i = rs[i-begin()];}
  tuple(const_iterator b, const_iterator /*e */)
  { 
    // REQUIRE(((e-b) == N),
    // 	       "tuple<T," << N << ">::tuple(b,e) :  e-b = " << e-b << "!" ,1)
    for(iterator it = begin(); it != end(); ++it,++b) 
      *it = *b; 
  }

  // range-checking possible
  T& operator[](unsigned i)       {return X[i];}
  T  operator[](unsigned i) const {return X[i];}

  iterator       begin() { return &(X[0]);}
  iterator       end()   { return (begin() + N);}
  const_iterator begin() const { return &(X[0]);}
  const_iterator end()   const { return (begin() + N);}

  unsigned size() const { return N;}
  bool empty()    const { return false;}
private:
  T X[N];
};

#endif
