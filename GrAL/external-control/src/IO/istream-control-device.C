

/* ------------------------------------------------------------

    Copyright (C) 1997 - 2009 Guntram Berti, 
    Contact: guntram AT users DOT berlios DOT de

    This file is part of the Grid Algorithms Library (GrAL),
    available at http://gral.berlios.de

    GrAL is distributed under the MIT license,
    see the file LICENSE or http://gral.berlios.de/license

--------------------------------------------------------------- */


#include "IO/istream-control-device.h"
#include "IO/skip-comments.h"
#include "Utility/ref-ptr.h"

namespace GrAL {


  istream_control_device_impl::istream_control_device_impl(boost::shared_ptr<std::istream> i, 
							   std::string const& nm, 
							   std::string const& ind /* = "" */)
    : in(i), name_(nm), indent_(ind) {}

  std::string istream_control_device_impl::name() const { return name_ ;}

  void istream_control_device_impl::update()        {  MV.ReadValues(*in);}

  void istream_control_device_impl::add(const std::string& name, Mutator value) 
  { MV.AddVariable(name,value);}

  void istream_control_device_impl::register_at(ControlDevice& Ctrl) { register_at(Ctrl,"");}
  
  void istream_control_device_impl::register_at(ControlDevice& Ctrl, 
						const std::string& prefix) {
    Ctrl.add( ( (prefix == "") || (name() == "") ? prefix : prefix + "-") +name(), 
	      GetMutator(*this));
}

  void istream_control_device_impl::attach_to(std::istream& in_new) { 
    boost::shared_ptr<std::istream> in_new_p(&in_new, null_deleter());
    boost::swap(in, in_new_p);
  } 

  void istream_control_device_impl::print_values(std::ostream& out) const { MV.PrintValues(out);}
  
  void istream_control_device_impl::print_values(std::ostream& out, 
						 std::string const& ind) const 
  { MV.PrintValues(out, ind, " ");}




//istream_control_device_impl* 
  boost::shared_ptr<control_device_impl>
  istream_control_device_impl::get_sub_device(std::string const& nm) 
  { 
    boost::shared_ptr<self> sub(new istream_control_device_impl(in,nm, indent_+" ")); 
    add(nm,GetMutator(*sub));
    sub_devices_.push_back(sub);
    return sub;
  }

  void istream_control_device_impl::read(std::istream& in) {
    skip_comment(in);
    char c;
    in >> c;
    REQUIRE( (c == '{'),"control-device " << name()  << "no leading {!\n",1);
    //  if( c == '{') {
    (in >> std::ws).get(c);
    while( c != '}') {
      in.putback(c);
      
      skip_comment(in);
      (in >> std::ws).get(c); 
      if( c== '}') return;
      in.putback(c);

      MV.ReadVariable(in);
      (in >> std::ws).get(c); 
     }
    //  }
    print_unrecognized(std::cerr);
   
}

  void istream_control_device_impl::print(std::ostream& out) const {
    out <<  '\n' << indent_ << "{\n";
    print_values(out, indent_);
    out << indent_ << "}";
  }


  bool istream_control_device_impl::has_unrecognized() const {
    bool res = MV.HasUnrecognized();
    std::list<boost::shared_ptr<self> >::const_iterator subs;
    for(subs = sub_devices_.begin(); subs != sub_devices_.end(); ++subs)
      res = res || (*subs)->has_unrecognized();
    return res;
  }

  int  istream_control_device_impl::num_unrecognized() const {
    int res = MV.NumUnrecognized();
    std::list<boost::shared_ptr<self> >::const_iterator subs;
    for(subs = sub_devices_.begin(); subs != sub_devices_.end(); ++subs)
      res +=  (*subs)->num_unrecognized();
    return res;
  }
  
  void istream_control_device_impl::print_unrecognized(std::ostream& out) const
  { print_unrecognized(out,"");}
  
  void istream_control_device_impl::print_unrecognized(std::ostream& out, 
						       std::string const& prefix) const
  {
    std::string nm = ( prefix == "" ? name() : prefix + "::" + name());
    if(MV.HasUnrecognized()) {
      out << "WARNING: Unrecognized Values in ControlDevice " << nm << ":\n";
      MV.PrintUnrecognized(out);
      out << "END Unrecognized Values in ControlDevice " << nm << '\n';
    }
    std::list<boost::shared_ptr<self> >::const_iterator subs;
    for(subs = sub_devices_.begin(); subs != sub_devices_.end(); ++subs)
      (*subs)->print_unrecognized(out,nm);
  }



  bool istream_control_device_impl::has_unread_required() const { 
    bool res = MV.HasUnreadRequired();
        std::list<boost::shared_ptr<self> >::const_iterator subs;
    for(subs = sub_devices_.begin(); subs != sub_devices_.end(); ++subs)
      res = res || (*subs)->has_unread_required();
    return res;
  }
  
  int  istream_control_device_impl::num_unread_required() const {
    int res = MV.NumUnreadRequired();
    std::list<boost::shared_ptr<self> >::const_iterator subs;
    for(subs = sub_devices_.begin(); subs != sub_devices_.end(); ++subs)
      res += (*subs)->num_unread_required();
    return res;
  }

  void istream_control_device_impl::print_unread_required(std::ostream& out) const { print_unread_required(out,"");}

  void istream_control_device_impl::print_unread_required(std::ostream& out, std::string const& prefix) const {
    std::string nm = ( prefix == "" ? name() : prefix + "::" + name());
    if(MV.HasUnreadRequired()) {
      out << "WARNING: The following required values were not found in ControlDevice " << nm << ":\n";
      MV.PrintUnreadRequired(out);
      out << "End missing required values in ControlDevice " << nm << std::endl;
    }
    std::list<boost::shared_ptr<self> >::const_iterator subs;
    for(subs = sub_devices_.begin(); subs != sub_devices_.end(); ++subs)
      (*subs)->print_unread_required(out,nm);
  }


} // namespace GrAL 
