#ifndef NMWR_GB_MUTATOR_BASE_H
#define NMWR_GB_MUTATOR_BASE_H

//----------------------------------------------------------------
//   (c) Guntram Berti, 1997
//   Chair for Numerical Mathematics & Scientific Computing (NMWR)
//   TU Cottbus - Germany
//   http://math-s.math.tu-cottbus.de/NMWR
//   
//----------------------------------------------------------------

#include "forward/iostream.h"
#include "forward/string.h"


/**
 @author Guntram Berti
 @see mutator.h
      control-device.h
*/

// placeholder-class for arbitrary variables
// that may be read and written from  or to streams, resp.


class Mutator {
public:
  virtual void read (istream& in)  = 0;
  virtual void print(ostream& out) const = 0;
  virtual void print(ostream& out, const string& name) const = 0;

  virtual string vartypename() const = 0;
  virtual string description() const;
  virtual ~Mutator() {}
};


class ControlDevice;


// abstract base for everything that has some parameters
// to be controled externally. The implementation of 
// register_at will use a concrete implementation
// of Mutator

class controlable {
public:
  virtual void init() {}
  virtual void update() {}
  virtual void finish() {}

  virtual void register_at(ControlDevice&,const string& /*prefix*/) {}
  virtual void register_at(ControlDevice&) {}
  // virtual void unregister_at(ControlDevice&) = 0;

  virtual void notify()  {} // may be used to signal a change
  virtual ~controlable() {}
};


#endif
