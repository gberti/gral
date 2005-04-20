#ifndef NMWR_GB_ISTREAM_CONTROL_DEVICE_H
#define NMWR_GB_ISTREAM_CONTROL_DEVICE_H



// $LICENSE


#include <iostream>
#include <list>  // STL
#include <string>

#include "IO/control-device.h"
#include "IO/read-parameters.h"

namespace GrAL {

class istream_control_device_impl : public control_device_impl {
  typedef istream_control_device_impl self;
public:
  istream_control_device_impl(boost::shared_ptr<std::istream> i,
                              std::string const& nm, 
                              std::string const& ind = "");

  boost::shared_ptr<control_device_impl> get_sub_device(std::string const& nm);

  virtual std::string name() const;

  virtual void update();
  
  virtual void add(const std::string& name, boost::shared_ptr<Mutator> value);

  virtual void register_at(ControlDevice& Ctrl);
  virtual void register_at(ControlDevice& Ctrl, const std::string& prefix);

  virtual void attach_to(std::istream& in_new);
  virtual void print_values(std::ostream& out) const;
  virtual void print_values(std::ostream& out, std::string const& ind) const; 

  virtual void read(std::istream& in);
  virtual void print(std::ostream& out) const;
  virtual void print_unrecognized(std::ostream& out) const;
  virtual void print_unrecognized(std::ostream& out, std::string const& prefix) const;

  friend std::istream& operator>>(std::istream& in,  self& rhs) 
    { rhs.read(in);   return in;}
  friend std::ostream& operator<<(std::ostream& out, const self& rhs) 
    { rhs.print(out); return out;}

protected:
  boost::shared_ptr<std::istream> in;
  std::string name_;
  std::string indent_;
  MutableVars MV;
  std::list<boost::shared_ptr<self> > sub_devices_;
};

} // namespace GrAL 

#endif
