#ifndef GRAL_GB_EXTERNAL_CONTROL_CONTROLLER_H
#define GRAL_GB_EXTERNAL_CONTROL_CONTROLLER_H



// $LICENSE


#include <iostream>
#include <string> 

#include <boost/shared_ptr.hpp>
#include "IO/mutator.h"
#include "IO/checker.h"

namespace GrAL {


/*! \file 
    \brief abstract interface for controlling parameters.

     Parameters  may  for instance be 
     - read from a file
     - set interactively via a GUI or a command interpreter

     \fixed 21-04-2005
      Fixed the memory leak in control_device_impl::add(std::string const&, Mutator* m)
      and related classes by using <a target="parent" href="http://www.boost.org/libs/smart_ptr/shared_ptr.htm">boost::shared_ptr</a>.
*/





//  implementation class ("letter")
class control_device_impl : public controlable {
public:
  virtual void update() = 0;
  virtual void add(std::string const&, boost::shared_ptr<Mutator>) = 0;

  virtual void print_values(std::ostream&) const = 0;


  virtual void print_unrecognized(std::ostream&) const = 0;
  virtual void print_unrecognized(std::ostream&, std::string const&) const = 0;

  virtual void attach_to(std::istream& in) = 0;
  virtual boost::shared_ptr<control_device_impl> get_sub_device(std::string const& nm) = 0;

  virtual std::string name() const = 0;
  virtual ~control_device_impl() {}
};

/*! \brief Abstract interface to parameter control

    \see ControlDeviceCreators for creation
    \see \ref ex1.C  for examples
*/
class ControlDevice  {
public:
  ControlDevice() {}
  ControlDevice(boost::shared_ptr<control_device_impl> imp) : impl(imp) {}

  //! Add \c value_ref under name \c nm to this device
  void add(std::string const& nm, boost::shared_ptr<Mutator> value_ref);
  //! Add \c value_ref under name \c nm to this device
  void add(char        const* nm, boost::shared_ptr<Mutator> value_ref);
  //! Add \c value_ref under name \c nm with checking \c c to this device
  void add(std::string const& nm, boost::shared_ptr<Mutator> value_ref, Checker c);
  //! Add \c value_ref under name \c nm with checking \c c to this device
  void add(char        const* nm, boost::shared_ptr<Mutator> value_ref, Checker c);

  //! Read & update all registered parameters
  void update();

  //! print current values of parameters (for debugging)
  void print_values      (std::ostream&) const;
  //! print all keys which could not be found during last update
  void print_unrecognized(std::ostream&) const;
  void attach_to         (std::istream& in);

  //! Make this device controled by another device.
  void register_at(ControlDevice&, std::string const& prefix);

  std::string name() const;

  ControlDevice getSubDevice(std::string const& name);
  ControlDevice getSubDevice(char        const* name);
private:
  boost::shared_ptr<control_device_impl> impl;
};


//! register variable \c t to ControlDevice \c Ctrl
template<class T>
inline void RegisterAt(ControlDevice& Ctrl, std::string const& name, T& t)
{ 
  boost::shared_ptr<Mutator> p(new TypedMutator<T>(t));
  Ctrl.add(name, p); 
}

//! register variable \c t with checks \c c to ControlDevice  \c Ctrl
template<class T>
inline void RegisterAt(ControlDevice& Ctrl, std::string const& name, T& t, Checker c)
{ 
  boost::shared_ptr<Mutator> p(new TypedMutator<T>(t));
  p->set_checker(c);
  Ctrl.add(name, p); 
}

//! register variable t with limits checks to ControlDevice \c Ctrl
template<class T>
inline void RegisterAt(ControlDevice& Ctrl, std::string const& name, T& t, T const& lower_bound, T const& upper_bound)
{ 
  boost::shared_ptr<Mutator> p(new TypedMutator<T>(t));
  p->set_checker(GetLimitsChecker(t, lower_bound, upper_bound, name));
  Ctrl.add(name, p); 
}




//! register variable t to ControlDevice
template<class T>
inline void RegisterAt(ControlDevice& Ctrl, char const*   name, T& t)
{  Ctrl.add(name, boost::shared_ptr<Mutator>(new TypedMutator<T>(t))); }


/*! \defgroup ControlDeviceCreators Creator functions for ControlDevice

    For examples of using these creator functions (or factories), see <tt> \ref ex1.C</tt>.
*/

/*! \brief Create a  ControlDevice that reads name-value-pairs from a stream
    
    \ingroup ControlDeviceCreators 
 */
extern ControlDevice GetStreamDevice(std::istream* in, 
                                     std::string const& name = "");


/*! \brief Create a  ControlDevice that reads name-value-pairs from a file
 */
extern ControlDevice GetFileControlDevice(char const*   filename, 
                                          std::string const& name);

/*! \brief Create a ControlDevice that first reads from file and then from command line
    
    \ingroup ControlDeviceCreators 
 */
extern ControlDevice GetCommandlineAndFileControlDevice(int argc, char* argv[],
							std::string const& filename, 
							std::string const& name);

/*! \brief Create a ControlDevice that reads from given a stream and from a file
    
    \ingroup ControlDeviceCreators 
 */
extern ControlDevice GetDuplexControlDevice(std::istream     & in2,
					    char        const* filename, 
                                            std::string const& name);
    
/*! \brief Create a ControlDevice that reads from given a stream and from a file
    
    \ingroup ControlDeviceCreators 
 */
extern ControlDevice GetDuplexControlDevice(std::istream     & in2,
					    std::string const& filename, 
                                            std::string const& name);

} // namespace GrAL 

#endif
