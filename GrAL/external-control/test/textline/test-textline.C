/*! \file
 */


/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti,
    Contact: guntram AT users DOT berlios DOT de

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */

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
