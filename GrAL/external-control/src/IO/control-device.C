
// $LICENSE

#include <fstream>
#include <string>

#ifdef GRAL_HAS_SSTREAM
#include <sstream>
#else
#include <strstream.h>
#endif

#include "Utility/safe-file.h"
#include "IO/control-device.h"
#include "IO/istream-control-device.h"
#include "IO/multi-istream-control-device.h"
#include "IO/command-line.h"
 
#include "Utility/pre-post-conditions.h" 
#include "Utility/ref-ptr.h" // for null_deleter

namespace GrAL {

std::string mutator_impl::description() const { return std::string("");}

  ControlDevice::ControlDevice() : required_flag(all_default_flag) {}
  
  ControlDevice::ControlDevice(boost::shared_ptr<control_device_impl> imp) 
    : impl(imp), required_flag(all_default_flag)  {}


  void ControlDevice::update(int flag, std::ostream& out) 
  { 
    impl->update(); 
    if(flag & show_unrecognized)
      print_unrecognized(out);
    if(flag & show_unread)
      print_unread_required(out);
  }



void ControlDevice::print_values(std::ostream& out) const { impl->print_values(out);}


bool ControlDevice::has_unrecognized() const { return impl->has_unrecognized();}
  
int  ControlDevice::num_unrecognized() const { return impl->num_unrecognized();}

void ControlDevice::print_unrecognized(std::ostream& out) const { impl->print_unrecognized(out);}


bool ControlDevice::has_unread_required()                    const { return impl->has_unread_required();}
  
int  ControlDevice::num_unread_required()                    const { return impl->num_unread_required();}

void ControlDevice::print_unread_required(std::ostream& out) const { impl->print_unread_required(out);}




void ControlDevice::attach_to(std::istream& in) { impl->attach_to(in);}

  void ControlDevice::all_required() 
  { 
    required_flag = all_required_flag;
  }

  void ControlDevice::all_optional() 
  { 
    required_flag = all_optional_flag;
  }

  void ControlDevice::all_default()
  {
    required_flag = all_default_flag;
  }

  void ControlDevice::add(std::string const& name, Mutator value_ref) 
  { 
    if(required_flag == all_required_flag)
      value_ref.make_required();
    else if(required_flag == all_optional_flag)
      value_ref.make_optional();
    impl->add(name,value_ref);
  }

  void ControlDevice::add(std::string const& name, Mutator value_ref, Checker c) 
  { 
    value_ref.set_checker(c); 
    add(name, value_ref);
  }
  void ControlDevice::add(char        const* name, Mutator value_ref, Checker c) 
  { 
    value_ref.set_checker(c); 
    add(name, value_ref);
  }

  void ControlDevice::add(char const*   nm, Mutator  value_ref)
  { add(std::string(nm),value_ref);}

void ControlDevice::register_at(ControlDevice& Ctrl, std::string const& prefix)
{ impl->register_at(Ctrl,prefix);}

std::string ControlDevice::name() const { return impl->name();}

ControlDevice ControlDevice::getSubDevice(std::string const& nm) 
{ 
  ControlDevice sub(impl->get_sub_device(nm));
  // sub.register_at(*this,"");
  return sub;
}

ControlDevice ControlDevice::getSubDevice(char const*   name) 
{ return getSubDevice(std::string(name));}





ControlDevice GetStreamDevice(std::istream & in, const std::string& name)
{ 
  boost::shared_ptr<std::istream> inp(&in, null_deleter());
  return ControlDevice(boost::shared_ptr<control_device_impl>(new istream_control_device_impl(inp,name)));
}
	

ControlDevice GetFileControlDevice(const std::string& filename, const std::string& name) 
{ return GetFileControlDevice(filename.c_str(),name);} 

ControlDevice GetFileControlDevice(const char* filename, const std::string& name) {
  std::ifstream *inf(new std::ifstream(filename));
  REQUIRE_ALWAYS(inf->is_open(), "could not open file \"" << filename << "\"!",1);
  boost::shared_ptr<std::istream> infile(inf);
  return ControlDevice(boost::shared_ptr<control_device_impl>(new istream_control_device_impl(infile, name)));
}




ControlDevice GetDuplexControlDevice(std::istream& in1,
				     const char* filename, const std::string& name) {
  boost::shared_ptr<std::istream> in1p(&in1, null_deleter());
  boost::shared_ptr<std::istream> in2p(new std::ifstream(filename));
  return ControlDevice(boost::shared_ptr<control_device_impl>(new multi_istream_control_device(in1p,in2p,name)));
}

  
  ControlDevice GetDuplexControlDevice(boost::shared_ptr<std::istream> in1,
				     const char* filename, const std::string& name) {
    boost::shared_ptr<std::istream> in2(new std::ifstream(filename));
    return ControlDevice(boost::shared_ptr<control_device_impl>(new multi_istream_control_device(in1,in2,name)));
  }
  

ControlDevice GetDuplexControlDevice(std::istream& in2,
				     const std::string& filename, const std::string& name) {
  return GetDuplexControlDevice(in2,filename.c_str(),name);
}

ControlDevice GetCommandlineAndFileControlDevice(int argc, char* argv[],
						 const std::string& filename, 
						 const std::string& name) 
{
  Commandline cmd(argc,argv);
  //  istringstream* in = new istringstream(cmd.get().str());
#ifdef GRAL_HAS_SSTREAM
  boost::shared_ptr<std::istream> in(new std::istringstream(cmd.str()));
#else
  boost::shared_ptr<std::istream> in(new std::istrstream(cmd.c_str()));
#endif
  return GetDuplexControlDevice(in,
				filename.c_str(),
				name);
}

} // namespace GrAL 
