#ifndef NMWR_GB_CONTROLLER_H
#define NMWR_GB_CONTROLLER_H


//----------------------------------------------------------------
//   (c) Guntram Berti, 1997
//   Chair for Numerical Mathematics & Scientific Computing (NMWR)
//   TU Cottbus - Germany
//   http://math-s.math.tu-cottbus.de/NMWR
//   
//----------------------------------------------------------------


#include "forward/iostream.h"
#include "forward/string.h"

#include "IO/mutator.h"

/////////////////////////////////////////////////////////////////
//
//   abstract interface for controlling parameters,
//   e.g. they may be 
//   - read from a file
//   - set interactively via a GUI or a command interpreter
//
/////////////////////////////////////////////////////////////////





// implementation class
class control_device_impl : public controlable {
public:
  virtual void update() = 0;
  virtual void add(const string&, Mutator*) = 0;

  // virtual void set(const string&, Mutator*) = 0;
  // virtual void remove(const string&, Mutator*) = 0;

  virtual void print_values(ostream&) const = 0;


  virtual void print_unrecognized(ostream&) const = 0;
  virtual void print_unrecognized(ostream&, const string&) const = 0;

  virtual void attach_to(istream& in) = 0;
  virtual control_device_impl* get_sub_device(const string& nm) = 0;

  virtual string name() const = 0;
  virtual ~control_device_impl() {}
};

// envelope class
class ControlDevice  {
public:
  ControlDevice(control_device_impl* imp = 0) : impl(imp) {}

  void add(const string& nm,Mutator* value_ref);
  void add(const char*   nm,Mutator* value_ref);
  //  void remove(const string& name);
  //  void set(const string& name,Mutator* value_ref);

  void update();
  void print_values(ostream&) const;
  void print_unrecognized(ostream&) const;
  void attach_to(istream& in);


  void register_at(ControlDevice&, const string& prefix);

  string name() const;

  ControlDevice getSubDevice(const string& name);
  ControlDevice getSubDevice(const char*   name);
private:
  control_device_impl* impl;
};


template<class T>
inline void RegisterAt(ControlDevice& Ctrl, const string& name, T& t)
{ 
  TypedMutator<T>* p = new TypedMutator<T>(t);
  Ctrl.add(name, p); 
}

template<class T>
inline void RegisterAt(ControlDevice& Ctrl, const char*   name, T& t)
{  Ctrl.add(name, new TypedMutator<T>(t)); }



// a simple ControlDevice that reads name-value-pairs from a file
extern ControlDevice GetStreamDevice(istream* in, const string& name = "");
//extern ControlDevice GetFileControlDevice(const string& filename, const string& name);
extern ControlDevice GetFileControlDevice(const char*   filename, const string& name);

extern ControlDevice GetCommandlineAndFileControlDevice(int argc, char* argv[],
							const string& filename, 
							const string& name);


extern ControlDevice GetDuplexControlDevice(istream& in2,
					    const char* filename, const string& name);
extern ControlDevice GetDuplexControlDevice(istream& in2,
					    const string& filename, const string& name);
#endif
