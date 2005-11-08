/*! \file

*/

#include "Utility/floating-point.h"

#include <boost/range/size.hpp>

#include <iostream>
#include <iomanip>


template<class I, class F>
I numerator(F x) {
  I n,d;
  GrAL::floating_point::float2rat(x,n,d);
  return n;
}

template<class I, class F>
I denominator(F x) {
  I n,d;
  GrAL::floating_point::float2rat(x,n,d);
  return d;
}


template<class I, class It>
void test_rational(It x, It x_end)
{
  for(int i = 0; i < x_end - x; ++i) {
    double rat  = (long double)numerator<I>(x[i]) / (long double) denominator<I>(x[i]);
    double diff = x[i] - rat;
    unsigned d = std::numeric_limits<I>::digits;
    std::cout << std::scientific << x[i] << " = " 
	      << std::setwidth(d) << numerator<I>(x[i]) << " / " 
	      << std::setwidth(d) << denominator<I>(x[i]) << " = "
	      << rat << " (diff=" << diff << ")\n";
  }
}


int main() {
  using namespace std;
  
  float  x[]   = { 1.0, 1.1, 1.2, 1.25, 1.5, 2.0};
  double xd [] = { -1.0, -0.5, 0.0, 1.0e-6, 0.0001, 0.01, 0.1, 0.25, 0.5, 
                   1.0, 1.1, 1.2, 4.0/3.0, 1.25, 1.5, 1.9, 2.0, 3.0, 4.0, 
                   10.5, 1000, 1.0e5, 1.0e8 };
  long double xld[] = { 1.0, 1.1, 1.2, 4.0/3.0, 1.25, 1.5, 1.9, 2.0};

  cout << "Testing float -> int/int\n";
  test_rational<int>(x,  x +boost::size(x));
  cout << "Testing double -> int/int\n";
  test_rational<int>(xd, xd+boost::size(xd));
  cout << "Testing double -> longlong/longlong\n";
  test_rational<long long>(xd, xd+boost::size(xd));
  //cout << "Testing long double -> longlong/longlong\n";
  //test_rational<long long>(xld, xld+boost::size(xld));

}
