#ifndef GRAL_GB_GEOMETRY_ARRAY_OPERATIONS_H
#define GRAL_GB_GEOMETRY_ARRAY_OPERATIONS_H

// $LICENSE_NEC_2007

#include "Container/algebraic-operators.h"
#include "Container/mathematical-operators.h"
#include "Geometry/point-traits.h"

#include <boost/utility/enable_if.hpp>
#include <boost/type_traits.hpp>

namespace GrAL {



  /*! \defgroup compwise_arrayoperations Component-wise array operations

      This modules defines a number of component-wise operations on array-like types.
      A common precondion on the type is an existing specialisation of \ref \c GrAL::point_traits.
    
      The operations use <a href="http://www.boost.org/libs/utility/enable_if.html" target="_parent">boost::enable_if</a>
      to control overload resolution. Specialise the metafunctions \c use_componentwise_operations
      and \c use_componentwise_operations_with_scalar to enable (default) or disable these operations.

     \see Tests in \ref test-array-operations.C     

     \todo Relational operators, with scalar, power, divides

  
  */


  /*! \brief Enabling metafunction for componentwise array operations

      \ingroup compwise_arrayoperations

      By default, using componentwise operations is enabled for a type \c P
      iff \c point_traits<P> is specialised.

      To switch it off for a type \c myPoint, use
      \code
      template<> struct use_componentwise_operations<myPoint>
      { enum { value = 0 }; };
      \endcode
 
      Finer control is provided by its derived classes.
  */
  template<class P>
  struct use_componentwise_operations {
    enum { value = point_traits<P>::is_specialised };
  };

  template<> struct use_componentwise_operations<unsigned char> { enum { value = 0 }; };
  template<> struct use_componentwise_operations<signed char>   { enum { value = 0 }; };
  template<> struct use_componentwise_operations<int>           { enum { value = 0 }; };
  template<> struct use_componentwise_operations<unsigned int>  { enum { value = 0 }; };
  template<> struct use_componentwise_operations<long long int> { enum { value = 0 }; };
  template<> struct use_componentwise_operations<float>         { enum { value = 0 }; };
  template<> struct use_componentwise_operations<double>        { enum { value = 0 }; };

  template<class P, class S, int USE>
  struct use_component_wise_ops_with_scalar_aux 
  {
    typedef point_traits<P> pt;
    enum { value = boost::is_convertible<S, typename pt::component_type>::value };
  };

  template<class P, class S>
  struct use_component_wise_ops_with_scalar_aux<P,S,0> {
    enum { value = 0 };
  };

  /*! \brief  Enabling metafunction for componentwise array operations involving a scalar

      \ingroup compwise_arrayoperations
    
        By default, using componentwise operations with a scalar of type \c S 
        is enabled for a type \c P
	iff use of componentwise operations is enabled for \c P and \c S is convertible to
	the component type of \c P.
   */
  template<class P, class S>
  struct use_componentwise_operations_with_scalar 
    : public use_component_wise_ops_with_scalar_aux<P,S, use_componentwise_operations<P>::value>
  {};


  /*! \brief Generic binary componentwise operations

      \ingroup compwise_arrayoperations
   */
  template<class P, class BinaryAlgebraicOp>
  inline 
  typename boost::enable_if<use_componentwise_operations<P>, P>::type
  componentwise(P const& lhs, P const& rhs,  BinaryAlgebraicOp op)
  {
    P res;
    typedef point_traits<P> pt;
    for(int i = pt::LowerIndex(lhs); i <= pt::UpperIndex(lhs); ++i)
    res[i] = op(lhs[i], rhs[i]);
    return res;
  }

  /*! \brief Generic unary componentwise operations

      \ingroup compwise_arrayoperations
   */
  template<class P, class BinaryAlgebraicOp>
  inline 
  typename boost::enable_if<use_componentwise_operations<P>, P>::type
  componentwise(P const& lhs, BinaryAlgebraicOp op)
  {
    P res;
    typedef point_traits<P> pt;
    for(int i = pt::LowerIndex(lhs); i <= pt::UpperIndex(lhs); ++i)
      res[i] = op(lhs[i]);
    return res;
  }

  /*! \brief Generic componentwise operation with scalar from the right

      \ingroup compwise_arrayoperations
   */
  template<class P, class S,  class BinaryAlgebraicOp>
  inline 
  typename boost::enable_if<use_componentwise_operations_with_scalar<P,S>,P>::type
  componentwise_with_scalar(P const& lhs, S const& rhs,  BinaryAlgebraicOp op)
  {
    P res;
    typedef point_traits<P> pt;
    for(int i = pt::LowerIndex(lhs); i <= pt::UpperIndex(lhs); ++i)
      res[i] = op(lhs[i], rhs);
    return res;
  }

  /*! \brief Generic componentwise operation with scalar from the left

      \ingroup compwise_arrayoperations
   */
  template<class P, class S,  class BinaryAlgebraicOp>
  inline 
  typename boost::enable_if<use_componentwise_operations_with_scalar<P,S>, P>::type
  componentwise_with_scalar(S const& lhs, P const& rhs,  BinaryAlgebraicOp op)
  {
    P res;
    typedef point_traits<P> pt;
    for(int i = pt::LowerIndex(lhs); i <= pt::UpperIndex(lhs); ++i)
      res[i] = op(lhs, rhs[i]);
    return res;
  }



  /*! \defgroup componentwise_algebraic_arrayoperations Componentwise algebraic array operations

      \ingroup compwise_arrayoperations
   */

  /*! \brief Componentwise product
    
     \ingroup componentwise_algebraic_arrayoperations 
   */
  template<class P>
  inline
  typename boost::enable_if<use_componentwise_operations<P>, P>::type 
  product(P const& lhs, P const& rhs)
  { return componentwise(lhs,rhs, algebraic_operators::multiply()); }

  /*! \brief Componentwise quotient
    
     \ingroup componentwise_algebraic_arrayoperations 
   */
  template<class P>
  inline
  typename boost::enable_if<use_componentwise_operations<P>, P>::type 
  quotient(P const& lhs, P const& rhs)
  { return componentwise(lhs,rhs, algebraic_operators::divide()); }

  /*! \brief Componentwise sum
    
     \ingroup componentwise_algebraic_arrayoperations 

     This is the ordinary vector sum.
   */

  template<class P>
  inline
  typename boost::enable_if<use_componentwise_operations<P>, P>::type 
  sum(P const& lhs, P const& rhs)
  { return componentwise(lhs,rhs, algebraic_operators::add()); }
 
  /*! \brief Componentwise difference
    
     \ingroup componentwise_algebraic_arrayoperations 

     This is the ordinary vector difference.
   */

  template<class P>
  inline
  typename boost::enable_if<use_componentwise_operations<P>, P>::type 
  difference(P const& lhs, P const& rhs)
  { return componentwise(lhs,rhs, algebraic_operators::subtract()); }


  /*! \brief Switch on / off  componentwise algebraic operators
    \ingroup componentwise_algebraic_arrayoperations
  */
  template<class P> struct use_algebraic_array_operators  : public use_componentwise_operations<P> {};
  /*! \brief Switch on / off  componentwise addition operator
    \ingroup componentwise_algebraic_arrayoperations
  */
  template<class P> struct use_array_operator_plus        : public use_algebraic_array_operators<P> {};
  /*! \brief Switch on / off  componentwise subtraction operator
    \ingroup componentwise_algebraic_arrayoperations
  */
  template<class P> struct use_array_operator_minus       : public use_algebraic_array_operators<P> {};

  /*! \brief Namespace for binary operators on arrays

     \ingroup componentwise_algebraic_arrayoperations 
 
      These operators get injected into the global namespace below
      in order to be accessible. Overload control is provided
      by using \ref use_componentwise_operations (uses boost::enable_if). 
   */
  namespace binary_algebraic_operators_on_arrays {
    /*! \brief Componentwise sum
      
    \ingroup componentwise_algebraic_arrayoperations 
    
    This is the ordinary vector sum, equivalent to \c sum().
    */
    
    template<class P>
    inline
    typename boost::enable_if<use_array_operator_plus<P>, P>::type 
    operator+(P const& lhs, P const& rhs) 
    { return componentwise(lhs,rhs, algebraic_operators::add()); }


    /*! \brief Componentwise difference
      
    \ingroup componentwise_algebraic_arrayoperations 
    
    This is the ordinary vector difference, equivalent to \c difference().
    */
    template<class P>
    inline
    typename boost::enable_if<use_array_operator_minus<P>, P>::type 
    operator-(P const& lhs, P const& rhs)
    { return componentwise(lhs,rhs, algebraic_operators::subtract()); }
    
  } // binary_algebraic_operators_on_arrays 


  /*! \brief Switch on / off scalar multiplication from the right.
    \ingroup componentwise_algebraic_arrayoperations
  */
  template<class P, class S>
  struct use_scalar_mult_from_right : public use_componentwise_operations_with_scalar<P,S> {};
  /*! \brief Switch on / off scalar multiplication from the left
    \ingroup componentwise_algebraic_arrayoperations
  */
  template<class P, class S>
  struct use_scalar_mult_from_left  : public use_componentwise_operations_with_scalar<P,S> {};
  /*! \brief Switch on / off scalar division.
    \ingroup componentwise_algebraic_arrayoperations
  */
  template<class P, class S>
  struct use_scalar_division        : public use_componentwise_operations_with_scalar<P,S> {};


  /*! \brief Namespace for binary operators on arrays involving scalars
 
     \ingroup componentwise_algebraic_arrayoperations 

      These operators get injected into the global namespace below
      in order to be accessible. Overload control is provided
      by using \ref use_componentwise_operations_with_scalar
      or its refinements for individual operators (uses boost::enable_if). 
   */

  namespace algebraic_operators_with_scalars_on_arrays {

    /*! \brief Multiplication with a scalar from the right
      
        \ingroup componentwise_algebraic_arrayoperations 
    */
    
    template<class P, class S>
    inline 
    typename boost::enable_if<use_scalar_mult_from_right<P,S>, P>::type
    operator*(P const& lhs, S const& rhs) 
    { return componentwise_with_scalar(lhs,rhs, algebraic_operators::multiply()); } 
    
    /*! \brief Multiplication with a scalar from the left
      
    \ingroup componentwise_algebraic_arrayoperations 
    */
    template<class P, class S>
    inline 
    typename boost::enable_if<use_scalar_mult_from_left<P,S>, P>::type
    operator*(S const& lhs, P const& rhs) 
    { return componentwise_with_scalar(lhs,rhs, algebraic_operators::multiply()); }
    
    /*! \brief Division by a scalar.
      
    \ingroup componentwise_algebraic_arrayoperations 
    */
    template<class P, class S>
    inline 
    typename boost::enable_if<use_scalar_division<P,S>, P>::type
    operator/(P const& lhs, S const& rhs) 
    { return componentwise_with_scalar(lhs,rhs, algebraic_operators::divide()); }
    

  } // namespace algebraic_operators_with_scalars_on_arrays



  /*! \defgroup componentwise_comparison_arrayoperations Componentwise comparison array operations

      \ingroup compwise_arrayoperations
   */
 
  /*! \brief Componentwise maximum.

    \ingroup componentwise_comparison_arrayoperations
  */
  template<class P>
  inline
  typename boost::enable_if<use_componentwise_operations<P>, P>::type 
  max_tuple(P const& lhs, P const& rhs)
  { return componentwise(lhs,rhs, algebraic_operators::max()); }
 
  /*! \brief Componentwise minimum.

    \ingroup componentwise_comparison_arrayoperations
  */
  template<class P>
  inline
  typename boost::enable_if<use_componentwise_operations<P>, P>::type 
  min_tuple(P const& lhs, P const& rhs)
  { return componentwise(lhs,rhs, algebraic_operators::min()); }



  /*! \defgroup componentwise_rounding_arrayoperations Componentwise rounding array operations

      \ingroup compwise_arrayoperations
   */

  /*! \brief Componentwise floor operation
      
      \ingroup componentwise_rounding_arrayoperations
  */
  template<class P>
  inline
  typename boost::enable_if<use_componentwise_operations<P>, P>::type 
  floor_tuple(P const& rhs) 
  { return componentwise(rhs, mathematical_operators::floor());}

  /*! \brief Convenience wrapper of floor with conversion

      It is often needed to convert the result of floor_tuple
      to an array type with integral value type.
   */
  template<class RES, class P>
  inline
  typename boost::enable_if<use_componentwise_operations<P>, RES>::type
  floor_tuple(P const& a)
  { 
    return GrAL::convert_point<RES>(floor_tuple(a));
  }

  /*! \brief Componentwise ceiling operation
      
      \ingroup componentwise_rounding_arrayoperations
  */
  template<class P>
  inline
  typename boost::enable_if<use_componentwise_operations<P>, P>::type 
  ceil_tuple(P const& rhs) 
  { return componentwise(rhs, mathematical_operators::ceil());}


  /*! \brief Convenience wrapper of ceil with conversion

      It is often needed to convert the result of \c ceil_tuple
      to an array type with integral value type.
   */
  template<class RES, class P>
  inline
  typename boost::enable_if<use_componentwise_operations<P>, RES>::type
  ceil_tuple(P const& a)
  { 
    return GrAL::convert_point<RES>(ceil_tuple(a));
  }

 /*! \brief Componentwise rounding operation
      
      \ingroup componentwise_rounding_arrayoperations
  */
  template<class P>
  inline
  typename boost::enable_if<use_componentwise_operations<P>, P>::type 
  round_tuple(P const& rhs) 
  { return componentwise(rhs, mathematical_operators::round());}


  /*! \brief Convenience wrapper of round with conversion

      It is often needed to convert the result of round_tuple
      to an array type with integral value type.
   */
  template<class RES, class P>
  inline
  typename boost::enable_if<use_componentwise_operations<P>, RES>::type
  round_tuple(P const& a)
  { 
    return GrAL::convert_point<RES>(round_tuple(a));
  }

  /*! \brief Componentwise clamping operation
    
     \ingroup componentwise_rounding_arrayoperations
  */
  template<class Q, class P>
  inline
  typename boost::enable_if<use_componentwise_operations<P>, P>::type 
  clamp_tuple(Q const& low, Q const& high, P const& rhs) 
  { 
    P res;
    typedef point_traits<P> pt;
    for(int i = pt::LowerIndex(rhs); i <= pt::UpperIndex(rhs); ++i)
      res[i] = mathematical_operators::clamp(low[i], high[i], rhs[i]);
    return res;
  }

  /*! \brief Convenience wrapper of clamp with conversion

      It is often needed to convert the result of floor_tuple
      to an array type with integral value type.
   */
  template<class RES, class Q, class P>
  inline
  typename boost::enable_if<use_componentwise_operations<P>, RES>::type
  clamp_tuple(Q const& low, Q const& high, P const& a)
  { 
    return GrAL::convert_point<RES>(clamp_tuple(low,high,a));
  }


  /*! \brief Operator class for componentwise clamping 
    
     \ingroup componentwise_rounding_arrayoperations
  */
  template<class Q>
  struct clamper {
    Q low, high;
    clamper(Q const& lw, Q const& hi) : low(lw), high(hi) {}

    //! \brief Clamp \c rhs to the interval [low,high]
    template<class T> T operator()(T const& rhs) const { return clamp_tuple(low,high,rhs);}

    //! \brief Clamp \c rhs to the interval [low,high], force result type RES
    template<class RES, class T> RES operator()(T const& rhs) const 
    { return clamp_tuple<RES>(low,high,rhs);}
  };

  


} // namespace GrAL

// make the operators globally visible, and let boost::enable_if
// constrain selection.
using namespace GrAL::algebraic_operators_with_scalars_on_arrays;
using namespace GrAL::binary_algebraic_operators_on_arrays;

#endif
