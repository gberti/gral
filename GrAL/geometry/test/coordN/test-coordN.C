/*! \file 
  
   Test harness for coordN.

 */


/* ------------------------------------------------------------

    Copyright (C) 2001 - 2009 Guntram Berti, 
    Contact: guntram AT users DOT berlios DOT de

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */
_2003

#include "Geometry/coords.h"


#include <iostream>

namespace GrAL {
  template class coordN<1>;
  //template class coordN<2>;
  //template class coordN<3>;
  template class coordN<4>;
}


int main() {
  using namespace GrAL;
  using namespace std;

  coordN<2> p(1,2);
  coordN<2> q(3,4);

  cout << "p=" << p << endl;
  cout << "q=" << q << endl;
  cout << "p-p=" << p-p << endl;
  cout << "p+p=" << p+p << endl;
  cout << "-p=" << -p << endl;
  cout << "-p=" << -1.0*p << endl;
  cout << "-p=" << p*-1.0 << endl;
  cout << "-p=" << p/-1.0 << endl;

}
