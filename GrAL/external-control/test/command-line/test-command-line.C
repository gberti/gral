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

#include "IO/command-line.h"

#include <iostream>

int main() {
  using namespace std;

  {
    char* argv[] = { "", "-a", "A" };
    int argc = sizeof(argv)/sizeof(char*);
    
    GrAL::Commandline C(argc, argv);
    
    cout << C.str() << "\n";
  }
 
  {
    char* argv[] = { "", "-a", "A B C" };
    int argc = sizeof(argv)/sizeof(char*);
    
    GrAL::Commandline C(argc, argv, GrAL::Commandline::protect_args_with_spaces);
    
    cout << C.str() << "\n";
  }

  
};
