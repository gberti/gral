#include "Utility/ref-ptr.h"
#include "Utility/pre-post-conditions.h"

#include <iostream>


using namespace GrAL;

class A_ {
public:
  int i;

  A_() : i(1) {}

  int&           f1() { return i;}
  int const&     f2() { return i;}
  // DANGER: temporary returned!
  int            f3() { return i;}
  temporary<int> f4() { return temporary<int>(i);}
};


class X {
  int i;
public:
  X(int ii = 1) : i(ii) {}
};

class Y {
  int i;
public:
  Y(int ii = 2) : i(ii) {}
};

class G {
  ref_ptr<X const> x;
  ref_ptr<Y const> y1;
  Y       y_owned;
  ref_ptr<Y>       y_excl_owned;
public:
  G() : 
    x(new X(88), ref_ptr_base::excl_owned), 
    y1(new Y(99), ref_ptr_base::excl_owned), 
    y_excl_owned(new Y(44),  ref_ptr_base::excl_owned)
 {}
  G(ref_ptr<X const> xx) : x(xx) {}
  G(ref_ptr<Y const> yy) : y1(yy), y_owned(*yy) {}

  ref_ptr<X const> TheX()  const { return x;}
  ref_ptr<Y const> TheY1() const { return y1;}
  ref_ptr<Y const> TheYOwned() const { return ref_ptr<Y const>(y_owned);}
  ref_ptr<Y const> TheYExclOwned() const { return  y_excl_owned;}

  temporary<X const>   AnX() const { return temporary<X const>(X(66));}
  temporary<Y>         AnY() const { return temporary<Y>(Y(77));}
};

int main() {
  // using namespace GrAL;
  using namespace std;

  A_ a;

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


  G g1;
  G g2(g1.TheX());
  G g3(g1.TheY1());
  G g4(g1.TheYOwned());
  G g5(g1.AnX());
#ifdef COMPILE_FAIL
  G g6(g1.AnY()); // this fails because we cannot construct ref_ptr<T const> from temporary<T>
                  // without making the templated constructor ref_ptr<T>::ref_ptr(temporary<U>) non-explicit.
#endif
  G g7(ref_ptr<Y const>(g1.AnY()));
#ifdef COMPILE_FAIL
  G g8(make_ref_ptr(g1.AnY())); // ambiguous
#endif

  Y y1(* g1.TheY1());
  Y y2(* g1.TheYOwned());
  Y y3(g1.AnY());
  Y y4(* g1.TheYExclOwned());

  {
    int i;
    ref_ptr<int> ri(i);
    ref_ptr<int> ri1(ri); //.get_ref());
    ref_ptr<const int> rci (ri); //.get_ref<const int>());
    ref_ptr<const int> rci2(ri);

    cout << "ri:   " << (ri  .owned() ? "" : "not") << " owned" << endl;
    cout << "ri2:  " << (ri1 .owned() ? "" : "not") << " owned" << endl;
    cout << "rci:  " << (rci .owned() ? "" : "not") << " owned" << endl;
    cout << "rci2: " << (rci2.owned() ? "" : "not") << " owned" << endl;
  }
  {
    int i;
    ref_ptr<int> ri(i);
    ri.make_copy();
    ref_ptr<int> ri1(ri); //.get_ref());
    ref_ptr<const int> rci (ri); //.get_ref<const int>());
    ref_ptr<int>       ri2(ri);
    ref_ptr<const int> rci2(ri);

    cout << "ri:   " << (ri  .owned() ? "" : "not") << " owned" << endl;
    cout << "ri1:  " << (ri1 .owned() ? "" : "not") << " owned" << endl;
    cout << "ri2:  " << (ri2 .owned() ? "" : "not") << " owned" << endl;
    cout << "rci:  " << (rci .owned() ? "" : "not") << " owned" << endl;
    cout << "rci2: " << (rci2.owned() ? "" : "not") << " owned" << endl;

    ri1.make_copy();
    ri2.make_copy();
    rci2.make_copy();
    rci.make_copy();

    cout << "ri:   " << (ri  .owned() ? "" : "not") << " owned" << endl;
    cout << "ri1:  " << (ri1 .owned() ? "" : "not") << " owned" << endl;
    cout << "ri2:  " << (ri2 .owned() ? "" : "not") << " owned" << endl;
    cout << "rci:  " << (rci .owned() ? "" : "not") << " owned" << endl;
    cout << "rci2: " << (rci2.owned() ? "" : "not") << " owned" << endl;

    ri = ri1;
    rci = ri;
    rci2 = rci;
    cout << "ri:   " << (ri  .owned() ? "" : "not") << " owned" << endl;
    cout << "ri1:  " << (ri1 .owned() ? "" : "not") << " owned" << endl;
    cout << "ri2:  " << (ri2 .owned() ? "" : "not") << " owned" << endl;
    cout << "rci:  " << (rci .owned() ? "" : "not") << " owned" << endl;
    cout << "rci2: " << (rci2.owned() ? "" : "not") << " owned" << endl;

  }


}
