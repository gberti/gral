/*! \file
 */

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
