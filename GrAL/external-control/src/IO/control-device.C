#include <fstream.h>
#include "mystring.h"

/* #include "Iso-std/sstream" */
#include <strstream.h> 

#include "IO/safe-file.h"
#include "IO/control-device.h"
#include "IO/istream-control-device.h"
#include "IO/multi-istream-control-device.h"
 
#include "Utility/pre-post-conditions.h" 

string Mutator::description() const { return string("");}


void ControlDevice::update() { impl->update(); print_unrecognized(cerr);}

void ControlDevice::print_values(ostream& out) const { impl->print_values(out);}

void ControlDevice::print_unrecognized(ostream& out) const { impl->print_unrecognized(out);}

void ControlDevice::attach_to(istream& in) { impl->attach_to(in);}

void ControlDevice::add(const string& name,Mutator* value_ref) 
{ impl->add(name,value_ref);}

void ControlDevice::add(const char*   nm,Mutator* value_ref)
{ add(string(nm),value_ref);}

void ControlDevice::register_at(ControlDevice& Ctrl, const string& prefix)
{ impl->register_at(Ctrl,prefix);}

string ControlDevice::name() const { return impl->name();}

ControlDevice ControlDevice::getSubDevice(const string& nm) 
{ 
  ControlDevice sub(impl->get_sub_device(nm));
  // sub.register_at(*this,"");
  return sub;
}

ControlDevice ControlDevice::getSubDevice(const char*   name) 
{ return getSubDevice(string(name));}





ControlDevice GetStreamDevice(istream* in, const string& name)
{ return ControlDevice( new istream_control_device_impl(in,name));}

ControlDevice GetFileControlDevice(const string& filename, const string& name) 
{ return GetFileControlDevice(filename.c_str(),name);} 

ControlDevice GetFileControlDevice(const char* filename, const string& name) {
 ifstream * infile = new ifstream;
 file_interactive::open(*infile,filename);
 return ControlDevice(new istream_control_device_impl(infile, name));
}


#include "IO/command-line.h"


ControlDevice GetDuplexControlDevice(istream& in2,
				     const char* filename, const string& name) {
 ifstream * in1 = new ifstream(filename);
  return ControlDevice(new multi_istream_control_device(in1,&in2,name));
}

ControlDevice GetDuplexControlDevice(istream* in2,
				     const char* filename, const string& name) {
 ifstream * in1 = new ifstream(filename);
  return ControlDevice(new multi_istream_control_device(in1,in2,name));
}

ControlDevice GetDuplexControlDevice(istream& in2,
				     const string& filename, const string& name) {
  return GetDuplexControlDevice(in2,filename.c_str(),name);
}

ControlDevice GetCommandlineAndFileControlDevice(int argc, char* argv[],
						 const string& filename, 
						 const string& name) 
{
  Commandline cmd(argc,argv);
  //  istringstream* in = new istringstream(cmd.get().str());
  istrstream* in = new istrstream(cmd.c_str());
  return GetDuplexControlDevice(in,
				filename.c_str(),
				name);
}
