

/*! \file

*/

#include "Container/union-find.h"
#include "Utility/pre-post-conditions.h"

#include <iostream>
#include <vector>

int main() {
  using namespace std;

  GrAL::union_find<int> uf;

  for(int i = 0; i < 10; ++i)
    REQUIRE_ALWAYS(i == uf(i), "i=" << i << " uf(i)=" << uf(i), 1);

  uf.join(0,1);
  uf.join(2,3);
  uf.join(1,3);

  uf.join(4,5);
  uf.join(5,6);
  uf.join(7,8);
  uf.join(5,8);

  for(int i = 0; i < 10; ++i)
    cout << "uf(" << i << ") = " << uf(i) << " size = " << uf.size(i) << "\n";

  for(GrAL::union_find<int>::const_iterator s = uf.begin(); s != uf.end(); ++s)
    cout << "set " << (*s).second << " has " << uf.size((*s).second) << " members\n";

  GrAL::union_find<int> uf2;
  int N = ( 1 << 10);
  cout << "Testing union-find for " << N << " items ... " << endl;
  for(int k = 2; k < N; k *= 2) {
    for(int i = 0; i < N/k; ++i)
      uf2.join(k*i, k*i + k/2);
    cout << k << ": " << uf2.size() << " sets" << endl;
  }
  vector<int> setof(N);
  for(int i = 0; i < N; ++i)
    setof[i] = uf2(i);
}
