
// $LICENSE


#include "Utility/reference-count.h"

#include <iostream.h>

#include "Utility/pre-post-conditions.h"

class testA {
public:
  virtual testA* clone() const = 0;
  virtual ~testA() {}
  virtual void do_mut() = 0;
  virtual void do_const() const = 0;
};

class testD : public testA {
  int i;
public:
  testD(int ii = 0) : i(ii) {}
  testD(testD const& rhs) : i(rhs.i) 
  { cerr << "D::D(D const&) called, i = " << i << "\n";}
  virtual ~testD() { cerr << "D::~D() called, i = " << i << "\n";}
  virtual  testD*  clone() const {
    cerr << "D::clone() called , i = " << i << "\n";
    return new testD(*this);
  }
  virtual void do_mut() {++i;} 
  virtual void do_const() const {}

};

template<>
struct copy_traits<testA> : public copy_traits_base<testA> {
  static testA* clone(testA const& a) { return a.clone();}
};

int main() {

  for(int k = 1; k <= 10; ++k)
  {
    copy_on_write_ptr<int> p = new int(1);
    int i = *p;
    REQUIRE_ALWAYS((i==1), "Different value of i = " << i << "\n",1);
    copy_on_write_ptr<int> q = p;
    copy_on_write_ptr<int> r = p;
    
    REQUIRE_ALWAYS( (*p == *q), "" , 1);
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
    copy_on_write_ptr<testA> pA1(new testD(1));
    copy_on_write_ptr<testA> pA2 = pA1;
    copy_on_write_ptr<testA> pA3 = pA1;

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
