/*! \file
 */

#include "IO/quoted-string.h"

#include <string>
#include <iostream>
#include <sstream>

int f(std::string const& s) { return s.size();}

int main() {
  using namespace std;

  GrAL::quoted_string qs; 
  { 
    string s("\"A B C D\"");
    stringstream in(s);
    
    cout << s << "\n";
    in >> qs;
    cout << qs << "\n";
    cout << qs.unquoted() << "\n";
    f(qs);
  }

  {
    string s("A B C D");
    stringstream in(s);
    
    cout << s << "\n";
    in >> qs;
    cout << qs << "\n";
    cout << qs.unquoted() << "\n";
  }

}
