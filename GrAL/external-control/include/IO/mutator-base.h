#ifndef NMWR_GB_MUTATOR_BASE_H
#define NMWR_GB_MUTATOR_BASE_H


// $LICENSE

#include <iostream>
#include <string>



//! Placeholder-class for arbitrary variables

/*! Placeholder-class for arbitrary variables
    that (for example) may be read and written from or to streams, resp.
*/

class Mutator {
public:
  virtual void read (std::istream& in)  = 0;
  virtual void print(std::ostream& out) const = 0;
  virtual void print(std::ostream& out, const std::string& name) const = 0;

  virtual std::string vartypename() const = 0;
  virtual std::string description() const;
  virtual ~Mutator() {}
};


class ControlDevice;


/*! abstract base for everything that has some parameters
    to be controled externally. The implementation of 
    register_at will use a concrete implementation
    of Mutator
*/

class controlable {
public:
  virtual void init() {}
  virtual void update() {}
  virtual void finish() {}

  //! expose internal data to ControlDevice Ctrl
  virtual void register_at(ControlDevice& Ctrl) {}
  //! expose internal data to ControlDevice Ctrl, prepending prefix to the name
  virtual void register_at(ControlDevice& Ctrl, const std::string& /*prefix*/) {}

  //! may be used to signal a change
  virtual void notify()  {} 

  virtual ~controlable() {}
};


#endif
