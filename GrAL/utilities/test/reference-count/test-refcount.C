
#include "Utility/reference-count.h"

#include <iostream.h>

#include "Utility/pre-post-conditions.h"

class A {
public:
  virtual A* clone() const = 0;
  virtual ~A() {}
  virtual void do_mut() = 0;
  virtual void do_const() const = 0;
};

class D : public A {
  int i;
public:
  D(int ii = 0) : i(ii) {}
  D(D const& rhs) : i(rhs.i) 
  { cerr << "D::D(D const&) called, i = " << i << "\n";}
  virtual ~D() { cerr << "D::~D() called, i = " << i << "\n";}
  virtual  D*  clone() const {
    cerr << "D::clone() called , i = " << i << "\n";
    return new D(*this);
  }
  virtual void do_mut() {++i;} 
  virtual void do_const() const {}

};

template<>
struct copy_traits<A> {
  static A* clone(A const& a) { return a.clone();}
};

int main() {

  for(int k = 1; k <= 10; ++k)
  {
    copy_on_write_ptr<int> p = new int(1);
    int i = *p;
    copy_on_write_ptr<int> q = p;
    copy_on_write_ptr<int> r = p;
    
    REQUIRE( (*p == *q), "" , 1);
    cout << "*p = " << *p << "  *q = " << *q << "  *r = " << *r << endl;
    *q = 2;
    cout << "*p = " << *p << "  *q = " << *q << "  *r = " << *r << endl;
    *p = 3;
    cout << "*p = " << *p << "  *q = " << *q << "  *r = " << *r << endl;
    r = p;
    cout << "*p = " << *p << "  *q = " << *q << "  *r = " << *r << endl;
    ++(*r);
    cout << "*p = " << *p << "  *q = " << *q << "  *r = " << *r << endl;


    // test abstract class as template parameter
    copy_on_write_ptr<A> pA1(new D(1));
    copy_on_write_ptr<A> pA2 = pA1;
    copy_on_write_ptr<A> pA3 = pA1;

    cerr << "-> pA1->do_mut() \n"; 
    pA1->do_mut();
    cerr << "-> pA3 = pA1 \n"; 
    pA3 = pA1;
    cerr << "-> pA2->do_const()\n"; 
    pA2->do_const(); // selects T* operator->, not T const* operator-> !
    cerr << "-> pA3 = pA2 \n"; 
    pA3 = pA2;
    cerr << "-> pA2->do_const()\n";    
    pA2->do_const(); // selects T* operator->, not T const* operator-> !
  }

  return 0;
}
