#include "IO/control-device.h"
#include "IO/quoted-string.h"


/* ------------------------------------------------------------

    Copyright (C) 2008 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */

 
/*! \file

   Demonstrate quoted strings.
   Usage:
   ex2 -s "A B C D"
*/

int main(int argc, char* argv[]) {
  using namespace GrAL;
  using namespace std;

  for(int i = 0; i < argc; ++i)
    cout << argv[i] << "\n";

  GrAL::ControlDevice Ctrl = GetCommandlineAndFileControlDevice(argc,argv,
								Commandline::protect_args_with_spaces, "","");
  
  quoted_string qs;
  RegisterAt(Ctrl, "-path", qs);

  Ctrl.update();
  Ctrl.print_values(cout);

  cout << "quoted:   " << qs << "\n"
       << "unquoted: " << qs.unquoted() << "\n";
}
