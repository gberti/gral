/*******************************************************************************
+
+  LEDA  3.1.2
+
+
+  handle_types.h
+
+
+  Copyright (c) 1995  by  Max-Planck-Institut fuer Informatik
+  Im Stadtwald, 66123 Saarbruecken, FRG     
+  All rights reserved.
+ 
*******************************************************************************/


#ifndef MY_HANDLE_H
#define MY_HANDLE_H

#include <iostream.h>
//------------------------------------------------------------------------------
// my_handle_base/rep: base classes for my_handle types string, point, segment,...
//------------------------------------------------------------------------------


class my_handle_rep  {

friend class my_handle_base;
  
protected:

   int  count;
   
         my_handle_rep()  { count = 1; }
	 virtual ~my_handle_rep()  {}

};


class my_handle_base {

protected:

  my_handle_rep* PTR;
  my_handle_base(my_handle_rep* ptr) { PTR=ptr;} // no count++

public:

  my_handle_base() {}
  ~my_handle_base() {}
  my_handle_base(const my_handle_base& x) { PTR = x.PTR;  PTR->count++; }


my_handle_base& operator=(const my_handle_base& x)
{ 
  x.PTR->count++;
  PTR->count--;
  if (PTR->count < 0)
    cerr << "my_handlebase::= : count = " << PTR->count << endl;
  if (PTR->count == 0)  delete PTR;
  PTR = x.PTR;
  return *this;
 }

public:

int    refs()    const { return PTR->count; }
void   clear()         
{ 
  PTR->count--;
  if (PTR->count < 0 ) cerr <<"my_handle_base::clear: count = " << PTR->count << endl;
  if ((PTR->count)==0) delete PTR; 
}
};

#endif


