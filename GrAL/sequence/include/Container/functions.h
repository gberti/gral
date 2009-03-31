#ifndef NMWR_GB_CONTAINER_FUNCTIONS_H
#define NMWR_GB_CONTAINER_FUNCTIONS_H


/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include "Config/compiler-config.h"

#include <functional>

/*! \brief Things still missing from C++ standard

    Probably we should use $boost instead.
 */
namespace stdext {

  /*! \brief Identity function
   */
  template <class T>
  struct identity : public std::unary_function<T, T> {
    const T& operator()(const T& x) const { return x; }
  };

  /*! \brief Composition of two Adaptable Unary Functions

  stolen from SGI STL 
  */
  template <class _Operation1, class _Operation2>
  class unary_compose
    : public std::unary_function<typename _Operation2::argument_type,
				 typename _Operation1::result_type> 
  {
  protected:
    _Operation1 __op1;
    _Operation2 __op2;
  public:
    unary_compose(const _Operation1& __x, const _Operation2& __y) 
      : __op1(__x), __op2(__y) {}
    typename _Operation1::result_type
    operator()(const typename _Operation2::argument_type& __x) const {
      return __op1(__op2(__x));
    }
  };
  
  /*! \brief Composition of two Adaptable Unary Functions

      Stolen from SGI STL 
  */
  template <class _Operation1, class _Operation2>
  inline unary_compose<_Operation1,_Operation2> 
  compose1(const _Operation1& __op1, const _Operation2& __op2)
  {
    return unary_compose<_Operation1,_Operation2>(__op1, __op2);
  }
  
  /*! \brief Composition of two  Adaptable Unary Functions and a Adaptable Binary Function

    Stolen from SGI STL 
  */
  template <class _Operation1, class _Operation2, class _Operation3>
  class binary_compose
    : public std::unary_function<typename _Operation2::argument_type,
				 typename _Operation1::result_type> {
  protected:
    _Operation1 _M_op1;
    _Operation2 _M_op2;
    _Operation3 _M_op3;
  public:
    binary_compose(const _Operation1& __x, const _Operation2& __y, 
		   const _Operation3& __z) 
      : _M_op1(__x), _M_op2(__y), _M_op3(__z) { }
    typename _Operation1::result_type
    operator()(const typename _Operation2::argument_type& __x) const {
      return _M_op1(_M_op2(__x), _M_op3(__x));
    }
  };
  
  /*! \brief Composition of two Adaptable Unary Functions and a Adaptable Binary Function

    Stolen from SGI STL 
  */
  template <class _Operation1, class _Operation2, class _Operation3>
  inline binary_compose<_Operation1, _Operation2, _Operation3> 
  compose2(const _Operation1& __op1, const _Operation2& __op2, 
	   const _Operation3& __op3)
  {
    return binary_compose<_Operation1,_Operation2,_Operation3>
      (__op1, __op2, __op3);
  }
  
} // namespace stdext

#endif
