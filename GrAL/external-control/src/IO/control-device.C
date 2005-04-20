
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

std::string Mutator::description() const { return ::std::string("");}


void ControlDevice::update() { impl->update(); print_unrecognized(::std::cerr);}

void ControlDevice::print_values(::std::ostream& out) const { impl->print_values(out);}

void ControlDevice::print_unrecognized(::std::ostream& out) const { impl->print_unrecognized(out);}

void ControlDevice::attach_to(::std::istream& in) { impl->attach_to(in);}

  void ControlDevice::add(std::string const& name, boost::shared_ptr<Mutator> value_ref) 
{ impl->add(name,value_ref);}

  void ControlDevice::add(char const*   nm, boost::shared_ptr<Mutator> value_ref)
{ add(::std::string(nm),value_ref);}

void ControlDevice::register_at(ControlDevice& Ctrl, ::std::string const& prefix)
{ impl->register_at(Ctrl,prefix);}

std::string ControlDevice::name() const { return impl->name();}

ControlDevice ControlDevice::getSubDevice(::std::string const& nm) 
{ 
  ControlDevice sub(impl->get_sub_device(nm));
  // sub.register_at(*this,"");
  return sub;
}

ControlDevice ControlDevice::getSubDevice(char const*   name) 
{ return getSubDevice(::std::string(name));}





ControlDevice GetStreamDevice(std::istream & in, const std::string& name)
{ 
  boost::shared_ptr<std::istream> inp(&in, null_deleter());
  return ControlDevice(boost::shared_ptr<control_device_impl>(new istream_control_device_impl(inp,name)));
}
	

ControlDevice GetFileControlDevice(const ::std::string& filename, const std::string& name) 
{ return GetFileControlDevice(filename.c_str(),name);} 

ControlDevice GetFileControlDevice(const char* filename, const std::string& name) {
  std::ifstream *inf(new std::ifstream(name.c_str()));
  REQUIRE_ALWAYS(inf->is_open(), "could not open file \"" << name << "\"!",1);
  boost::shared_ptr<std::istream> infile(inf);
  return ControlDevice(boost::shared_ptr<control_device_impl>(new istream_control_device_impl(infile, name)));
}




ControlDevice GetDuplexControlDevice(std::istream& in2,
				     const char* filename, const std::string& name) {
  boost::shared_ptr<std::istream> in1p(new std::ifstream(filename));
  boost::shared_ptr<std::istream> in2p(&in2, null_deleter());
  return ControlDevice(boost::shared_ptr<control_device_impl>(new multi_istream_control_device(in1p,in2p,name)));
}

  
  ControlDevice GetDuplexControlDevice(boost::shared_ptr<std::istream> in2,
				     const char* filename, const ::std::string& name) {
    boost::shared_ptr<std::istream> in1(new std::ifstream(filename));
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
