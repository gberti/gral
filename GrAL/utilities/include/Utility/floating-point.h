#ifndef GRAL_GB_UTILITY_FLOATING_POINT_H
#define GRAL_GB_UTILITY_FLOATING_POINT_H



/* ------------------------------------------------------------

    Copyright (C) 2005 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include <cmath>
#include <limits>

namespace GrAL { namespace floating_point {


  template<class F>
  struct floating_ops {
    static F frexp(F x, int& exp) { return std::frexp(x,&exp);}
  };

  /*
  template<> struct floating_ops<float> {
    // not in namespace std?
    static float frexp(float x, int& exp) { return frexpf(x,&exp); }
  };

  template<> struct floating_ops<double> {
    static double frexp(double x, int& exp) { return std::frexp(x,&exp);}
  };

  template<> struct floating_ops<long double> {
    static long double frexp(long double x, int& exp) { return frexpl(x,&exp);}
  };
  */

  /*! \brief Compute exact (if possible) rational representation  of floating point number

    \todo Flag compile-time error if integer type I has less bits than the mantissa of F.
  */
  template<class I, class F>
  bool float2rat(F x, I& numerator, I& denominator) {
    bool res = true; // can represent x
    // TODO: check if I is broad enough ...
    I one(1);
    int exp = 0;
    F normalized_x = floating_ops<F>::frexp(x,exp);
    unsigned d = std::numeric_limits<F>::digits;

    // Now: x == normalized_x * 2^exp && 0.5 <= normalized_x < 1
    denominator = one << d; // = 2^d

    // normalized_x * 2^d is an integer
    // x == 2^exp * (normalized_x * 2^d) / 2^d 
    numerator = I(normalized_x * denominator);

    // adjust for the missing factor 2^exp
    if(exp > 0) 
      denominator >>= exp;
    else {
      // avoid overflow if  -exp+d >= dI = digits(I) 
      if(-exp + d >  std::numeric_limits<I>::digits - 1) {
	int dI = std::numeric_limits<I>::digits;
	// change denominator to maximal value  2^{dI-1} = 2^d * 2^{dI-1-d}
	int diff = dI-1 -d;
	denominator *= (one << diff);
	// ... and reduce numrator by 2^exp * 2^{dI-1-d}. Here we lose digits!
	res = (numerator % (one << (-exp - diff)) == 0);
	numerator   /= (one << (-exp - diff));
      }
      else
	denominator *= (one << (-exp));
    }
    return res;
  }


}} // namespace GrAL { namespace floating_point 

#endif
