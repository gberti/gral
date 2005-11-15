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
    mutator_impl() : read_flag(false), is_required_(false) {}
    virtual ~mutator_impl() {}
 
    void         read (std::istream& in)  { read_flag = read_var(in); }
    virtual void print(std::ostream& out) const = 0;
    virtual void print(std::ostream& out, const std::string& name) const = 0;
    
    virtual std::string vartypename() const = 0;
    virtual std::string description() const;
    
    void run_checker() { c.check(); }
    void set_checker(Checker _c) { c = _c;}
    bool was_read() const { return read_flag;}

    void set_required()       { is_required_ = true;}
    void set_optional()       { is_required_ = false;}
    bool is_required()  const { return  is_required_;}
    bool is_optional()  const { return !is_required_;}
  protected:
    virtual bool read_var(std::istream& in)  = 0;
  private:
    Checker c;
    bool    read_flag;
    bool    is_required_;
 };
  
  /*! \brief Placeholder-class for arbitrary variables
    that (for example) may be read and written from or to streams, resp.

  */
  class Mutator {
    typedef Mutator self;
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
    bool was_read() const { return impl->was_read();}

    self make_required()      { impl->set_required(); return *this;}
    self make_optional()      { impl->set_optional(); return *this;}

    bool is_required()  const { return impl->is_required();}
    bool is_optional()  const { return impl->is_optional();}
  private:
    boost::shared_ptr<mutator_impl> impl;
  };

  inline std::ostream& operator<<(std::ostream& out, Mutator  m) { m.print(out); return out;}
  inline std::istream& operator>>(std::istream& in,  Mutator& m) { m.read (in);  return in; }

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
