#ifndef GRAL_GB_UTILITY_NUMERIC_TYPES_H
#define GRAL_GB_UTILITY_NUMERIC_TYPES_H

// $LICENSE_NEC

/*! \brief Templates for promotion of numeric types (floating point and integral)

    \todo This is very incomplete. No promotion rules among integral types specified.
    \todo Integral types get promoted to floating point types, which is not always OK
        (may result in information loss. Use boost::numeric_cast<> ?)
 */
namespace numeric_types {

  struct floating_point_type {};
  struct integral_type       {};

  template<class T> struct number_type  { typedef integral_type type;};
  template<> struct number_type<float>  { typedef floating_point_type type;};
  template<> struct number_type<double> { typedef floating_point_type type;};


  template<class T, class U, class number_type_1, class number_type_2> struct promote_aux { typedef T type;};
  template<class T, class U> struct promote_aux<T,U, floating_point_type, integral_type>  { typedef T type;};
  template<class T, class U> struct promote_aux<T,U, integral_type, floating_point_type>  { typedef U type;};

  template<class T, class U> struct promote 
  { typedef typename promote_aux<T,U, typename number_type<T>::type, typename number_type<U>::type>::type type;};

  template<class T> struct promote<T,T>    { typedef T      type;};
  template<> struct promote<double,float>  { typedef double type;};
  template<> struct promote<float, double> { typedef double type;};

} // namespace numeric_types


#endif
