
// $LICENSE_NEC_2003

/*! \file

    Test harness for combinatorial algorithms
*/

#include "Container/combinatorial.h"
#include "Container/range.h"

#include <iostream>
#include <vector>

int main() {
  using namespace std;
  namespace ctf = compile_time_functions;

  // test power
  cout << "Testing power:\n";
  cout << "power<2,0>=" << ctf::power<2,0>::value << endl;
  cout << "power<2,1>=" << ctf::power<2,1>::value << endl;
  cout << "power<2,31>=" << ctf::power<2,31>::value << endl;
  cout << "power<2,32>=" << ctf::power<2,32>::value << endl; // overflow

  // test factorial
  cout << "Testing factorial:\n";
  // overflow for i = 15
  for(int i = 0; i <= 15; ++i)
    cout << "factorial(" << i << ")=" << factorial(i) << endl;
    

  // test binomial coeff.
  cout << "Testing binomial coefficient:\n";
  unsigned N = 4;
  for(unsigned n = 0; n <= N; ++n) {
    cout << n << "   ";
    for(unsigned i = 0; i < N-n; ++i) cout << ' ';
    for(unsigned k = 0; k <= n; ++k)
      cout << binomial_coeff(n,k) << " ";
    cout << "\n";
  }

  {
    cout << "Testing succ_ordered maxitem = 1 (empty v):\n";
    vector<unsigned> v;
    vector<unsigned> s = v;
    vector<unsigned> old_s = s;
    
    do {
      cout << range(s.begin(), s.end()) << " -> " << flush;
      old_s = s;
      s = succ_ordered(s,1);
    } while(s != old_s);
    cout << endl;
  }
  {
    cout << "Testing succ_ordered maxitem = 0:\n";
    vector<unsigned> v(1,0);
    vector<unsigned> s = v;
    vector<unsigned> old_s = s;
    
    do {
      cout << range(s.begin(), s.end()) << " -> " << flush;
      old_s = s;
      s = succ_ordered(s,0);
    } while(s != old_s);
    cout << endl;
  }
  {
    cout << "Testing succ_ordered maxitem = 1:\n";
    vector<unsigned> v(1,0);
    vector<unsigned> s = v;
    vector<unsigned> old_s = s;
    
    do {
      cout << range(s.begin(), s.end()) << " -> " << flush;
      old_s = s;
      s = succ_ordered(s,1);
    } while(s != old_s);
    cout << endl;
  }

  {
    cout << "Testing succ_ordered maxitem = 3:\n";
    vector<unsigned> v(2,0);
    v[0] = 0;
    v[1] = 1;
    vector<unsigned> s = v;
    vector<unsigned> old_s = s;
    
    do {
      cout << range(s.begin(), s.end()) << " -> " << flush;
      old_s = s;
      s = succ_ordered(s,3);
    } while(s != old_s);
    cout << endl;
  }


  {
    cout << "Testing succ_nary_number(v,2) (empty v):\n";
    vector<unsigned> v; // empty
    vector<unsigned> s = v;
    do {
      cout << range(s.begin(), s.end()) << " -> " << flush;
      s = succ_nary_number(s,2);
    } while (s != v );
    cout << endl;
  }

  {
    cout << "Testing succ_nary_number(v,2):\n";
    vector<unsigned> v(1,0);
    vector<unsigned> s = v;
    do {
      cout << range(s.begin(), s.end()) << " -> " << flush;
      s = succ_nary_number(s,2);
    } while (s != v );
    cout << endl;
  }
  {
    cout << "Testing succ_nary_number(v,2):\n";
    vector<unsigned> v(3,0);
    vector<unsigned> s = v;
    do {
      cout << range(s.begin(), s.end()) << " -> " << flush;
      s = succ_nary_number(s,2);
    } while (s != v );
    cout << endl;
  }

  {
    cout << "Testing succ_nary_number(v,3):\n";
    vector<unsigned> v(2,0);
    vector<unsigned> s = v;
    do {
      cout << range(s.begin(), s.end()) << " -> " << flush;
      s = succ_nary_number(s,3);

    } while (s != v );
    cout << endl;
  }

}
