
// $LICENSE_NEC_2004

/*! \file

     Test harness for \c selective_ostream
*/

#include "Utility/selective-ostream.h"
#include "Utility/pre-post-conditions.h"

int main() {
  
  GrAL::selective_ostream out(std::cout);
  out << "passed" << std::endl;
  out.off(); REQUIRE_ALWAYS(out.is_off(), "",1);
  out << "ignored" << std::endl;
  out.on();  REQUIRE_ALWAYS(out.is_on(), "",1);
  out << "passed" << std::endl;

  GrAL::selective_ostream out2;
  REQUIRE_ALWAYS(out2.is_off(), "",1);
  out2 << "ignored";
  try {
    out2.on();
  }
  catch(...) {
    std::cout << "Continuing." << std::endl;
  }
  out2.redirect(std::cout);
  out2.on();
  out2 << "passed" << std::endl;
}
