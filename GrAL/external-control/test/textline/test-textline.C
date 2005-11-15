/*! \file
 */

#include "IO/textline.h"

#include <sstream>

int main() {
  using namespace std;

  std::string t;
  t += "A  A   A\n";
  t += "B  B   B\n";
  t += "\n";
  t += "C\n";
  t += "D";

  std::stringstream is(t);
  GrAL::textline line;

  while(is >> line)
    cout << "line:" << line;
  cout << endl;
};
