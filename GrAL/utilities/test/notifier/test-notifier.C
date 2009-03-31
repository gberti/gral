
/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include "Utility/notifier.h"

#include <iostream>

using namespace std;
using namespace GrAL;

class NA : public virtual notifier {
  int i;
protected:
   void  assign_values(const NA& rs) { i = rs.i;}
public:
  NA(int ii) : i(ii) {}
  NA& operator=(const NA& rs) {
    cout << "NA::operator=\n";
    if(this != &rs) {
      before_assignment();
      assign_values(rs);
      // notify_all();
      after_assignment();
    }
    return *this;
  }

  NA(const NA& rs) {
    cout << "NA::NA(const NA&)\n";
    i = rs.i;
  }

  void set_i(int k) { 
    i = k; 
    for(observer_iterator ob = begin_observer(); ob != end_observer(); ++ob)
      // could use a more specific function here
      (*ob)->notifier_assigned(this);
  }
  int  get_i() const { return i;}
};


class OA : public virtual observer {
  int i;
  const NA* na;
public:
  OA(int ii, const NA& n) : i(ii), na(&n) {
    connect(na);
  }

  OA(const OA& rs) : observer(rs), i(rs.i), na(rs.na) {}
  OA& operator=(const OA& rs) {
    cout << "OA::operator= called!\n";
    if(this != & rs) {
      observer::operator=(rs);
      i = rs.i;
      na = rs.na;
    }
    return *this;
  }

  virtual void notifier_assigned(const notifier* nt)
    {
      if (nt == (const notifier*)na) 
	cout << "OA: notifier == na (ok), ";
      else
	cout << "OA: notifier != na, ";
      
      cout << " old value: " << i << " ";
      i = na->get_i();
      cout << " new value: " << i << '\n';
     
    }
  // Normally it makes no sense to have more than one notifier ...
  void add_connection(const notifier* nt) { connect(nt);}
};


int main() {
  // using namespace GrAL;
 NA na1(1);
 OA oa1(2,na1);

 na1.set_i(3);

 NA na2(na1);

 na2.set_i(5);

 OA oa2(4,na2);
 oa2 = oa1;

 OA oa3(oa2);

 na1.set_i(7);

 NA na3(10);
 na1 = na3;

 OA oa4(11,na3);

 cout << "na2: " << na2.NumOfObservers() << " observers\n"; 
 oa4.add_connection(&na2);
 cout << "na2: " << na2.NumOfObservers() << " observers\n"; 
 oa4.add_connection(&na2);
 cout << "na2: " << na2.NumOfObservers() << " observers\n"; 

 oa3 = oa4;
 cout << "na2: " << na2.NumOfObservers() << " observers\n"; 

 na3.set_i(12);
 na1.set_i(13);
 na2.set_i(14);

 cout << "na1: " << na1.NumOfObservers() << " observers\n" 
      << "na2: " << na2.NumOfObservers() << " observers\n" 
      << "na3: " << na3.NumOfObservers() << " observers\n" 
      << "oa1: " << oa1.NumOfNotifiers() << " notifiers\n" 
      << "oa2: " << oa2.NumOfNotifiers() << " notifiers\n"
      << "oa3: " << oa3.NumOfNotifiers() << " notifiers\n"
      << "oa4: " << oa4.NumOfNotifiers() << " notifiers\n";
}
