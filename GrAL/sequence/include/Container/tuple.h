#ifndef NMWR_GB_TUPLE_T_N_H
#define NMWR_GB_TUPLE_T_N_H


// $LICENSE


/*! \brief simple fixed-size container  with STL-style  begin() / end()
    \ingroup containers

    Tuple<T,N> is a model of STL Random Access Container (except reverse iterators).
 
    \todo
    Add reverse iterators.
*/

template<class T, unsigned N>
class tuple_base {
public:
  typedef       T*       iterator;
  typedef const T* const_iterator;
  typedef T value_type;
  typedef T c_array[N];


  // range-checking possible
  T& operator[](unsigned i)       { 
    check_range(i);
    return X[i];
  }
  T  operator[](unsigned i) const {return X[i];}

  iterator       begin() { return &(X[0]);}
  iterator       end()   { return (begin() + N);}
  const_iterator begin() const { return &(X[0]);}
  const_iterator end()   const { return (begin() + N);}

  unsigned size()   const { return N;}
  bool     empty()  const { return false;}
protected:
  T X[N];
private:
  void check_range(unsigned i) const {
    REQUIRE( ((0 <= i ) && (i < N)), 
	     " i = " << i << " must be in [0," << N << "]\n",1);
  }
};


template<class T, unsigned N>
class tuple : public tuple_base<T,N> {
  typedef tuple_base<T,N> base;

 public:
  // should not be necessary ... but problems with gcc 2.96
  typedef typename base::iterator       iterator;
  typedef typename base::const_iterator const_iterator;

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
  template<class It>
  tuple(It b, It e) {
    for(iterator it = begin(); it != end(); ++it,++b) 
      *it = *b; 
    REQUIRE( (b == e), "invalid range in constructor!\n",1);
  }
 
};

template<class T>
class tuple<T,3> : public tuple_base<T,3> {
  typedef tuple_base<T,3> base;
 public:
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
  template<class It>
  tuple(It b, It e) {
    for(iterator it = begin(); it != end(); ++it,++b) 
      *it = *b; 
    REQUIRE( (b == e), "invalid range in constructor!\n",1);
  }

  tuple(T const& t1, T const& t2, T const& t3) { X[0] = t1; X[1] = t2; X[2] = t3;}
};





template<class T, unsigned N>
inline
tuple<T,N> operator+(tuple<T,N> const& lhs, tuple<T,N> const& rhs)
{ 
  tuple<T,N> tmp(lhs);
  for(unsigned k = 0; k < N; ++k)
    tmp[k] += rhs[k];
  return tmp;
}

template<class T, unsigned N>
inline
tuple<T,N> operator-(tuple<T,N> const& lhs, tuple<T,N> const& rhs)
{ 
  tuple<T,N> tmp(lhs);
  for(unsigned k = 0; k < N; ++k)
    tmp[k] -= rhs[k];
  return tmp;
}

#endif
