#include "Utility/ref-ptr.h"
#include "Utility/pre-post-conditions.h"

#include <iostream>

class A {
public:
  int i;

  A() : i(1) {}

  int&           f1() { return i;}
  int const&     f2() { return i;}
  // DANGER: temporary returned!
  int            f3() { return i;}
  temporary<int> f4() { return temporary<int>(i);}
};


int main() {
  using namespace std;

  A a;

  ref_ptr<int> pi1(a.f1()); REQUIRE_ALWAYS( !pi1.owned(), "", 1);
#ifdef COMPILE_FAIL
  ref_ptr<int> pi2(a.f2()); // compile fail
  ref_ptr<int> pi3(a.f3()); // compile fail
#endif
  ref_ptr<int> pi4(a.f4()); REQUIRE_ALWAYS(  pi4.owned(), "", 1);

  ref_ptr<int const> pci1(a.f1());  REQUIRE_ALWAYS( !pci1.owned(), "", 1);
  ref_ptr<int const> pci2(a.f2());  REQUIRE_ALWAYS( !pci2.owned(), "", 1);

  // DANGER!! this creates a reference-to-temporary! 
  ref_ptr<int const> pci3(a.f3());  REQUIRE_ALWAYS( !pci3.owned(), "", 1);

  ref_ptr<int const> pci4(a.f4());  REQUIRE_ALWAYS(  pci4.owned(), "", 1);

  int* orig_addr = &(a.i);
  REQUIRE_ALWAYS( (orig_addr == &(*pi1)), "", 1);
  REQUIRE_ALWAYS( (orig_addr != &(*pi4)), "", 1);
  REQUIRE_ALWAYS( (orig_addr == &(*pci1)), "", 1);
  REQUIRE_ALWAYS( (orig_addr == &(*pci2)), "", 1);
  REQUIRE_ALWAYS( (orig_addr != &(*pci3)), "", 1);
  REQUIRE_ALWAYS( (orig_addr != &(*pci4)), "", 1);

  pci4 = pci1;
  REQUIRE_ALWAYS( (orig_addr == &(*pci4)), "", 1);
  REQUIRE_ALWAYS(  !pci4.owned(), "", 1);
  pci4 = pi1;
  REQUIRE_ALWAYS( (orig_addr == &(*pci4)), "", 1);
  REQUIRE_ALWAYS(  !pci4.owned(), "", 1);


  /* not reproducible
  cout << "&pi1="  << &(*pi1)  << "\n"
       << "&pi4="  << &(*pi4)  << "\n"
       << "&pic1=" << &(*pci1) << "\n"
       << "&pic2=" << &(*pci2) << "\n"
       << "&pic3=" << &(*pci3) << "\n"
       << "&pic4=" << &(*pci4) << "\n";
  */

#ifdef COMPILE_FAIL
  pi1 = pci1; // compile fail  
#endif
  pci1 = pi1;


}
