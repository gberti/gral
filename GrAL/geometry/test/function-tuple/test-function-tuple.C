
/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti,
    Contact: guntram AT users DOT berlios DOT de

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include "Geometry/function-tuple.h"
#include "Geometry/piecewise-function.h"
#include "Geometry/polynomial.h"

#include "Container/tuple-point-traits.h"

#include <iostream>



template<int N>
struct tuple_factory {
  template<class T>
  struct apply { typedef GrAL::tuple<T,N> type;};
};

int main() {
  using namespace std;  


  {
    typedef GrAL::polynomial<1,double> poly_type;
    typedef GrAL::function_tuple<poly_type, tuple_factory<2> > function_type;

    poly_type p[] = { poly_type(poly_type::coefficients_type(1.0,2.0)),  // p0(t) = 1 + 2t, 
		      poly_type(poly_type::coefficients_type(2.0,1.0))};  // p1(t) = 2 +  t, 
    function_type f(p,p+2);
    
    double t[] = { -1, -0.5, 0, 0.5, 1, 2, 3 };
    for(int i = 0; i < sizeof(t)/sizeof(double); ++i)
      cout << "f  (" << t[i] << ")= " << f(t[i]) << "  ";
    cout << "\n";
    for(int i = 0; i < sizeof(t)/sizeof(double); ++i)
      cout << "f' (" << t[i] << ")= " << f.d(t[i]) << "  ";
    cout << "\n";
    for(int i = 0; i < sizeof(t)/sizeof(double); ++i)
      cout << "f''(" << t[i] << ")= " << f.dd(t[i]) << "  ";
    cout << "\n";
    for(int i = 0; i < sizeof(t)/sizeof(double); ++i)
      cout << "k(" << t[i] << ")= " << f.curvature(t[i]) << "  ";
    cout << "\n";
    for(int i = 0; i < sizeof(t)/sizeof(double); ++i)
      cout << "tang(" << t[i] << ")= " << f.tangent(t[i]) << "  ";
    cout << "\n";

  }


 

}
