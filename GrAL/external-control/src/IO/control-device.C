#include <fstream.h>
#include <string>
#include <strstream.h> 

#include "Utility/safe-file.h"
#include "IO/control-device.h"
#include "IO/istream-control-device.h"
#include "IO/multi-istream-control-device.h"
#include "IO/command-line.h"
 
#include "Utility/pre-post-conditions.h" 

std::string Mutator::description() const { return std::string("");}


void ControlDevice::update() { impl->update(); print_unrecognized(cerr);}

void ControlDevice::print_values(std::ostream& out) const { impl->print_values(out);}

void ControlDevice::print_unrecognized(std::ostream& out) const { impl->print_unrecognized(out);}

void ControlDevice::attach_to(std::istream& in) { impl->attach_to(in);}

void ControlDevice::add(std::string const& name,Mutator* value_ref) 
{ impl->add(name,value_ref);}

void ControlDevice::add(char const*   nm,Mutator* value_ref)
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





ControlDevice GetStreamDevice(std::istream* in, const std::string& name)
{ return ControlDevice( new istream_control_device_impl(in,name));}

ControlDevice GetFileControlDevice(const std::string& filename, const std::string& name) 
{ return GetFileControlDevice(filename.c_str(),name);} 

ControlDevice GetFileControlDevice(const char* filename, const std::string& name) {
 std::ifstream * infile = new std::ifstream;
 file_interactive::open(*infile,filename);
 return ControlDevice(new istream_control_device_impl(infile, name));
}




ControlDevice GetDuplexControlDevice(std::istream& in2,
				     const char* filename, const std::string& name) {
 std::ifstream * in1 = new std::ifstream(filename);
  return ControlDevice(new multi_istream_control_device(in1,&in2,name));
}

ControlDevice GetDuplexControlDevice(std::istream* in2,
				     const char* filename, const std::string& name) {
 std::ifstream * in1 = new std::ifstream(filename);
  return ControlDevice(new multi_istream_control_device(in1,in2,name));
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
  std::istrstream* in = new std::istrstream(cmd.c_str());
  return GetDuplexControlDevice(in,
				filename.c_str(),
				name);
}
