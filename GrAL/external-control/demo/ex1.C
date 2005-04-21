
// $LICENSE

#include <iostream>
#include <string>

#include "IO/mutator.h"  // only needed for "special" mutators.
#include "IO/checker.h"
#include "IO/control-device.h"

/*! \file

   Demonstration for nested parameter control via parameter 
   file ("ctrl.in" in this example) and command line.

   try something like <br>
   <tt> $ ./ex1.exe   n 77  sub1 { sub1.1 { name1 blabla } x -0.888 } </tt> <br>
   on the commandline to see what is happening.
  <br>
   To see error handling, try: <br>
  <tt> $ ./ex1.exe   this is rubbish   n 77  sub1 { sub1.1 { name1 blabla } x -0.888 } </tt>
*/


int main(int argc, char* argv[]) {
  using namespace GrAL;
  using namespace std;

  //  read variables from file only: uncomment the following line
  //  ControlDevice Ctrl = GetFileControlDevice("ctrl.in","main");
 
  // read variables from file and then from commandline, 
  // such that command-line settings take precedence.
  GrAL::ControlDevice Ctrl = GetCommandlineAndFileControlDevice(argc,argv,"ex1.in","main");

  int n  = 5;
  int m = 7;
  double x = 1.0, y = 2.0;
  double checked_x = -3.0, checked_y = 3.0;
  double checked_upper_y = 0.0, checked_lower_y = 0.0;
  double checked_z = 0.0;
  std::string name1_1 = "1_1", name1_2 = "1_2";
  std::string name2_1 = "2_1", name2_2 = "2_2";

  bool flag = true;
  bool print_variables = true;
 
  Ctrl.add("print",GetCommentedMutator(print_variables,
				       "<bool> : print values of variables?"));
  Ctrl.add("n", GetMutator(n));
  Ctrl.add("n", GetMutator(m)); // m will be altered, not n!

  // create a nested subdevice of level 1
  ControlDevice Ctrl1 = Ctrl.getSubDevice("sub1");
  Ctrl1.add("flag-on",  GetTrueOnReadMutator(flag));
  Ctrl1.add("flag-off", GetFalseOnReadMutator(flag));
  Ctrl1.add("x", GetMutator(x));
  RegisterAt(Ctrl1, "y", y); // equivalent to the above
  // check the parameters

  // add limits checkers
  // variant 1
  Ctrl1.add (       "checked-x", GetMutator(checked_x), GetLimitsChecker(checked_x, -2.0, 2.0, "checked-x"));
  // variant 2
  RegisterAt(Ctrl1, "checked-y", checked_y, GetLimitsChecker    (checked_y, -2.0, 2.0, "checked-y"));
  // variant 3
  RegisterAt(Ctrl1, "checked-z", checked_z, -1.0, 1.0); 

  // This would replace the LimitsChecker installed before on checked_y
  // RegisterAt(Ctrl1, "checked-y", checked_y, GetUpperLimitChecker    (checked_y, -2.0, 2.0, "checked-y"));
  RegisterAt(Ctrl1, "checked-upper-y", checked_upper_y, GetUpperLimitChecker(checked_upper_y,       2.0, "checked-upper-y"));
  RegisterAt(Ctrl1, "checked-lower-y", checked_lower_y, GetLowerLimitChecker(checked_lower_y, -2.0,      "checked-lower-y"));


  // create a nested subdevice of level 2
  ControlDevice Ctrl1_1 = Ctrl1.getSubDevice("sub1.1");
  Ctrl1_1.add("name1", GetMutator(name1_1));
  Ctrl1_1.add("name2", GetMutator(name1_2));

  // create another nested subdevice of level 2
  ControlDevice Ctrl1_2 = Ctrl1.getSubDevice("sub1.2");
  Ctrl1_2.add("name1", GetMutator(name2_1));
  Ctrl1_2.add("name2", GetMutator(name2_2));


  //---------------------------------------------------------

  // set the parameters registered so long.
  // this warns about unrecognized strings in the input.
  try {
    Ctrl.update();
  }
  catch(std::out_of_range & e) {
    cout << "Exception caught: " << e.what() << " (ignored). \n";
  }

  // show all name-value pairs of Ctrl 
  // (not only those that have been read by update())
  Ctrl.print_values(cout);

  // Print warning if any strings did not correspond to valid name-value pairs.
  // This is done also by Ctrl.update(), so its superflous in principle.
  Ctrl.print_unrecognized(cout); 


  if(print_variables) {
    cerr << "\n"
	 << "Values of the variables:\n"
	 << "n: " << n << '\n'
	 << "m: " << m << '\n'
	 << "x: " << x << '\n'
	 << "y: " << y << '\n'
	 << "flag: " << flag << '\n'
	 << "name1_1: " << name1_1 << '\n' 
	 << "name1_2: " << name1_2 << '\n' 
	 << "name2_1: " << name2_1 << '\n' 
	 << "name2_2: " << name2_2 << '\n';
  }
}
