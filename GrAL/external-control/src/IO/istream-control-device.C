#include "IO/istream-control-device.h"
#include "IO/skip-comments.h"


istream_control_device_impl::istream_control_device_impl                                        (istream* i, const string& nm, const string& ind = "")
  : in(i), name_(nm), indent_(ind) {}

string istream_control_device_impl::name() const { return name_ ;}

void istream_control_device_impl::update()        {  MV.ReadValues(*in);}

void istream_control_device_impl::add(const string& name,Mutator* value) 
{ MV.AddVariable(name,value);}

void istream_control_device_impl::register_at(ControlDevice& Ctrl) { register_at(Ctrl,"");}

void istream_control_device_impl::register_at(ControlDevice& Ctrl, 
                                              const string& prefix) {
  Ctrl.add( ( (prefix == "") || (name() == "") ? prefix : prefix + "-") +name(), 
            GetMutator(*this));
}

void istream_control_device_impl::attach_to(istream& in_new) { in = &in_new;}

void istream_control_device_impl::print_values(ostream& out) const { MV.PrintValues(out);}

void istream_control_device_impl::print_values(ostream& out, const string& ind) const 
{ MV.PrintValues(out, ind, " ");}




//istream_control_device_impl* 
control_device_impl* 
istream_control_device_impl::get_sub_device(const string& nm) 
{ 
  self* sub = new istream_control_device_impl(in,nm, indent_+" "); 
  add(nm,GetMutator(*sub));
  sub_devices_.push_back(sub);
  return sub;
}

void istream_control_device_impl::read(istream& in) {
  skip_comment(in);
  char c;
  in >> c;
  REQUIRE( (c == '{'),"control-device " << name()  << "no leading {!\n",1);
  //  if( c == '{') {
    (in >> ws).get(c);
    while( c != '}') {
      in.putback(c);
    
      skip_comment(in);
      (in >> ws).get(c); 
      if( c== '}') return;
      in.putback(c);

      MV.ReadVariable(in);
      (in >> ws).get(c); 
     }
    //  }
    print_unrecognized(cerr);
   
}

void istream_control_device_impl::print(ostream& out) const {
  out <<  '\n' << indent_ << "{\n";
  print_values(out, indent_);
  out << indent_ << "}";
}

void istream_control_device_impl::print_unrecognized(ostream& out) const
{ print_unrecognized(out,"");}

void istream_control_device_impl::print_unrecognized(ostream& out, 
						     const string& prefix) const
{
  string nm = ( prefix == "" ? name() : prefix + "::" + name());
  if(MV.HasUnrecognized()) {
    out << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n"
	<< "WARNING: Unrecognized Values in ControlDevice " << nm << ":\n";
    MV.PrintUnrecognized(out);
    out << "END Unrecognized Values in ControlDevice " << nm << '\n';
  }
   list<self*>::const_iterator subs;
  for(subs = sub_devices_.begin(); subs != sub_devices_.end(); ++subs)
    (*subs)->print_unrecognized(out,nm);
}
