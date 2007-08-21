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
  virtual void add(std::string const&, Mutator) = 0;

  virtual void print_values(std::ostream&) const = 0;

  virtual bool has_unrecognized() const = 0;
  virtual int  num_unrecognized() const = 0;
  virtual void print_unrecognized(std::ostream&) const = 0;
  virtual void print_unrecognized(std::ostream&, std::string const&) const = 0;

  virtual bool has_unread_required() const = 0;
  virtual int  num_unread_required() const = 0;
  virtual void print_unread_required(std::ostream&) const = 0;
  virtual void print_unread_required(std::ostream&, std::string const&) const = 0;

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
  /*! \name Constructors 
    
   A ControlDevice is typically constructed by a factory function 
   (see module \ref ControlDeviceCreators).
  */
  //@{
  ControlDevice();
  ControlDevice(boost::shared_ptr<control_device_impl> imp);
  //@}

  //! \brief Add \c value_ref under name \c nm to this device
  void add(std::string const& nm, Mutator value_ref);
  //! \brief Add \c value_ref under name \c nm to this device
  void add(char        const* nm, Mutator value_ref);
  //! \brief Add \c value_ref under name \c nm with checking \c c to this device
  void add(std::string const& nm, Mutator value_ref, Checker c);
  //! \brief Add \c value_ref under name \c nm with checking \c c to this device
  void add(char        const* nm, Mutator value_ref, Checker c);

  //! \brief Make all subsequently added parameters required
  void all_required();
  //! \brief Make all subsequently added parameters optional
  void all_optional();
  //! \brief Use the parameter's own setting for all subsequently added parameters
  void all_default();

  //! \brief Selection of errors to show 
  enum update_flag { quiet = 0,
		     show_unrecognized = 1, 
		     show_unread = (show_unrecognized << 1) 
  };
  //! \brief Read & update all registered parameters
  void update(int flag      = show_unrecognized | show_unread, 
	      std::ostream& = std::cerr);

  //! \brief True if there are unrecognized values
  bool has_unrecognized() const;
 
  //! \brief Number of unrecognized values
  int  num_unrecognized() const;
 
  //! \brief print current values of parameters (for debugging)
  void print_values      (std::ostream&) const;
  //! \brief print all keys which could not be found during last update
  void print_unrecognized(std::ostream&) const;

  //! \brief  True if there are required parameters not read in update()
  bool has_unread_required() const;
  //! \brief Number of  required parameters not read in update()
  int  num_unread_required() const;
  //! \brief Print all required parmeters not read in update()
  void print_unread_required(std::ostream&) const;


  void attach_to         (std::istream& in);

  //! Make this device controled by another device.
  void register_at(ControlDevice&, std::string const& prefix);

  std::string name() const;

  ControlDevice getSubDevice(std::string const& name);
  ControlDevice getSubDevice(char        const* name);
private:
  boost::shared_ptr<control_device_impl> impl;
  enum { all_default_flag, all_required_flag, all_optional_flag };
  int  required_flag;
};


  /*! \defgroup controldeviceregistrators  ControlDevice variable registration functions 

      \brief Convenience functions to wrap the most common cases of registring a variable to a control device.

      For examples of using these  functions, see <tt> \ref ex1.C</tt>.
  */

  /*! \brief register variable \c t to ControlDevice \c Ctrl

      \ingroup  controldeviceregistrators
   */
template<class T>
inline void RegisterAt(ControlDevice& Ctrl, std::string const& name, T& t)
{ 
  Ctrl.add(name, GetMutator(t)); 
}

  /*! \brief  register variable \c t with checks \c c to ControlDevice  \c Ctrl

      \ingroup  controldeviceregistrators
  */
template<class T>
inline void RegisterAt(ControlDevice& Ctrl, std::string const& name, T& t, Checker c)
{ 
  Mutator m = GetMutator(t);
  m.set_checker(c);
  Ctrl.add(name, m); 
}

  /*! \brief  register variable t with limits checks to ControlDevice \c Ctrl
 
      \ingroup  controldeviceregistrators
   */
template<class T>
inline void RegisterAt(ControlDevice& Ctrl, std::string const& name, T& t, T const& lower_bound, T const& upper_bound)
{ 
  Mutator  m = GetMutator(t);
  m.set_checker(GetLimitsChecker(t, lower_bound, upper_bound, name));
  Ctrl.add(name, m); 
}


  /*! \brief register variable t to ControlDevice

      \ingroup  controldeviceregistrators
  */
template<class T>
inline void RegisterAt(ControlDevice& Ctrl, char const*   name, T& t)
{  Ctrl.add(name, GetMutator(t)); }

/*! \brief Convenience function to add a help text to a ControlDevice

    \ingroup  controldeviceregistrators

    \b Example:
    \code
     ControlDevice Ctrl = ...
     string h = "foo: make bars. Usage:\n";
    
     int nbars;
     RegisterAt(Ctrl, "-n", nbars);
     h += " -n <nbars> (number of bars)\n";

     // ...
     AddHelp(Ctrl, h);
     \endcode

     \note It would probably be better to have the help strings tied to the individual parameters
     by using and additional optional argument to RegisterAt. 
 */
void AddHelp(GrAL::ControlDevice & Ctrl, std::string const& help);

/*! \defgroup ControlDeviceCreators Creator functions for ControlDevice

    For examples of using these creator functions (or factories), see <tt> \ref ex1.C</tt>.
*/

/*! \brief Create a  ControlDevice that reads name-value-pairs from a stream
    
    \ingroup ControlDeviceCreators 
 */
extern ControlDevice GetStreamDevice(std::istream& in, 
                                     std::string const& name = "");


/*! \brief Create a  ControlDevice that reads name-value-pairs from a string
    
    \ingroup ControlDeviceCreators 
 */
extern ControlDevice GetStringControlDevice(std::string const& params,
					    std::string const& name = "");


/*! \brief Create a  ControlDevice that reads name-value-pairs from a file

    \ingroup ControlDeviceCreators 
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
