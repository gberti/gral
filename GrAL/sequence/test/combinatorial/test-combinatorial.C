#include "Container/combinatorial.h"
#include <iostream>

int main() {
  using namespace std;

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

}
