
/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti,
    Contact: guntram AT users DOT berlios DOT de

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include "Geometry/polynomial.h"

#include <iostream>

int main() {
  using namespace std;  

  {
    typedef GrAL::polynomial<0,double> poly_type;
    poly_type p(poly_type::coefficients_type(1.0)); // 1 
    
    cout << "p(0)=" << p(0) << " p(1)=" << p(1) << " p(2)=" << p(2) << "\n"; 
    cout << "p'(0)=" << p.d()(0) << " p'(1)=" << p.d()(1) << " p'(2)=" << p.d()(2) << "\n"; 

  }

  {
    typedef GrAL::polynomial<1,double> poly_type;
    poly_type p(poly_type::coefficients_type(1.0,2.0)); // p(t) = 1 + 2t, p'(t) = 2
    
    cout << "p(0)=" << p(0) << " p(1)=" << p(1) << " p(2)=" << p(2) << "\n"; 
    cout << "p'(0)=" << p.d()(0) << " p'(1)=" << p.d()(1) << " p'(2)=" << p.d()(2) << "\n"; 
  }
  
  {
    typedef GrAL::polynomial<2,double> poly_type;
    poly_type p(poly_type::coefficients_type(1.0,2.0,3.0)); // p(t)= 1 + 2t +3t^2, p'(t) = 2 + 6t
    
    cout << "p(0)=" << p(0) << " p(1)=" << p(1) << " p(2)=" << p(2) << "\n"; 
    cout << "p'(0)=" << p.d()(0) << " p'(1)=" << p.d()(1) << " p'(2)=" << p.d()(2) << "\n"; 
  }
  {
    typedef GrAL::polynomial<2,int> poly_type;
    poly_type p(poly_type::coefficients_type(1,2,3)); // 1 + 2t +3t^2
    
    cout << "p(0)=" << p(0) << " p(1)=" << p(1) << " p(2)=" << p(2) << "\n"; 
    cout << "p'(0)=" << p.d()(0) << " p'(1)=" << p.d()(1) << " p'(2)=" << p.d()(2) << "\n"; 
  }

  {
    typedef GrAL::tuple<double,2> vt;
    typedef GrAL::polynomial<2,double, vt > poly_type;
    poly_type p(poly_type::coefficients_type(vt(1.0,0.0),vt(2.0,1.0),vt(3.0,2.0))); // 1 + 2t +3t^2
    
    cout << "p(0)=" << p(0) << " p(1)=" << p(1) << " p(2)=" << p(2) << "\n"; 
    cout << "p'(0)=" << p.d()(0) << " p'(1)=" << p.d()(1) << " p'(2)=" << p.d()(2) << "\n"; 
  }
 

}
