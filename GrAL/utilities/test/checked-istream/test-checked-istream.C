/*! \file
    
    Test harness for \c checked_istream.
 */

#include <Utility/checked-istream.h>
#include <fstream>


int main() {
  using namespace std;  

  ifstream f("exists.dat");
  GrAL::checked_istream cf(&f);
  
  int k;
  while( cf >> k ) {
    cout << k << ' ' << flush;
  }
  // read beyond end of stream
  try {
    cf >> k;
  }
  catch(...) {
    cout << "<Continuing.>" << endl;
  }
  cout << endl;

  // file containing non-integers
  ifstream fbad("bad.dat");
  GrAL::checked_istream cfbad(&fbad); 
  try {
    while( cfbad >> k)
      cout << k << ' ' << flush; 
  }
  catch(...) {
    cout << "<Continuing.>" << endl;
  }
  cout << endl;

  ifstream f2("doesnotexist.dat");
  try{
    GrAL::checked_istream cf2(&f2);
    while( (cf2 >> k)) {
      cout << k << ' ' << flush;
    }
  }
  catch(...) {
    cout << "<Continuing.>" << endl;
  }
  cout << endl;
}
