/*! \file 
  
   Test harness for coordN.

 */

// $LICENSE_NEC_2003

#include "Geometry/coords.h"


#include <iostream>

template class coordN<1>;
//template class coordN<2>;
//template class coordN<3>;
template class coordN<4>;


int main() {
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
