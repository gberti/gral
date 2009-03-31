/*! \file

*/


/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include "Geometry/piecewise-linear-function.h"
#include <boost/range/size.hpp>



namespace GrAL {
  template class piecewise_linear_function<float, double>;
}

int main() {
  using namespace GrAL;
  using namespace std;

  typedef piecewise_linear_function<double, double> fdd_type;
  fdd_type f(2.0);
  cout << "f(1.0) = " << f(1.0) << "\n";

  double t[] = {-1.0, 0.0, 1.0};
  double v[] = {-2.0, 0.0, 3.0};

  std::cout << "size(t) = " << boost::size(t) << "\n";
  
  fdd_type f2(t, t+boost::size(t),v, v+boost::size(v));
  for(int i = 0; i < boost::size(t); ++i) {
    cout << "f(" << t[i]-0.1 << ") = " << f2(t[i]-0.1) << "\n"
	 << "f(" << t[i]     << ") = " << f2(t[i]    ) << "\n"
	 << "f(" << t[i]+0.1 << ") = " << f2(t[i]+0.1) << "\n";
  }
}
