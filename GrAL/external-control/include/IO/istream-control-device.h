#ifndef NMWR_GB_ISTREAM_CONTROL_DEVICE_H
#define NMWR_GB_ISTREAM_CONTROL_DEVICE_H


//----------------------------------------------------------------
//   (c) Guntram Berti, 1997
//   Chair for Numerical Mathematics & Scientific Computing (NMWR)
//   TU Cottbus - Germany
//   http://math-s.math.tu-cottbus.de/NMWR
//   
//----------------------------------------------------------------


#include <iostream.h>
#include <list.h>  // STL
#include "mystring.h"
#include "IO/control-device.h"
#include "IO/read-parameters.h"

class istream_control_device_impl : public control_device_impl {
  typedef istream_control_device_impl self;
public:
  istream_control_device_impl(istream* i, const string& nm, const string& ind = "");

  // istream_control_device_impl* get_sub_device(const string& nm);
  control_device_impl* get_sub_device(const string& nm);

  virtual string name() const;

  virtual void update();
  
  virtual void add(const string& name,Mutator* value);

  virtual void register_at(ControlDevice& Ctrl);
  virtual void register_at(ControlDevice& Ctrl, const string& prefix);

  virtual void attach_to(istream& in_new);
  virtual void print_values(ostream& out) const;
  virtual void print_values(ostream& out, const string& ind) const; 

  virtual void read(istream& in);
  virtual void print(ostream& out) const;
  virtual void print_unrecognized(ostream& out) const;
  virtual void print_unrecognized(ostream& out, const string& prefix) const;

  friend istream& operator>>(istream& in,  self& rhs) 
    { rhs.read(in);   return in;}
  friend ostream& operator<<(ostream& out, const self& rhs) 
    { rhs.print(out); return out;}

protected:
  istream* in;
  string name_;
  string indent_;
  MutableVars MV;
  list<self*> sub_devices_;
};



#endif
