

/* ------------------------------------------------------------

    Copyright (C) 2001 - 2009 Guntram Berti, 
    Contact: guntram AT users DOT berlios DOT de

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


/*! \file

    Test harness for combinatorial algorithms
*/

#include "Container/combinatorial.h"
#include "Container/range.h"

#include <algorithm>
#include <iostream>
#include <iomanip>
#include <vector>




int main() {
  using namespace GrAL;
  using namespace std;
  namespace comb = GrAL::combinatorial;
  namespace ctf =  comb::compile_time_functions;

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
    cout << "factorial(" << i << ")=" << comb::factorial(i) << endl;
    

  // test binomial coeff.
  cout << "Testing binomial coefficient:\n";
  unsigned N = 4;
  for(unsigned n = 0; n <= N; ++n) {
    cout << n << "   ";
    for(unsigned i = 0; i < N-n; ++i) cout << ' ';
    for(unsigned k = 0; k <= n; ++k)
      cout << comb::binomial_coeff(n,k) << " ";
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
      s = comb::succ_ordered(s,1);
    } while(!(s == old_s));
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
      s = comb::succ_ordered(s,0);
    } while(!(s == old_s));
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
      s = comb::succ_ordered(s,1);
    } while(!(s == old_s));
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
      s = comb::succ_ordered(s,3);
    } while(!(s == old_s));
    cout << endl;
  }


  {
    cout << "Testing succ_nary_number(v,2) (empty v):\n";
    vector<unsigned> v; // empty
    vector<unsigned> s = v;
    do {
      cout << range(s.begin(), s.end()) << " -> " << flush;
      s = comb::succ_nary_number(s,2);
    } while (!(s == v) );
    cout << endl;
  }

  {
    cout << "Testing succ_nary_number(v,2):\n";
    vector<unsigned> v(1,0);
    vector<unsigned> s = v;
    do {
      cout << range(s.begin(), s.end()) << " -> " << flush;
      s = comb::succ_nary_number(s,2);
    } while (!(s == v) );
    cout << endl;
  }
  {
    cout << "Testing succ_nary_number(v,2):\n";
    vector<unsigned> v(3,0);
    vector<unsigned> s = v;
    do {
      cout << range(s.begin(), s.end()) << " -> " << flush;
      s = comb::succ_nary_number(s,2);
    } while (! (s == v) );
    cout << endl;
  }

  {
    cout << "Testing succ_nary_number(v,3):\n";
    vector<unsigned> v(2,0);
    vector<unsigned> s = v;
    do {
      cout << range(s.begin(), s.end()) << " -> " << flush;
      s = comb::succ_nary_number(s,3);

    } while (!(s == v ));
    cout << endl;
  }

  {
    cout << "Testing mirror_bits unsigned:\n";
    unsigned n[] = { 0, 1, 2, 3, 4, std::numeric_limits<unsigned>::max() };
    for(unsigned i = 0; i < sizeof(n)/sizeof(unsigned); ++i)
    cout << comb::print_binary(n[i]) << "\n"
	 << comb::print_binary(comb::mirror_bits(n[i])) << "\n"
	 << comb::mirror_bits(n[i]) << "\n\n";
  } 
  {
    cout << "Testing mirror_bits long long:\n";
    unsigned long long n[] = { 0, 1, 2, 3, 4, 13,  std::numeric_limits<unsigned long long>::max()};
    cout << std::numeric_limits<unsigned long long>::max() << "\n"
	 << comb::print_binary( std::numeric_limits<unsigned long long>::max()) << "\n\n";
    for(unsigned i = 0; i < sizeof(n)/sizeof(unsigned long long); ++i)
    cout << comb::print_binary(n[i]) << "\n"
	 << comb::print_binary(comb::mirror_bits(n[i])) << "\n"
	 << comb::mirror_bits(n[i]) << "\n\n";
     
  }

  { 
    cout << "Testing power:\n";
    for(unsigned i = 0; i < std::numeric_limits<unsigned>::digits; ++i)
      cout << "2^" << i << " = " << comb::print_binary(comb::power(2U,i)) << " = " << comb::power(2U,i)  << "\n";
    cout << std::numeric_limits<unsigned>::max() << "\n"
	 << comb::print_binary( std::numeric_limits<unsigned>::max()) << "\n\n";
  }

  { 
    cout << "Testing power unsigned long long:\n";
    typedef unsigned long long utype;
    for(unsigned i = 0; i < std::numeric_limits<utype>::digits; ++i)
      cout << "2^" << i << " = " << comb::print_binary(comb::power(utype(2),i)) << " = " << comb::power(utype(2),i)  << " = " << (utype(1) << i) << "\n";
    cout << std::numeric_limits<utype>::max() << "\n"
	 << comb::print_binary( std::numeric_limits<utype>::max()) << "\n\n";
  }

}
