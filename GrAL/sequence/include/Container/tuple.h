#ifndef NMWR_GB_TUPLE_T_N_H
#define NMWR_GB_TUPLE_T_N_H


// $LICENSE

#include <iostream>
#include "Utility/pre-post-conditions.h"
#include "Utility/numeric-types.h"
#include "Utility/algebraic-operators.h"
#include "Utility/relational-operators.h"

// 'internal' class 
template<class T, unsigned N>
class tuple_base {
public:
  typedef       T*       iterator;
  typedef const T* const_iterator;
  typedef T value_type;
  typedef T c_array[N];


  // range-checking possible
  T& operator[](unsigned i)       {check_range(i); return X[i];}
  T  operator[](unsigned i) const {check_range(i); return X[i];}
  T  operator()(unsigned i) const {check_range(i); return X[i];}

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
    REQUIRE( (i < N), 
	     " i = " << i << " must be in [0," << N << "]\n",1);
  }
};


/*! \brief simple fixed-size container  with STL-style  begin() / end()
    \ingroup containers

    tuple<T,N> is a model of STL Random Access Container (except reverse iterators).
 
    \todo Add reverse iterators.
*/

template<class T, unsigned N>
class tuple : public tuple_base<T,N> {
  typedef tuple_base<T,N> base;

 public:
  // should not be necessary ... but problems with gcc 2.96
  typedef typename base::iterator       iterator;
  typedef typename base::const_iterator const_iterator;
  typedef typename base::c_array        c_array;

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
  template<class U>
  tuple(tuple<U,N> const& rhs) 
   { for(int i = 0; i < N; ++i) X[i] = rhs[i];}

  tuple<T,N> operator-() const { tuple<T,N> res; for(int i = 0; i < N; ++i) res[i] = -X[i]; return res;}
  
  tuple<T,N>& operator+=(tuple<T,N> const& rhs) { for(int i = 0; i < N; ++i) X[i] += rhs[i]; return *this;}
  tuple<T,N>& operator*=(T a)                   { for(int i = 0; i < N; ++i) X[i] *= a;      return *this;}
};

template<class T>
class tuple<T,2> : public tuple_base<T,2> {
  typedef tuple_base<T,2> base;
 public:
  // should not be necessary ... but problems with gcc 2.96
  typedef typename base::iterator       iterator;
  typedef typename base::const_iterator const_iterator;
  typedef typename base::c_array        c_array;

  tuple() {}
  tuple(const T& t)        { for(iterator i = begin(); i != end(); ++i)  *i = t;}
  tuple(const c_array& rs) { for(iterator i = begin(); i != end(); ++i)  *i = rs[i-begin()];}
  tuple(const_iterator b, const_iterator /*e */)
  { 
    for(iterator it = begin(); it != end(); ++it,++b) 
      *it = *b; 
  }
  /*
  template<class It>
  tuple(It b, It e) {
    for(iterator it = begin(); it != end(); ++it,++b) 
      *it = *b; 
    REQUIRE( (b == e), "invalid range in constructor!\n",1);
  }
  */
  tuple(T const& t1, T const& t2) { X[0] = t1; X[1] = t2;}

  template<class U>
  tuple(tuple<U,2> const& rhs) { X[0] = rhs[0]; X[1] = rhs[1];}

  T  x() const { return X[0];}
  T  y() const { return X[1];}
  T& x()       { return X[0];}
  T& y()       { return X[1];}

  tuple<T,2> operator-() const { return tuple<T,2>(-X[0],-X[1]);}
  tuple<T,2>& operator+=(tuple<T,2> const& rhs) { for(int i = 0; i < 2; ++i) X[i] += rhs[i]; return *this;}
  tuple<T,2>& operator*=(T a)                   { for(int i = 0; i < 2; ++i) X[i] *= a;      return *this;}

};

template<class T>
class tuple<T,3> : public tuple_base<T,3> {
  typedef tuple_base<T,3> base;
 public:
  // should not be necessary ... but problems with gcc 2.96
  typedef typename base::iterator       iterator;
  typedef typename base::const_iterator const_iterator;
  typedef typename base::c_array        c_array;

  tuple() {}
  tuple(const T& t)        { for(iterator i = begin(); i != end(); ++i)  *i = t;}
  tuple(const c_array& rs) { for(iterator i = begin(); i != end(); ++i)  *i = rs[i-begin()];}
  tuple(const_iterator b, const_iterator /*e */)
  { 
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

  template<class U>
  tuple(tuple<U,3> const& rhs) { X[0] = rhs[0]; X[1] = rhs[1]; X[2] = rhs[2];}


  T  x() const { return X[0];}
  T  y() const { return X[1];}
  T  z() const { return X[2];}
  T& x()       { return X[0];}
  T& y()       { return X[1];}
  T& z()       { return X[2];}

  tuple<T,3> operator-() const { return tuple<T,3>(-X[0],-X[1],-X[2]);}
  tuple<T,3>& operator+=(tuple<T,3> const& rhs) { for(int i = 0; i < 3; ++i) X[i] += rhs[i]; return *this;}
  tuple<T,3>& operator*=(T a)                   { for(int i = 0; i < 3; ++i) X[i] *= a;      return *this;}
};

template<class T>
class tuple<T,0>
{
public:
  template<class IT>
  tuple(IT beg, IT end) { REQUIRE(beg==end, "", 1);}
  tuple()  {}
  tuple(T) {}

  unsigned size() const { return 0;}
  typedef T const* iterator;

  T & operator[](unsigned i)       { REQUIRE(false, "", 1);  return tt;}
  T   operator[](unsigned i) const { REQUIRE(false, "", 1);  return 0;}
  T   operator()(unsigned i) const { REQUIRE(false, "", 1);  return 0;}

  iterator begin() const { return 0;}  
  iterator end()   const { return 0;}  

  static T tt;
};


template<class T> T tuple<T,0>::tt;


//-------- comparison operators  ----------

template<class T, unsigned N, class RelOperator>
inline bool compare(tuple<T,N> const& lhs, tuple<T,N> const& rhs, RelOperator op)
{
  bool res = true;
  for(unsigned k = 0; k < N; ++k)
    res = res && (op(lhs[k],rhs[k]));
  return res;
}

template<class T, unsigned N>
inline bool operator==(tuple<T,N> const& lhs, tuple<T,N> const& rhs)
{ return compare(lhs,rhs, relational_operators::eq());}

template<class T, unsigned N>
inline bool operator!=(tuple<T,N> const& lhs, tuple<T,N> const& rhs)
{ return !(lhs == rhs);}

/*! \brief Component-wise less.

    \note This is \e different from the lexicographical less:
          \f$ a < b \f$ means \f$ a[i] < b[i] \forall i \f$, 
          whereas lexicographical less means
          \f$ a[i] = b[i], \quad 0 \leq i < k  \quad \wedge \quad a[k] < b[k] \f$
 */
template<class T, unsigned N>
inline bool operator< (tuple<T,N> const& lhs, tuple<T,N> const& rhs)
{ return compare(lhs,rhs, relational_operators::lt());}

/*! \brief Component-wise greater.

    \note This is \e different from the lexicographical greater:
          \f$ a > b \f$ means \f$ a[i] > b[i] \forall i \f$, 
          whereas lexicographical greater means
          \f$ a[i] = b[i] \quad 0 \leq i < k  \quad \wedge \quad  a[k] > b[k] \f$
 */

template<class T, unsigned N>
inline bool operator> (tuple<T,N> const& lhs, tuple<T,N> const& rhs)
{ return compare(lhs,rhs, relational_operators::gt());}


template<class T, unsigned N>
inline bool operator<=(tuple<T,N> const& lhs, tuple<T,N> const& rhs)
{ return compare(lhs,rhs, relational_operators::leq());}

template<class T, unsigned N>
inline bool operator>=(tuple<T,N> const& lhs, tuple<T,N> const& rhs)
{ return compare(lhs,rhs, relational_operators::geq());}



//------  algebraic operators --------------

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

template<class T, unsigned N>
inline
tuple<T,N> operator*(T t, tuple<T,N> const& rhs)
{
  tuple<T,N> tmp;
  for(unsigned k = 0; k < N; ++k)
    tmp[k] = t*rhs[k];
  return tmp;
}

template<class T, unsigned N>
inline
tuple<T,N> operator*(tuple<T,N> const& lhs, T t)
{ return t*lhs;}

template<class T, unsigned N>
inline
tuple<T,N> operator/(tuple<T,N> const& lhs, T t)
{ return lhs * (1/t);}


template<class T, unsigned N>
inline
std::ostream& operator<<(std::ostream& out, tuple<T,N> const& t)
{
  for(unsigned i = 0; i < N; ++i)
    out << t[i] << (i < N-1 ? " " : "");
  return out;
}

template<class T, unsigned N>
inline
std::istream& operator>>(std::istream& in, tuple<T,N> & t)
{
  for(unsigned i = 0; i < N; ++i)
    in >> t[i];
  return in;
}


//----- component-wise algebraic operations

template<class T, class U, unsigned N, class BinaryAlgebraicOp>
inline 
tuple<typename numeric_types::promote<T,U>::type, N>
componentwise(tuple<T,N> const& lhs, tuple<U,N> const& rhs, BinaryAlgebraicOp op)
{ 
  //tuple<T,N> res;
  tuple<typename numeric_types::promote<T,U>::type, N> res;
  for(unsigned i = 0; i < N; ++i)
    res[i] = op(lhs[i], rhs[i]);
  return res;
}


/*! \brief component-wise product

    We do not use \c operator* syntax which could be confused with the dot-product.
*/
template<class T, class U, unsigned N>
inline
tuple<typename numeric_types::promote<T,U>::type, N>
product(tuple<T,N> const& lhs, tuple<U,N> const& rhs)
{ return componentwise(lhs,rhs, algebraic_operators::multiply()); }

/*! \brief component-wise quotient.

    For consistency with product(), the \c operator/ syntax is not used.
*/
template<class T, class U, unsigned N>
inline
tuple<typename numeric_types::promote<T,U>::type, N>
quotient(tuple<T,N> const& lhs, tuple<U,N> const& rhs)
{ return componentwise(lhs,rhs, algebraic_operators::divide()); }

/*! \brief component-wise power

   \f$ (a_0, \ldots, a_{N-1}) \mapsto (a_0^k, \ldots, a_{N-1}^k) \f$
*/
template<class T, class U, unsigned N>
inline
tuple<T,N>
power(tuple<T,N> const& a, U k)
{ 
  tuple<T,N> res(1);
  for(unsigned i = 0; i < N; ++i)
    for(U kk = 0; kk < k; ++kk)
      res[i] *= a(i);
  return res;
}


/*!  \brief Test for component-wise divisibility

     This makes sense only for integral types!
     
     <tt> does_divide(lhs, rhs) == true </tt>
     \f$ \Leftrightarrow \f$
     \c lhs[i] divides \c rhs[i] \f$ \forall i \f$
 */
template<class T, class U, unsigned N>
inline bool
does_divide(tuple<T,N> const& lhs, tuple<U,N> const& rhs)
{ return rhs == product(lhs, quotient(rhs,lhs)); }





#endif
