#ifndef NMWR_GB_MUTATOR_BASE_H
#define NMWR_GB_MUTATOR_BASE_H


// $LICENSE

#include "IO/checker-base.h"

#include <iostream>
#include <string>


namespace GrAL {


  // "letter" class
  class mutator_impl {
  public:
    virtual void read (std::istream& in)  = 0;
    virtual void print(std::ostream& out) const = 0;
    virtual void print(std::ostream& out, const std::string& name) const = 0;
    
    virtual std::string vartypename() const = 0;
    virtual std::string description() const;
    virtual ~mutator_impl() {}
    
    void run_checker() { c.check(); }
    void set_checker(Checker _c) { c = _c;}
    
  private:
    Checker c;
  };
  
  /*! \brief Placeholder-class for arbitrary variables
    that (for example) may be read and written from or to streams, resp.

  */
  class Mutator {
  public:
    Mutator() {}
    Mutator(boost::shared_ptr<mutator_impl> i) : impl(i) {}
    Mutator(mutator_impl                  * i) : impl(i) {}

    void read (std::istream& in) { impl->read(in);}
    void print(std::ostream& out) const { impl->print(out);}
    void print(std::ostream& out, const std::string& name) const { impl->print(out, name);}

    std::string vartypename() const { return impl->vartypename();}
    std::string description() const { return impl->description();}

    void run_checker()          { impl->run_checker(); }
    void set_checker(Checker c) { impl->set_checker(c);}
  private:
    boost::shared_ptr<mutator_impl> impl;
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

} // namespace GrAL 

#endif
